#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <iostream>
#include <string>
#include "color.hpp"

using namespace std;

struct Material
{
    public:
        //standard constructor
        Material():
            name_(string("material")),
            ka_({0.0f,0.0f,0.0f}),
            kd_({0.0f,0.0f,0.0f}),
            ks_({0.0f,0.0f,0.0f}),
            m_(0.0f),
            ref_(0.0f),
            opacity_(0.0f)
            {}
        
        //custom constructor
        Material(string const& name, Color const& ka, Color const& kd, Color const& ks, float m, float ref, float opacity):
            name_(name),
            ka_(ka),
            kd_(kd),
            ks_(ks),
            m_(m),
            ref_(ref),
            opacity_(opacity)
            {} 

    friend ostream& operator << (ostream& os, Material const& mat) {
        os << "name: " << mat.name_ << "\n"
           << "ambient: " << mat.ka_ << "\n"
           << "diffuse: " << mat.kd_ << "\n"
           << "specular: " <<mat.ks_ << "\n"
           << "reflection: " << mat.ref_ << "\n"
           << "opacity: " << mat.opacity_ <<"\n";
    }

    //member variables
       
        string name_;
        Color ka_; /*ka, kd, ks are reflection coefficients*/ 
        Color kd_;
        Color ks_;
        float m_;
        float ref_;
        float opacity_;
                    /* Phong-exponent needed for the intensity of the specular reflection, 
                    specular = direct reflection of the light source*/



};

#endif // !MATERIAL_HPP
