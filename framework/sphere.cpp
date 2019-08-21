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

Sphere::~Sphere()
 {
     
 }

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

Hit Sphere::intersect(Ray const &ray, float &t) {
    bool test;
    test = intersectRaySphere(ray.origin, normalize(ray.direction), center_ ,radius_*radius_, t);
    vec3 hitpoint = vec3{ray.direction*t};
    Hit result{test, t, hitpoint, ray.direction};

    return result;
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
