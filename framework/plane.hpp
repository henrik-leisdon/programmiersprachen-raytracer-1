#ifndef PLANE_HPP
#define PLANE_HPP

#include <glm/vec3.hpp>

using namespace glm;

struct Plane {
    vec3 origin;
    vec3 normal;
};

#endif