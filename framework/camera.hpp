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
        Camera(string const& name, float fov, vec3 const& pos);
        ~Camera();

        string getName() const;
        vec3 getPos() const;
        float getAngle() const;
        float getDist() const;

    private:
        string name_;
        vec3 pos_;
        vec3 up_; //needed vectors for orthonormal base
        vec3 dir_;
        float angle_;
        float dist_; //to pixel canvas
};

#endif // !CAMERA_HPP

