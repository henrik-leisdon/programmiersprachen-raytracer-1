#ifndef HIT_HPP
#define HIT_HPP

#include <iostream>
#include <glm/vec3.hpp>
#include <memory>
#include "color.hpp"

using namespace std;
using namespace glm;

#define INFINITY (__builtin_inff ())

//class Shape;

struct Hit {

    /*Hit():
        hit_{false},
        dist_{INFINITY},
        direction_{},
        hitpoint_{} {}

    Hit(bool hit, double dist, vec3
    const& direction, vec3 const& hitpoint):
        hit_{hit},
        dist_{dist},
        direction_{direction},
        hitpoint_{hitpoint} {}
    */

    bool hit_;
    double dist_;
    vec3 hitpoint_;
    vec3 direction_;
};

#endif