// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

Renderer::Renderer(Scene const& scene, unsigned w, unsigned h, std::string const& file): 
    scene_(scene)
  ,  width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
{}

//loads scene and renders every pixel
void Renderer::raycast()
{
  Scene scene;
  //Camera cam;
  std::size_t const checker_pattern_size = 20;
  //read_sdf("/home/vanessaretz/Schreibtisch/raytracer/programmiersprachen-raytracer-1/framework/materials.sdf", scene);
  //read_sdf("/home/IN/seso4651/Documents/raytracer/programmiersprachen-raytracer-1/framework/materials.sdf", scene);
  
  read_sdf("/home/henrik/Google_Drive/Uni/git/buw_raytracer_new/programmiersprachen-raytracer-1/framework/materials.sdf", scene);
  int i = scene.shapes_.size() ;
  
  int a;
  float d = (width_/2.0f) / (tan(70/2.0f*M_PI/180));

  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x,y);

      vec3 origin{0.0f,0.0f, 0.0f};
      vec3 direction{x-width_/2.0f,y-height_/2.0f, -d};
      vec3 normalizedDirection{glm::normalize(direction)};
      Ray ray{origin, normalizedDirection};

      p.color = trace(ray, scene);
      write(p);
    }
  }
  ppm_.save(filename_);
}

//trace the ray through the scene
Color Renderer::trace(Ray const &ray, Scene const &scene) {
    float distance = 0.0f;
    float dist = 100000.0f;
    shared_ptr<Shape> nearestObject;
    Hit h;
    vec3 cut;
    vec3 normVecToLight;

    Color col;
    Color amb_mat;
    Color amb_lig;
    Color final_ambiance;
    Color spec;
    Color diffuse;
    Color lightCol;

    //get the nearest object
    for (auto i : scene.shapes_) {
        h = i->intersect(ray, distance);
                
        if (h.hit_ == true) {
            if (h.dist_ < dist || dist == 0) {
                dist = h.dist_; 
                cut = h.hitpoint_;
                
                nearestObject = i;
            }
        }
    }
    if (nearestObject != nullptr) {
        lightCol = ptLight(h, ray, scene, nearestObject);
        
        final_ambiance = getAmbientIllumination(h, scene, nearestObject);
        //diffuse = getDiffuseIllumination(h, normVecToLight, scene, nearestObject);
        //spec = getSpecularIllumination(h, ray, normVecToLight, scene, nearestObject);
        //cout << "name: " << nearestObject->getName() << "\n";
        Color final =  lightCol; //+final_ambiance
        return final;
    } else {
        return Color{1.0f, 1.0f, 1.0f};
    }
}

//get the ambient light in scene
Color Renderer::getAmbientIllumination(Hit const& hit, Scene const& scene, shared_ptr<Shape> const& nearestObject){

    Color ia;
    Color amb_mat = nearestObject->getMaterial()->ka_;
    for(auto i : scene.light_) {
        ia = i->getColorAmb();
    }
    Color final_ambiance = amb_mat*ia;
    return final_ambiance;
}

// get the pointlights in the scene to determine the diffuse illumination
Color Renderer::ptLight(Hit const &hit, Ray const &ray, Scene const& scene, shared_ptr<Shape> const& nearestObject) {

    shared_ptr<Light> light;
    vec3 normVecToLight;
    vec3 lightPos;
    vec3 vecToLight;
    Color diffuse;
    Color specular;

    for(auto i : scene.light_){
        light = i;
        lightPos = i->getPos();
        vecToLight = {lightPos.x-hit.hitpoint_.x, lightPos.y-hit.hitpoint_.y,lightPos.z-hit.hitpoint_.z};
        normVecToLight = glm::normalize(vecToLight);
        diffuse = getDiffuseIllumination(hit, normVecToLight, scene, nearestObject, light);
        specular = getSpecularIllumination(hit, ray, normVecToLight, scene, nearestObject, light);
    }
    return diffuse; //+specular
}


Color Renderer::getDiffuseIllumination(Hit const &hit , vec3 normVecToLight, Scene const& scene, shared_ptr<Shape> const& nearestObject, shared_ptr<Light> const& light) {

    Color diff_col;
    int bright;
    Color ip;
    Color kd;
    Color diffuse_color;
    float vec;

    //for every light get the diffuse color of the nearest object
    //multiply the color of the diffuse light and its brightness with the material color of the object
    for (auto i : scene.light_) {
        vec = dot(normalize(hit.direction_), normVecToLight);
        diff_col = i->getColor();
        bright = i->getBrightness();
        ip = diff_col*bright;
    }
    kd = nearestObject->getMaterial()->kd_;
    //cout << "name: " << nearestObject->getName() << "\n" ;
    //cout << "kd: " << kd << "vec: " << vec << "bright:  " << bright << "\n";
    diffuse_color = ip*kd*vec;
    //cout << "diff" << diffuse_color << "\n";
    return diffuse_color;
}

/*Color Renderer::calculateSpecular(Hit const& hit, shared_ptr<Shape> nearestObject, Scene const& scene, Ray const& ray, vec3 const& hitpoint)
{
  Color specColor{0,0,0};
  for(auto i : scene.light_) 
  {
    vec3 vecToLight = normalize(i->getPos()-hitpoint);
    bool specular = true;
    Hit lightObstacle;
    for(auto j : scene.shapes_)
    {
      float distance;
       lightObstacle = j->intersect(Ray{hitpoint,vecToLight}, distance);

       if(lightObstacle.hit_)
       {
         specular = false;
       }
       
    } 
    if(specular)
    {
      float m = nearestObject->getMaterial()->m_;
      vec3 v = normalize(scene.camera_->getPos()-hitpoint);
     // cout << "hit: " << hit.hitpoint_.x << " " <<  hit.hitpoint_.y << " " << hit.hitpoint_.z << "\n";
     // cout << "v coords: " << v.x << " " << v.y << " " << v.z << "\n";
      vec3 r = dot(hit.direction_,vecToLight)*2.0f*hit.direction_-vecToLight;
      float p = dot(v,r);
      if(p<0)
        p = -p;
      float cos = pow(p,m);
      Color ip = i->getColor()*i->getBrightness();
      Color ks = nearestObject->getMaterial()->ks_;
      Color specColor = ks*((m+2)/(2*M_PI))*cos*ip;
    }
  }
  return specColor;
  
}*/


Color Renderer::getSpecularIllumination(Hit const &hit, Ray const &ray, vec3 normVecToLight, Scene const &scene, shared_ptr<Shape> const& nearestObject, shared_ptr<Light> const& light) {
    Color is;
    Color ks;
    Color final_specular;
    vec3 refl_angle;
    vec3 ausf_angle;
    float m;

            refl_angle = 2.0f * glm::dot(normVecToLight, normalize(hit.direction_)) * normalize(hit.direction_);
            ausf_angle = glm::normalize(ray.direction);

                if (glm::dot(ausf_angle, refl_angle) > 0) {
                    for (auto i : scene.light_) {
                        is = i->getColor() * i->getBrightness();
                        }
                m = nearestObject->getMaterial()->m_;
                ks = nearestObject->getMaterial()->ks_;
                final_specular = ks * pow(glm::dot(refl_angle, ausf_angle), m) * is; //((m+2.0f)/(2.0f*M_PI))
                        
                return final_specular;
                }
}


void Renderer::write(Pixel const& p) {

  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_*p.y + p.x);
  if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0) {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
      << "pixel out of ppm_ : "
      << (int)p.x << "," << (int)p.y
      << std::endl;
  } else {
    color_buffer_[buf_pos] = p.color;
  }

  ppm_.write(p);
}
