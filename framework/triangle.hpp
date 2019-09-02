#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <glm/vec3.hpp>

#include "ray.hpp"
#include "shape.hpp"
#include "hit.hpp"

using namespace std;
using namespace glm;

class Triangle : public Shape
{
    public:
        Triangle();
        Triangle(string name, glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c, std::shared_ptr<Material> const& mat);
        ~Triangle();

        vec3 getA() const;
        vec3 getB() const;
        vec3 getC() const;

        Hit intersect(Ray const& ray, float& t) override;

        ostream& print(ostream& os) const override;

    private:
        vec3 a_;
        vec3 b_;
        vec3 c_;

};

ostream& operator << (ostream& os, const Triangle& t);

#endif