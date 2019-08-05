#include "light.hpp"

using namespace std;
using namespace glm;

Light::Light():
    name_("default light"),
    pos_({0.0,0.0,0.0}),
    color_({0.0,0.0,0.0}),
    brightness_(10)
    {};

Light::Light(string name, vec3 const& pos, Color const& color, int brightness):
    name_{name},
    pos_{pos},
    color_{color},
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

int Light::getBrightness(){
    return brightness_;
}  