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