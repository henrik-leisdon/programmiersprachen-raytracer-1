#include "sphere.hpp"
#include <math.h>
#include <glm/gtx/intersect.hpp>

using namespace std;
using namespace glm;

Sphere::Sphere():
    Shape(),
    center_({0.0,0.0,0.0}),
    radius_{0.0}
    {};

Sphere::Sphere(string name, vec3 const& center, double radius, shared_ptr<Material> material):
    Shape{name, material},
    center_{center},
    radius_{radius}
    {};

Sphere::~Sphere() {}

vec3 Sphere::getCenter() const
{
    return center_;
}

double Sphere::getRadius() const
{
    return radius_;
}

double Sphere::area() const {
    double area = 4*M_PI*pow(getRadius(),2);
    return area;
}

double Sphere::volume() const {
    double volume = (4.0/3.0)*M_PI*pow(getRadius(),3);
    return volume;
}

Hit Sphere::intersect(Ray const &firstRay, float &t) {
    bool intersect;
    Hit result;

    Ray ray={firstRay.origin, firstRay.direction};

    if(isTransformed_){
        ray = transformRay(inverse_world_transform_, ray);
    } else {
        ray = firstRay;
    }

    vec3 normDir = normalize(ray.direction);
    intersect = intersectRaySphere(ray.origin, normalize(ray.direction), center_ ,radius_*radius_, t);
    if(intersect) {
        vec3 hitpoint = vec3{ray.origin+normDir*t};
        vec3 normToShape = vec3{hitpoint-center_};
        //cout << getName() << " normal in intersect: " << normToShape.x << " " << normToShape.y << " " << normToShape.z <<  "\n";
        result.hit_ = true;
        result.hitnormal_ = normToShape;
        result.hitpoint_ = hitpoint;
        result.dist_ = t;
        result.direction_ = normDir;
        if(isTransformed_) {
            vec4 transformNormal = glm::normalize(transpose(inverse_world_transform_)*vec4{result.hitnormal_,0});
            result.hitnormal_ = vec3({transformNormal.x, transformNormal.y, transformNormal.z});

            vec4 transformHit = world_transform_ * vec4{result.hitpoint_, 1};
            result.hitpoint_ = vec3{transformHit.x, transformHit.y, transformHit.z};
        }
        return result;
    }
    else {
        Hit result = Hit();
        return result;
    }
    
        
}

ostream& Sphere::print(ostream &os) const {
    Shape::print(os);
    os
    << "Position : " << center_.x << ", " << center_.y << ", " << center_.z << "\n"
    << "Radius : " << radius_ << "\n";

    return os;
}

ostream& operator << (ostream& os, const Sphere s) {
    return s.print(os);
}
