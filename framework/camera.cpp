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
        dist_ = tan((90 - angle_ / 2) * M_PI /180);
        //dist_ = 0.5/tan(radius);

    };

float Camera::getAngle() {
    return angle_;
}

vec3 Camera::getPos() {
    return pos_;
}
Ray Camera::camRay(int x, int y, float width, float height) const {
   vec3 rayOrigin{0.0,0.0,0.0};
   vec3 rayDirection = glm::normalize(vec3{(x-width/2.0f), (y-height/2.0f), -(width/2)/tan(angle_/2 * M_PI / 180)});
    //float d = (width/2.0f) / (tan(angle_/2.0f * M_PI/180.0f));
    //glm::vec3 direction{x - (0.5f * width), y - (0.5f * height), -d};
    //direction = normalize(direction);

    Ray camRay {rayOrigin, rayDirection};

    return camRay;
}
