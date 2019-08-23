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
    float finalDistance;
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

    if(distance1 > 0) {
        cutting_pt={ray.origin + distance1 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeOne.normal);
            test= true;
        }
    }

    if(distance4 > 0) {
        cutting_pt={ray.origin + distance4 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeOne.normal);
            test= true;
        }
    }

    if(distance2 > 0) {
        cutting_pt={ray.origin + distance2 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeOne.normal);
            test= true;
        }
    }

    if(distance5 > 0) {
        cutting_pt={ray.origin + distance5 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeOne.normal);
            test= true;
        }
    }

    if(distance3 > 0) {
        cutting_pt={ray.origin + distance3 * ray.direction};
        if(cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeOne.normal);
            test= true;
        }
    }

    if(distance6 > 0) {
        cutting_pt={ray.origin + distance6 * ray.direction};
        if(cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeOne.normal);
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
                normDir = normalize(ray.direction);
                test = true;
            }
        }

        finalDistance = glm::length(cutting_pt - ray.origin);
        result = {test, finalDistance, closest_cut, normDir, closest_normal};
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


