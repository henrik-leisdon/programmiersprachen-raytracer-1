#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include "shape.hpp"
#include "hit.hpp"
#include "ray.hpp"
#include <vector>

using namespace std;

class Composite: public Shape
{
    public:
        Composite();
        Composite(string const& name);
        ~Composite();

        void addShape(shared_ptr<Shape> newShape);
        Hit intersect(Ray const& ray, float& t) override;

        ostream& print(ostream& os) const override;

        vector<shared_ptr<Shape>> shapes_;
};

#endif // !COMPOSITE_HPP
