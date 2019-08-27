#ifndef RAY_HPP
#define RAY_HPP
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>

struct Ray
{
    
    Ray(){
        origin = glm::vec3();
        direction = glm::vec3();
    }

    Ray(glm::vec3 orig, glm::vec3 dir) : origin{orig}, direction{dir} {}

friend Ray transformRay(glm::mat4 const& transform_inv, Ray const& ray){
        Ray newRay;
        glm::vec3 newOrigin{transform_inv* glm::vec4(ray.origin, 1)};
        glm::vec3 newDirection{transform_inv* glm::vec4(ray.direction, 0)};

        newRay.origin = glm::vec3(newOrigin);
        newRay.direction = glm::vec3(newDirection);
        return newRay;
    }

    glm::vec3 origin = {0.0f,0.0f,0.0f};
    glm::vec3 direction = {0.0f,0.0f,-1.0f};




};


#endif