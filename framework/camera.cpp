#include <glm-0.9.5.3/glm/geometric.hpp>
#include "camera.hpp"

using namespace std;
using namespace glm;


#define TRANSLATION_MAT mat4x4{vec4{1.0f, 0.0f, 0.0f, 0.0f}, vec4{0.0f, 1.0f, 0.0f, 0.0f}, vec4{0.0f, 0.0f, 1.0f, 0.0f}, vec4{trans.x, trans.y, trans.z, 1.0f}};
#define X_ROTATION_MAT  mat4x4{vec4{1.0f, 0.0f, 0.0f, 0.0f}, vec4{0.0f, cos(rad), sin(rad), 0.0f}, vec4{0.0f, -sin(rad), cos(rad), 0.0f}, vec4{0.0f, 0.0f, 0.0f, 1.0f}};
#define Y_ROTATION_MAT  mat4x4{vec4{cos(rad), 0.0f, -sin(rad), 0.0f}, vec4{0.0f, 1.0f, 0.0f, 0.0f}, vec4{sin(rad), 0.0f, cos(rad), 0.0f}, vec4{0.0f, 0.0f, 0.0f, 1.0f}};
#define Z_ROTATION_MAT  mat4x4{vec4{cos(rad), sin(rad), 0.0f, 0.0f}, vec4{-sin(rad), cos(rad), 0.0f, 0.0f}, vec4{0.0f, 0.0f, 1.0f, 0.0f}, vec4{0.0f, 0.0f, 0.0f, 1.0f}};

Camera::Camera():
    name_("default camera"),
    dist_(0.0f),
    angle_(0.0f),
    pos_({0.0f,0.0f,0.0f}),
    dir_({0.0f,0.0f,-1.0f}),
    up_({0.0f,1.0f,0.0f}),
    transform_{mat4(1.0)},
    inverse_transform_{glm::inverse(transform_)},
    isTransformed_{false}
    {};

Camera::Camera(string const& name, float fov, vec3 const& pos, vec3 const& dir, vec3 const& upVec):
    name_(name),
    dist_(0.0f),
    angle_(fov),
    pos_(pos),
    dir_(dir),
    up_(upVec),
    transform_{mat4(1.0)},
    inverse_transform_{glm::inverse(transform_)},
    isTransformed_{false}
    {
        float radius = (angle_ * M_PI/360);
        dist_ = tan(90 - angle_ / 2); //* M_PI /180

    };

void Camera::translate(vec3 const &trans) {
    cameraTranslation_=TRANSLATION_MAT;
    transform_=cameraTranslation_*transform_;
    inverse_transform_=glm::inverse(transform_);
    isTransformed_ = true;
}

void Camera::rotateX(float phi) {
    float rad = phi*(M_PI/180.0f);
    cameraRotation_=X_ROTATION_MAT;
    transform_=cameraRotation_*transform_;
    inverse_transform_=glm::inverse(transform_);
    isTransformed_ = true;
}

void Camera::rotateY(float phi) {
    float rad = phi*(M_PI/180.0f);
    cameraRotation_=Y_ROTATION_MAT;
    transform_=cameraRotation_*transform_;
    inverse_transform_=glm::inverse(transform_);
    isTransformed_ = true;
}

void Camera::rotateZ(float phi) {
    float rad = phi*(M_PI/180.0f);
    cameraRotation_=Z_ROTATION_MAT;
    transform_=cameraRotation_*transform_;
    inverse_transform_=glm::inverse(transform_);
    isTransformed_ = true;
}

Ray Camera::calc_cam_ray(float x, float y, float height, float width, float d) const {
    //x-width_/2.0f,y-height_/2.0f, -d
    vec3 direction{x-width/2.0f,y-height/2.0f, -d};
    Ray cameraRay = {pos_, glm::normalize(direction)};

    if(isTransformed_){
        Ray ray = transformRay(inverse_transform_, cameraRay);
        return ray;
    } else {
        return cameraRay;
    }
}

string Camera::getName() {
    return name_;
}

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

