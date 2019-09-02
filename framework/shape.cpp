#include "shape.hpp"

using namespace std;
using namespace glm;

#define TRANSLATION_MAT mat4x4{vec4{1.0f, 0.0f, 0.0f, 0.0f}, vec4{0.0f, 1.0f, 0.0f, 0.0f}, vec4{0.0f, 0.0f, 1.0f, 0.0f}, vec4{trans.x, trans.y, trans.z, 1.0f}};
#define SCALING_MAT     mat4x4{vec4{scale.x, 0.0f, 0.0f, 0.0f}, vec4{0.0f, scale.y, 0.0f, 0.0f}, vec4{0.0f, 0.0f, scale.z, 0.0f}, vec4{0.0f, 0.0f, 0.0f, 1.0f}};
#define X_ROTATION_MAT  mat4x4{vec4{1.0f, 0.0f, 0.0f, 0.0f}, vec4{0.0f, cos(rad), sin(rad), 0.0f}, vec4{0.0f, -sin(rad), cos(rad), 0.0f}, vec4{0.0f, 0.0f, 0.0f, 1.0f}};
#define Y_ROTATION_MAT  mat4x4{vec4{cos(rad), 0.0f, -sin(rad), 0.0f}, vec4{0.0f, 1.0f, 0.0f, 0.0f}, vec4{sin(rad), 0.0f, cos(rad), 0.0f}, vec4{0.0f, 0.0f, 0.0f, 1.0f}};
#define Z_ROTATION_MAT  mat4x4{vec4{cos(rad), sin(rad), 0.0f, 0.0f}, vec4{-sin(rad), cos(rad), 0.0f, 0.0f}, vec4{0.0f, 0.0f, 1.0f, 0.0f}, vec4{0.0f, 0.0f, 0.0f, 1.0f}};


Shape::Shape():
    name_{"empty Object"},
    material_{},
    world_transform_{mat4(1.0f)},
    inverse_world_transform_{mat4(1.0f)},
    translate_{mat4(1.0f)},
    scale_{mat4(1.0f)},
    rotate_{mat4(1.0f)}
    {};

Shape::Shape(string name, shared_ptr<Material> const& material):
    name_{name},
    material_{material},
    world_transform_{mat4(1.0f)},
    inverse_world_transform_{mat4(1.0f)},
    translate_{mat4(1.0f)},
    scale_{mat4(1.0f)},
    rotate_{mat4(1.0f)}
    {};

Shape::~Shape() {};

//getter

string Shape::getName() const
{
    return name_;
}

shared_ptr<Material> Shape::getMaterial() const
{
    return material_;
}

ostream& Shape::print(ostream& os) const
{
    os << "name: " << name_ << "\n"
       << "material: " << *material_ << "\n";
    return os;
}

void Shape::translate(vec3 const &trans) {
    translate_=TRANSLATION_MAT;
    world_transform_=world_transform_*translate_*rotate_*scale_;
    inverse_world_transform_=inverse(world_transform_);

    isTransformed_=true;
}

void Shape::scale(vec3 const &scale) {
    scale_=SCALING_MAT;
    world_transform_=world_transform_*translate_*rotate_*scale_;
    inverse_world_transform_=inverse(world_transform_);

    isTransformed_=true;
}

void Shape::rotateX(float phi) {
    float rad = phi*(M_PI/180.0f);
    rotate_=X_ROTATION_MAT;
    world_transform_=world_transform_*translate_*rotate_*scale_;
    inverse_world_transform_=inverse(world_transform_);

    isTransformed_=true;
}

void Shape::rotateY(float phi) {
    float rad = phi*(M_PI/180.0f);
    rotate_=Y_ROTATION_MAT;
    world_transform_=world_transform_*translate_*rotate_*scale_;
    inverse_world_transform_=inverse(world_transform_);

    isTransformed_=true;
}

void Shape::rotateZ(float phi) {
    float rad = phi*(M_PI/180.0f);
    rotate_=Z_ROTATION_MAT;
    world_transform_=world_transform_*translate_*rotate_*scale_;
    inverse_world_transform_=inverse(world_transform_);

    isTransformed_=true;
}

ostream& operator <<(ostream& os, Shape const& s)
{
    return s.print(os);
}