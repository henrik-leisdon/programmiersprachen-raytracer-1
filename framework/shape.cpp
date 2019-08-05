#include "shape.hpp"

using namespace std;

Shape::Shape():
    name_{"empty Object"}
    {};

Shape::Shape(string name, shared_ptr<Material> const& material):
    name_{name},
    material_{material}
    {};

Shape::~Shape() {};

//getter

string Shape::getName() const
{
    return name_;
}

shared_ptr<Material> Shape::getMaterial() const
{
    return material_;
}

ostream& Shape::print(ostream& os) const
{
    os << "name: " << name_ << "\n"
       << "material: " << *material_ << "\n";
    return os;
}

ostream& operator <<(ostream& os, Shape const& s)
{
    return s.print(os);
}