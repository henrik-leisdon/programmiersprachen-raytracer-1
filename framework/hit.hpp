#ifndef HIT_HPP
#define HIT_HPP

#include <iostream>
#include <glm/vec3.hpp>
#include <memory>
//#include "shape.hpp"
#include "color.hpp"

using namespace std;
using namespace glm;

//class Shape;

struct Hit {

    Hit():
        hit_(false),
        dist_(0.0f),
        hitpoint_(0.0f,0.0f,0.0f),
        direction_(0.0f,0.0f,0.0f),
        hitnormal_(0.0f,0.0f,0.0f)
        {}

    Hit(bool const& hit, float const& dist, vec3 const& hitpoint, vec3 const& direction, vec3 const& hitnormal):
        hit_(hit),
        dist_(dist),
        hitpoint_(hitpoint),
        direction_(direction),
        hitnormal_(hitnormal)
        {}

    bool hit_;
    float dist_;
    vec3 hitpoint_;
    vec3 direction_;
    vec3 hitnormal_; //normale n zur Ebene
};

#endif
