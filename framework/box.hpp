#ifndef BOX_HPP
#define BOX_HPP

#include <glm/vec3.hpp>
#include <memory>
#include <hit.hpp>
#include "shape.hpp"
#include "ray.hpp"

using namespace std;
using namespace glm;

class Box : public Shape
{
    public:
        Box();
        Box(string name, vec3 const& boxMin, vec3 const& boxMax, shared_ptr <Material> material);
        ~Box();

        vec3 getBoxMin();
        vec3 getBoxMax();

        double area() const override;
        double volume() const override;
        Hit intersect(Ray const& ray, float& t) override;

        ostream& print(ostream& os) const override;
        ostream& operator << (ostream& os, const Box& b);

    private:
        vec3 boxMin_;
        vec3 boxMax_;
};

#endif // !BOX_HPP
