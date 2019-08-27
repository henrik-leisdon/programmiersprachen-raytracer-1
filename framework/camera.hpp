#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>
#include <cmath>
#include <glm/vec3.hpp>

#include "ray.hpp"

using namespace std;
using namespace glm;

class Camera 
{
    public:
        Camera();
        Camera(string const& name, float fov, vec3 const& pos, vec3 const& dir, vec3 const& upVec);


        float getAngle();
        vec3 getPos();
        vec3 getUp();
        vec3 getDir();

    private:
        string name_;
        vec3 pos_;
        vec3 up_; //vector to determine where is the top of the camera
        vec3 dir_; // direction the vector is facing
        float angle_; //field of view (bad naming so far)
        float dist_; //to pixel canvas
};

#endif // !CAMERA_HPP

