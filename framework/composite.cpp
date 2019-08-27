#include "composite.hpp"

Composite::Composite():
    Shape::Shape{},
    composition_{}
    {}

Composite::Composite(std::string const& name):
    Shape::Shape{name,{}},
    composition_{}
    {}

void Composite::addShape(std::shared_ptr<Shape> const& newShape)
{
    composition_.push_back(newShape);
}



