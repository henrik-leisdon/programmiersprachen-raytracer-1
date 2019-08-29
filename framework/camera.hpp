#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "ray.hpp"

using namespace std;
using namespace glm;

class Camera 
{
    public:
        Camera();
        Camera(string const& name, float fov, vec3 const& pos, vec3 const& dir, vec3 const& upVec);

        void translate(vec3 const& vec3);
        void rotateX(float phi);
        void rotateY(float phi);
        void rotateZ(float phi);

        Ray calc_cam_ray(float x, float y, float height, float width, float d) const;

        string getName();
        float getAngle();
        vec3 getPos();
        vec3 getUp();
        vec3 getDir();

    protected:
        string name_;
        vec3 pos_;
        vec3 up_; //vector to determine where is the top of the camera
        vec3 dir_; // direction the vector is facing
        float angle_; //field of view (bad naming so far)
        float dist_; //to pixel canvas

        mat4 transform_;
        mat4 inverse_transform_;
        mat4 cameraTranslation_;
        mat4 cameraRotation_;
        bool isTransformed_;
};

#endif // !CAMERA_HPP

