#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <iostream>
#include <string>
#include "color.hpp"
#include <glm/vec3.hpp>

#include "ray.hpp"

using namespace std;
using namespace glm;

class Light
{
    public:
        Light();
       Light(string name, vec3 const& pos, Color const& color, Color const& ambColor, int brightness); //Color const& ambColor,

        string getName();
        vec3 getPos();
        Color getColor();
        Color getColorAmb();
        int getBrightness();

    private:
        string name_;
        vec3 pos_;
        Color color_;
        Color ambColor_;
        int brightness_;


};


#endif // !LIGHT_HPP