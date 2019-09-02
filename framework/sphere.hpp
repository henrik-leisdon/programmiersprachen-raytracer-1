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
        Sphere(string name, vec3 const& center, double radius, std::shared_ptr<Material> material);
        ~Sphere();

        vec3 getCenter() const;
        double getRadius() const;

        double area() const;
        double volume() const;
        Hit intersect(Ray const& ray, float& t) override;

        ostream& print (ostream& os) const override;

    private:
        vec3 center_;
        double radius_;

};

ostream& operator << (ostream& os, const Sphere& s);


#endif