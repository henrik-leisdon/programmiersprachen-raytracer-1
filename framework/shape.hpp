#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <string>
#include <hit.hpp>

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
        virtual Hit intersect (Ray const& ray, float& t) = 0;
        virtual ostream& print (ostream& os) const;

        virtual void translate(vec3 const& vec);
        virtual void scale(vec3 const& vec);
        virtual void rotate(float phi, vec3 const& vec);

            protected:
            string name_;
            shared_ptr<Material> material_;

            bool isTransformed_;
            mat4 world_transform_;
            mat4 inverse_world_transform_;
            mat4 translate_;
            mat4 scale_;
            mat4 rotate_;

};

ostream& operator <<(ostream& os, Shape const& s);
#endif