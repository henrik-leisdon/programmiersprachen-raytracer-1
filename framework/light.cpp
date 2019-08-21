#include "light.hpp"

using namespace std;
using namespace glm;

Light::Light():
    name_("default light"),
    pos_({0.0,0.0,0.0}),
    color_({0.0,0.0,0.0}),
    ambColor_({0.0,0.0,0.0}),
    brightness_(10)
    {};

Light::Light(string name, vec3 const& pos, Color const& color, Color const& ambColor, int brightness): //Color const& ambColor
    name_{name},
    pos_{pos},
    color_{color},
    ambColor_{ambColor},
    brightness_{brightness}
    {};


string Light::getName()
{
    return name_;
}

vec3 Light::getPos(){
    return pos_;
}

Color Light::getColor(){
    return color_;
}

Color Light::getColorAmb() {
    return ambColor_;
}

int Light::getBrightness(){
    return brightness_;
}  