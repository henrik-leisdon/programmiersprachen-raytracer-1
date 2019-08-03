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
