#ifndef HIT_HPP
#define HIT_HPP

#include <iostream>
#include <glm/vec3.hpp>
#include <memory>
#include "color.hpp"

using namespace std;
using namespace glm;

struct Hit {

    bool hit_;
    float dist_;
    vec3 hitpoint_;
    vec3 direction_;
    vec3 hitnormal_; //normale n zur Ebene
};

#endif