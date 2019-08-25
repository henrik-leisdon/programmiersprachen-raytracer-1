#include <glm-0.9.5.3/glm/glm.hpp>
#include "box.hpp"
#include "plane.hpp"
#include <vector>

using namespace std;
using namespace glm;

Box::Box():
    Shape(),
    boxMin_({0.0,0.0,0.0}),
    boxMax_({0.0,0.0,0.0})
    {};

Box::Box(string name, vec3 const& boxMin, vec3 const& boxMax, shared_ptr<Material> material):
    Shape{name, material},
    boxMin_{boxMin},
    boxMax_{boxMax}
    {};

Box::~Box() {};

//getter
vec3 Box::getBoxMin()
{
    return boxMin_;
}

vec3 Box::getBoxMax()
{
    return boxMax_;
}

double Box::area() const 
{
    double length{boxMax_.x-boxMin_.x};
    double height{boxMax_.y-boxMin_.y};
    double width{boxMax_.z-boxMin_.z};

    return 2*length*height+2*length*width+2*height*width;
}

double Box::volume() const {

    double length{boxMax_.x-boxMin_.x};
    double height{boxMax_.y-boxMin_.y};
    double width{boxMax_.z-boxMin_.z};

    double volume = length*height*width;
    return volume;
}

Hit Box::intersect(Ray const &ray, float &t) {
    Hit result;
    bool test;
    vec3 bounds[2];
    int sign[3];
    float tMin, tMax, tyMin, tyMax, tzMin, tzMax;
    float distance;

    vector<vec3> cut_pts;
    vector<vec3> cut_norms;

    bounds[0] = boxMin_;
    bounds[1] = boxMax_;

    vec3 inverseDirection = {1.0f/ray.direction.x , 1.0f/ray.direction.y , 1.0f/ray.direction.z};

    sign[0] = (inverseDirection.x < 0);
    sign[1] = (inverseDirection.y < 0);
    sign[2] = (inverseDirection.z < 0);

    //distances
    tMin = (bounds[sign[0]].x - ray.origin.x)*inverseDirection.x;
    tMax = (bounds[1 - sign[0]].x - ray.origin.x)*inverseDirection.x;

    tyMin = (bounds[sign[1]].y - ray.origin.y)*inverseDirection.y;
    tyMax = (bounds[1 - sign[1]].y - ray.origin.y)*inverseDirection.y;

    tzMin = (bounds[sign[2]].z - ray.origin.z)*inverseDirection.z;
    tzMax = (bounds[1 - sign[2]].z - ray.origin.z)*inverseDirection.z;

    if( (tMin > tyMax) || (tyMin > tMax)) { test = false;}
    if( tyMin > tMin) { tMin = tyMin;}
    if( tyMax < tMax) { tMax = tyMax;}

    if( (tMin > tzMax) || (tzMin > tMax)) { test = false;}
    if( tzMin > tMin) { tMin = tzMin;}
    if( tzMax < tMax) { tMax = tzMax;}

    if( (tMin >= t) && (tMax <= t)) { test = true; }

    vec3 cut_pt;
    vec3 cut_norm;

    if(tMin != 0){
        cut_pt = ray.origin + tMin * ray.direction;
        cut_norm = {-1.0f, 0.0f, 0.0f};
        if(cut_pt.y <= boxMax_.y && cut_pt.y >= boxMin_.y && cut_pt.z <= boxMax_.z && cut_pt.z >= boxMin_.z) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    if(tMax != 0){
        cut_pt = ray.origin + tMax * ray.direction;
        cut_norm = {1.0f, 0.0f ,0.0f};
        if(cut_pt.y <= boxMax_.y && cut_pt.y >= boxMin_.y && cut_pt.z <= boxMax_.z && cut_pt.z >= boxMin_.z) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    if(tyMin != 0){
        cut_pt = ray.origin + tyMin * ray.direction;
        cut_norm = {0.0f, -1.0f, 0.0f};
        if(cut_pt.x <= boxMax_.x && cut_pt.x >= boxMin_.x && cut_pt.z <= boxMax_.z && cut_pt.z >= boxMin_.z) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    if(tyMax != 0){
        cut_pt = ray.origin + tyMax * ray.direction;
        cut_norm = {0.0f, 1.0f, 0.0f};
       if(cut_pt.x <= boxMax_.x && cut_pt.x >= boxMin_.x && cut_pt.z <= boxMax_.z && cut_pt.z >= boxMin_.z) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    if(tzMin != 0){
        cut_pt = ray.origin + tzMin * ray.direction;
        cut_norm = {0.0f, 0.0f, -1.0f};
       if(cut_pt.x <= boxMax_.x && cut_pt.x >= boxMin_.x && cut_pt.y <= boxMax_.y && cut_pt.y >= boxMin_.y) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    if(tzMax != 0){
        cut_pt = ray.origin + tzMax * ray.direction;
        cut_norm = {0.0f, 0.0f, 1.0f};
        if(cut_pt.x <= boxMax_.x && cut_pt.x >= boxMin_.x && cut_pt.y <= boxMax_.y && cut_pt.y >= boxMin_.y) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    vec3 closest_cut;
    vec3 closest_norm;

    if(cut_pts.size() > 0){
        closest_cut = cut_pts.at(0);
        closest_norm = cut_norms.at(0);
        //closest_norm = cut_norms.at(0);
        for(auto i = 0; i < cut_pts.size(); ++i){
            if(glm::length(cut_pts.at(i) - ray.origin) < glm::length(closest_cut - ray.origin)){
                closest_cut = cut_pts.at(i);
                closest_norm = cut_norms.at(i);
                cout << "----------------- got normal ----------------" << closest_norm.x << " " << closest_norm.y << " " << closest_norm.z << "\n";
            }
        }

        vec3 directionR = ray.direction;
        vec3 normDirection = glm::normalize(directionR);

        distance = glm::length(cut_pt - ray.origin);
        result={test, distance, cut_pt, normDirection, closest_norm};

        cout << "test " << test << "\n";
        cout << "distance " << distance << "\n";
        cout << "hitpoint " << cut_pt.x << " " << cut_pt.y << " " << cut_pt.z << "\n";
        cout << "direction " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << "\n";
        cout << "normalized direction " << normDirection.x << " " << normDirection.y << " " << normDirection.z << "\n";
        cout << "normal " << closest_norm.x << " " << closest_norm.y << " " << closest_norm.z << "\n";
        return result;
    }
}

ostream& Box::print(ostream &os) const {
    Shape::print(os);
    os
    << "Position Min : " << boxMin_.x << ", " << boxMin_.y << ", " << boxMin_.z << "\n"
    << "Position Max : " << boxMax_.x << ", " << boxMax_.y << ", " << boxMax_.z << "\n";

    return os;
}

ostream& operator << (ostream& os, const Box& b) {
    return b.print(os);
}

/*float finalDistance;
    bool test;
    Hit result;

    vec3 normDir;
    vec3 cutting_pt;
    vec3 closest_cut;
    vec3 closest_normal;
    vector<vec3> cutting_pts;
    vector<vec3> cutting_norm;

    Plane planeOne   {boxMin_, vec3{-1,0,0} };
    Plane planeTwo   {boxMin_, vec3{0,0,-1} };
    Plane planeThree {boxMin_, vec3{0,-1,0} };
    Plane planeFour  {boxMax_, vec3{1,0,0 } };
    Plane planeFive  {boxMax_, vec3{0,0,1 } };
    Plane planeSix   {boxMax_, vec3{0,1,0 } };

    float distance1 = (dot(planeOne.normal, planeOne.origin) - dot(ray.origin, planeOne.normal)) / (dot(ray.direction, planeOne.normal));
    float distance2 = (dot(planeTwo.normal, planeTwo.origin) - dot(ray.origin, planeTwo.normal)) / (dot(ray.direction, planeTwo.normal));
    float distance3 = (dot(planeThree.normal, planeThree.origin) - dot(ray.origin, planeThree.normal)) / (dot(ray.direction, planeThree.normal));
    float distance4 = (dot(planeFour.normal, planeFour.origin) - dot(ray.origin, planeFour.normal)) / (dot(ray.direction, planeFour.normal));
    float distance5 = (dot(planeFive.normal, planeFive.origin) - dot(ray.origin, planeFive.normal)) / (dot(ray.direction, planeFive.normal));
    float distance6 = (dot(planeSix.normal, planeSix.origin) - dot(ray.origin, planeSix.normal)) / (dot(ray.direction, planeSix.normal));

    normDir = normalize(ray.direction);

    if(distance1 != 0) {
        cutting_pt={ray.origin + distance1 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeOne.normal);
            test= true;
        }
    }

    if(distance4 != 0) {
        cutting_pt={ray.origin + distance4 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeFour.normal);
            test= true;
        }
    }

    if(distance2 != 0) {
        cutting_pt={ray.origin + distance2 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeTwo.normal);
            test= true;
        }
    }

    if(distance5 != 0) {
        cutting_pt={ray.origin + distance5 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeFive.normal);
            test= true;
        }
    }normDir = normalize(ray.direction);

    if(distance3 != 0) {
        cutting_pt={ray.origin + distance3 * ray.direction};
        if(cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeThree.normal);
            test= true;
        }
    }

    if(distance6 > 0) {
        cutting_pt={ray.origin + distance6 * ray.direction};
        if(cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeSix.normal);
            test= true;
        }
    }

    if(cutting_pts.size() > 0) {
        closest_cut = cutting_pts.at(0);
        closest_normal = cutting_norm.at(0);
        for(auto i = 0; i < cutting_pts.size(); ++i){
            if(glm::length(cutting_pts.at(i) - ray.origin) < glm::length(closest_cut - ray.origin)){
                closest_cut = cutting_pts.at(i);
                closest_normal = cutting_norm.at(i);
                cout << "dist " << finalDistance << "\n";
                cout << "hitpoint " << closest_cut.x << " " << closest_cut.y << " " << closest_cut.z << "\n";
                cout << "normal " << closest_normal.x << " " << closest_normal.y << " " << closest_normal.z << "\n";
                test = true;
            }
        }

        finalDistance = glm::length(cutting_pt - ray.origin);

        //cout << "dist " << finalDistance << "\n";
        //cout << "hitpoint " << closest_cut.x << " " << closest_cut.y << " " << closest_cut.z << "\n";
        //cout << "normal " << closest_normal.x << " " << closest_normal.y << " " << closest_normal.z << "\n";

    }

    result = {test, finalDistance, closest_cut, normDir , closest_normal};
    return result;
     */
