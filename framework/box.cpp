#include <glm-0.9.5.3/glm/glm.hpp>
#include "box.hpp"

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

    float ta[6];
    bool test = false;

    /*if(boxMin_.x > boxMax_.x) swap(boxMin_.x, boxMax_.x);
    if(boxMin_.y > boxMax_.y) swap(boxMin_.y, boxMax_.y);
    if(boxMin_.z > boxMax_.z) swap(boxMin_.z, boxMax_.z);*/

    //left
    if(ray.direction.x != 0) {
        ta[0] = (boxMin_.x - ray.origin.x) / normalize(ray.direction.x);  //normalize causes box to be as warped as sphere
        vec3 left = ray.origin + ta[0] * ray.direction;
        if((left.y <= boxMax_.y && left.y >= boxMin_.y && left.z >= boxMax_.z && left.z <= boxMin_.z))
        {
            test = true;
        } 
        else {
            test = false;
            ta[0] = -1;
        }

    //right
        ta[1] = (boxMax_.x - ray.origin.x) / normalize(ray.direction.x);
        vec3 right = ray.origin + ta[1] * ray.direction;
        if((right.y <= boxMax_.y && right.y >= boxMin_.y && right.z >= boxMax_.z && right.z <= boxMin_.z))
        {
            test = true;
        } else {
            test = false;
            ta[1] = -1;
        }
    }

    //top
    if(ray.direction.y != 0) {
        ta[2] = (boxMax_.y - ray.origin.y) / normalize(ray.direction.y);
        vec3 top = ray.origin + ta[2] * ray.direction;
        if((top.x <= boxMax_.x && top.x >= boxMin_.x && top.z >= boxMax_.z && top.z <= boxMin_.z )) {
            test = true;
        } 
        else {
            test = false;
            ta[2] = -1;
        }

    //bottom
        ta[3] = (boxMin_.y - ray.origin.y) / normalize(ray.direction.y);
        vec3 bottom = ray.origin + ta[3] * ray.direction;
        if((bottom.x <= boxMax_.x && bottom.x >= boxMin_.x && bottom.z >= boxMax_.z && bottom.z <= boxMin_.z )) 
        {
            test = true;
        } 
        else 
        {
            test = false;
            ta[3] = -1;
        }
    }

    //front
    if(ray.direction.z !=0) {
        ta[4] = (boxMin_.z - ray.origin.z) / normalize(ray.direction.z);
        vec3 front = ray.origin + ta[4] * ray.direction;
        if((front.x <= boxMax_.x && front.x >= boxMin_.x && front.y <= boxMax_.y && front.y >= boxMin_.y)) 
        {
            test = true;
        } 
        else 
        {
            test = false;
            ta[4] = -1;
        }

    //back
        ta[5] = (boxMin_.z - ray.origin.z) / normalize(ray.direction.z);
        vec3 back = ray.origin + ta[5] * ray.direction;
        if((back.x <= boxMax_.x && back.x >= boxMin_.x && back.y <= boxMax_.y && back.y >= boxMin_.y)) 
        {
            test = true;
        } 
        else 
        {
            test = false;
            ta[5] = -1; 
        }
    }

    float final_t;
    for(int i = 0; i<6; ++i){
        if(ta[i] > 0 && (ta[i] < final_t )) { //|| final_t == -1
            final_t = ta[i];
            test = true;
        }
    }


    Hit result = {test, t, ray.origin, ray.direction}; //same result w/ t and final_t
    return result;

    /*float r = (boxMin_.x - ray.origin.x)/ray.direction.x;
    float y = ray.origin.y + r*ray.direction.y;
    float z = ray.origin.z + r*ray.direction.z;
    if(boxMin_.y <= y && y <= boxMax_.y){
        if(boxMin_.z <= z && z <= boxMax_.z){
            test = true;
        }
    }

    if(t < r){
        test = false;
    }*/



}

    /*float r = (boxMin_.x - ray.origin.x)/ray.direction.x;
    bool test = false;

    float y = ray.origin.y + r*ray.direction.y;
    float z = ray.origin.z + r*ray.direction.z;
    if(boxMin_.y <= y && y <= boxMax_.y){
        if(boxMin_.z <= z && z <= boxMax_.z){
            test = true;
        }
    }

    if(t < r){
        test = false;
    }

    Hit result{test, t, ray.origin, ray.direction};
    return result;
} */

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