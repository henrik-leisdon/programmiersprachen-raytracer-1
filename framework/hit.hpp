#ifndef HIT_HPP
#define HIT_HPP

#include <iostream>
#include <glm/vec3.hpp>
#include <memory>
#include "color.hpp"

using namespace std;
using namespace glm;

struct Hit {
    bool hit_ = false;
    double dist_ = 0.0;
    vec3 hitpoint_;
    vec3 direction_;
};

#endif