#include "triangle.hpp"
#include <math.h>
#include <glm/gtx/intersect.hpp>

using namespace std;
using namespace glm;

Triangle::Triangle():
    Shape(),
    a_({0.0,0.0,0.0}),
    b_({0.0,0.0,0.0}),
    c_({0.0,0.0,0.0})
    {};

Triangle::Triangle(string name, vec3 const& a, vec3 const& b, vec3 const& c, std::shared_ptr<Material> const& material):
    Shape{name, material},
    a_{a},
    b_{b},
    c_{c}
    {};

Triangle::~Triangle()
 {};

vec3 Triangle::getA() const
{
    return a_;
}

vec3 Triangle::getB() const
{
    return b_;
}

vec3 Triangle::getC() const
{
    return c_;
}

Hit Triangle::intersect(Ray const& firstRay, float& t) {
    bool intersect;
    Hit result;

    Ray ray={firstRay.origin, firstRay.direction};

    if(isTransformed_) {
        ray = transformRay(inverse_world_transform_, ray);
    } else {
        ray = firstRay;
    }

    const float eps = 0.000000001;
    vec3 a_b = b_-a_;
    vec3 a_c = c_-a_;
    vec3 h = glm::cross(ray.direction, a_c);
    float a = glm::dot(a_b, h);

    if(a>eps && a<eps) {
        result.hit_=false;
        intersect = false;
    }

    float f = 1/a;
    vec3 s = ray.origin-a_;
    float u = f*(dot(s,h));
    if(u<0.0 || u>1.0) {
        result.hit_=false;
    }

    vec3 q = cross(s,a_b);
    float v = f*dot(ray.direction, q);
    if(t> eps) {
        result.hitpoint_ = ray.origin+ray.direction*t;
        result.hitnormal_= normalize(cross(a_b,a_c));

        vec4 transformed_cut = world_transform_ * vec4(result.hitpoint_,1);
        vec4 transformed_normal = normalize(transpose(inverse_world_transform_)*vec4{result.hitnormal_,0});

        result.hitpoint_ = vec3{transformed_cut.x,transformed_cut.y,transformed_cut.z};
        result.hitnormal_ = vec3{transformed_normal.x,transformed_normal.y,transformed_normal.z};

        result.dist_ = length(result.hitpoint_-firstRay.origin);
        result.hit_ = true;
        result.direction_ = ray.direction;
        return result;
    }
    else {
        result = Hit();
        return result;
    }
}

ostream& Triangle::print(ostream &os) const {

}


ostream& operator << (ostream& os, const Triangle t) {

}