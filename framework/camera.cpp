#include <glm-0.9.5.3/glm/geometric.hpp>
#include "camera.hpp"

using namespace std;
using namespace glm;


Camera::Camera():
    name_("default camera"),
    dist_(0.0f),
    angle_(0.0f),
    pos_({0.0f,0.0f,0.0f}),
    dir_({0.0f,0.0f,-1.0f}),
    up_({0.0f,1.0f,0.0f})
    {};

Camera::Camera(string const& name, float fov, vec3 const& pos):
    name_(name),
    dist_(0.0f),
    angle_(fov),
    pos_(pos),
    dir_({0.0f,0.0f,-1.0f}),
    up_({0.0f,1.0f,0.0f})
    {
        float radius = (angle_ * M_PI/360);
        dist_ = tan(90 - angle_ / 2); //* M_PI /180

    };

float Camera::getAngle() {
    return angle_;
}

vec3 Camera::getPos() {
    return pos_;
}

vec3 Camera::getUp() {
    return up_;
}

vec3 Camera::getDir() {
    return dir_;
}

