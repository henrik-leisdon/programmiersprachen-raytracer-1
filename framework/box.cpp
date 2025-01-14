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

Hit Box::intersect(Ray const &firstRay, float &t) {
Ray ray;
if(isTransformed_) {
    ray = transformRay(inverse_world_transform_, firstRay);
} else {
    ray = firstRay;
}

if (boxMin_.x > boxMax_.x) std::swap(boxMin_.x, boxMax_.x);
if (boxMin_.y > boxMax_.y) std::swap(boxMin_.y, boxMax_.y);
if (boxMin_.z > boxMax_.z) std::swap(boxMin_.z, boxMax_.z);

//cout << "boxMin z" << boxMin_.z << " boxMax z" << boxMax_.z << endl;

vec3 dirNormaized = normalize(ray.direction);

float xMinDist = (boxMin_.x-ray.origin.x)/dirNormaized.x;
float xMaxDist = (boxMax_.x-ray.origin.x)/dirNormaized.x;
float yMinDist = (boxMin_.y-ray.origin.y)/dirNormaized.y;
float yMaxDist = (boxMax_.y-ray.origin.y)/dirNormaized.y;
float zMaxDist = (boxMin_.z-ray.origin.z)/dirNormaized.z;
float zMinDist = (boxMax_.z-ray.origin.z)/dirNormaized.z;
//cout << "min dist: " << zMinDist << " max dist " << zMaxDist  <<endl;
//cout << "dir x min: " << xMinDist << " dir x max: " << xMaxDist << "dir y min: " << yMinDist << " dir y max: " << yMaxDist << "dir z min: " << zMinDist << " dir z max: " << zMaxDist << endl;

//possible hitpoints:
vec3 xMinHitP = ray.origin + xMinDist*(dirNormaized);
vec3 xMaxHitP = ray.origin + xMaxDist*(dirNormaized);
vec3 yMinHitP = ray.origin + yMinDist*(dirNormaized);
vec3 yMaxHitP = ray.origin + yMaxDist*(dirNormaized);
vec3 zMinHitP = ray.origin + zMinDist*(dirNormaized);
vec3 zMaxHitP = ray.origin + zMaxDist*(dirNormaized);

Hit hitpoints[6];

//cout << "xMinHitPoint: " << xMinHitP.x << " " << xMinHitP.y << " " << xMinHitP.z << endl;

//left:

hitpoints[0].hit_ = false;
if(xMinHitP.y <= boxMax_.y && xMinHitP.y >= boxMin_.y &&
    xMinHitP.z <= boxMax_.z && xMinHitP.z >= boxMin_.z && xMinDist > 0)
{
    //cout << "hit inleft";
    hitpoints[0].hit_ = true;
    hitpoints[0].hitpoint_ = xMinHitP;
    hitpoints[0].dist_ = xMinDist;
    hitpoints[0].hitnormal_ = vec3{-1.0f,0.0f,0.0f};
}

//right:
hitpoints[1].hit_ = false;
if(xMaxHitP.y <=boxMax_.y && xMaxHitP.y >= boxMin_.y
    && xMaxHitP.z <= boxMax_.z && xMaxHitP.z >= boxMin_.z && xMaxDist > 0)
{
    hitpoints[1].hit_ = true;
    hitpoints[1].hitpoint_ = xMaxHitP;
    hitpoints[1].dist_ = xMaxDist;
    hitpoints[1].hitnormal_ = vec3{1.0f,0.0f,0.0f};
}

//bottom
hitpoints[2].hit_ = false;
if(yMinHitP.x <= boxMax_.x && yMinHitP.x >= boxMin_.x
    && yMinHitP.z <= boxMax_.z && yMinHitP.z >= boxMin_.z && yMinDist > 0)
{
    hitpoints[2].hit_ = true;
    hitpoints[2].hitpoint_ = yMinHitP;
    hitpoints[2].dist_ = yMinDist;
    hitpoints[2].hitnormal_ = vec3{0.0f,-1.0f,0.0f};
}

//top:
hitpoints[3].hit_ = false;
if(yMaxHitP.x <= boxMax_.x && yMaxHitP.x >= boxMin_.x
    && yMaxHitP.z <= boxMax_.z && yMaxHitP.z >= boxMin_.z && yMaxDist > 0)
{
    hitpoints[3].hit_ = true;
    hitpoints[3].hitpoint_ = yMaxHitP;
    hitpoints[3].dist_ = yMaxDist;
    hitpoints[3].hitnormal_ = vec3{0.0f,1.0f,0.0f};

}

//front:
hitpoints[4].hit_ = false;
if(zMinHitP.x <=boxMax_.x && zMinHitP.x >= boxMin_.x
    && zMinHitP.y <= boxMax_.y && zMinHitP.y >= boxMin_.y && zMinDist > 0)
{
    hitpoints[4].hit_ = true;
    hitpoints[4].hitpoint_ = zMinHitP;
    hitpoints[4].dist_ = zMinDist;
    hitpoints[4].hitnormal_ = vec3{0.0f,0.0f,1.0f};

}

//back:
hitpoints[5].hit_ = false;
if(zMaxHitP.x <=boxMax_.x && zMaxHitP.x >= boxMin_.x
    && zMaxHitP.y <= boxMax_.y && zMaxHitP.y >= boxMin_.y && zMaxDist > 0)
{
    hitpoints[5].hit_ = true;
    hitpoints[5].hitpoint_ = zMaxHitP;
    hitpoints[5].dist_ = zMaxDist;
    hitpoints[5].hitnormal_ = vec3{0.0f,0.0f,-1.0f};

}

float smallestDistance = 10000000.0f;
Hit closestHit;
 closestHit.hit_ = false;
 for(int i = 0; i< 6; i++) {
     if(hitpoints[i].dist_<smallestDistance && hitpoints[i].hit_==true) {
         //cout << "closest hit: " << hitpoints[i].hitpoint_.x << " " << hitpoints[i].hitpoint_.y << " " << hitpoints[i].hitpoint_.z << endl;
         closestHit = hitpoints[i];
         smallestDistance = hitpoints[i].dist_;
     }
 }

if(isTransformed_) {
    vec4 transformHit = world_transform_ * vec4{closestHit.hitpoint_, 1};
    closestHit.hitpoint_ = vec3{transformHit.x, transformHit.y, transformHit.z};

    vec4 transformNormal = glm::normalize(transpose(inverse_world_transform_)*vec4{closestHit.hitnormal_, 0});
    closestHit.hitnormal_ = vec3{transformNormal.x, transformNormal.y, transformNormal.z};

    //closestHit.hitnormal_ = vec3(mat3(transpose(inverse_world_transform_))* closestHit.hitnormal_);
}

return closestHit;

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


/*
 Hit Box::intersect(Ray const &ray, float &t) {
    float finalDistance;
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
 */
