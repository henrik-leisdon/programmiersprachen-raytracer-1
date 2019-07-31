#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <memory>
#include <glm/vec3.hpp>
#include <math.h>
#include <string>

#include "color.hpp"
#include "ray.hpp"
#include "material.hpp"

using namespace std;


class Shape
{
    public:
        Shape();
        Shape(string name, shared_ptr<Material> const& material);
        ~Shape();

        string getName() const;
        shared_ptr<Material> getMaterial() const;

        virtual double area() const = 0;
        virtual double volume() const = 0;
        virtual bool intersect (Ray const& ray, float& t) = 0;
        virtual ostream& print (ostream& os) const;

        private:
            string name_;
            shared_ptr<Material> material_;

};

ostream& operator <<(ostream& os, Shape const& s);
#endif