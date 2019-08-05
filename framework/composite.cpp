#include "composite.hpp"

Composite::Composite():
    Shape::Shape(),
    shapes_{}
    {}

Composite::Composite(string const& name):
    Shape::Shape{name,{}},
    shapes_{}
    {}

void Composite::addShape(shared_ptr<Shape> newShape){
    shapes_.push_back(newShape);
}

ostream& Composite::print(ostream& os) const
{
    for(auto const& shape : shapes_)
    {
        shape -> print(os);
        os << "\n";
    }
    return os;
}

Hit Composite::intersect(Ray const& ray, float& t) 
{
    Hit closest;
    Hit current;
    for(auto const& s:shapes_) 
    {
        current = s -> intersect(ray, t);
        if(current.dist_ < closest.dist_)
        {
            closest = current;
        }
    }
    return closest;
}
