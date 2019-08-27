#ifndef RAY_HPP
#define RAY_HPP
#include <glm/vec3.hpp>

struct Ray
{
    
    Ray(){
        origin = glm::vec3();
        direction = glm::vec3();
    }

    Ray(glm::vec3 orig, glm::vec3 dir) : origin{orig}, direction{dir} {}

    glm::vec3 origin = {0.0f,0.0f,0.0f};
    glm::vec3 direction = {0.0f,0.0f,-1.0f};




};


#endif