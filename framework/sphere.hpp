#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <glm/vec3.hpp>

#include "ray.hpp"
#include "shape.hpp"
#include "hit.hpp"

using namespace std;
using namespace glm;

class Sphere : public Shape
{
    public:
        Sphere();
        Sphere(string name, vec3 const& center, double radius, shared_ptr<Material> material);

        vec3 getCenter() const;
        double getRadius() const;

        double area() const override;
        double volume() const override;
        Hit intersect(Ray const& ray, float& t) override;

        ostream& print (ostream& os) const override;
        ostream& operator << (ostream& os, const Sphere& s);

    private:
        vec3 center_;
        double radius_;
};

#endif