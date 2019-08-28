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
  shared_ptr<Camera> cam = scene.camera_;
  int a;
  float d = (width_/2.0f) / (tan(cam->getAngle()/2.0f*M_PI/180));

  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x,y);
      int step = 2;
      //cout << "x: " << x << " y " << y <<endl;
      vec3 origin = cam->getPos();
      vec3 direction{x-width_/2.0f,y-height_/2.0f, -d};
      vec3 normalizedDirection{glm::normalize(direction)};
      Ray ray{origin, normalizedDirection};
      p.color = trace(ray, scene, step);

      write(p);
    }
  }
  ppm_.save(filename_);
}

//trace the ray through the scene
Color Renderer::trace(Ray const &ray, Scene const &scene, int step) {
    float distance = 0.0f;
    float dist = 1000000.0f;
    shared_ptr<Shape> nearestObject;
    Hit h;
    Hit hit;
 
    
    for (auto i : scene.shapes_) {
        h = i->intersect(ray, distance);    
              
        if (h.hit_ == true) {
            if (h.dist_ < dist) {
                dist = h.dist_;
                hit.dist_ = h.dist_; 
                hit.hitnormal_ = h.hitnormal_;
                nearestObject = i;
                hit.hitpoint_ = h.hitpoint_;
                hit.direction_ = h.direction_;
                //cout << "dir h: " << h.direction_.x << " " << h.direction_.y << " " << h.direction_.z 
                //<< " ray.dir: " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z <<endl;
                hit.hit_ = h.hit_;
                
                
            }
        }
    }
    if (nearestObject != nullptr) {
      Color final;
      if(nearestObject->getMaterial()->ref_>0){

      float ref_coefficient  = nearestObject->getMaterial()->ref_;
      float negRef = 1.0f-ref_coefficient;
        final = calculateReflection(hit, ray, scene, nearestObject, step-1)*ref_coefficient+(getNormalColor(hit,ray, scene, nearestObject) *negRef);
      }else {
        Color illumination = getNormalColor(hit, ray, scene, nearestObject);
        final =  illumination;  
      }
      
        
        return final;
    } else {
        return Color{0.1f, 0.1f, 0.1f};
    }
}

Color Renderer::getNormalColor(Hit const& hit, Ray const& ray, Scene const& scene, shared_ptr<Shape> const& nearestObject){
  Color lightCol = ptLight(hit, ray, scene, nearestObject);
  Color ambiance = getAmbientIllumination(hit, scene, nearestObject);
  return lightCol+ambiance;
        
}

//get the ambient light in scene
Color Renderer::getAmbientIllumination(Hit const& hit, Scene const& scene, shared_ptr<Shape> const& nearestObject){

    Ambient ambient = scene.ambient_;
    Color amb = ambient.amb_; 
    Color final_ambiance = amb*nearestObject->getMaterial()->ka_; //ambiente mal ka_;
    return final_ambiance;
}


// get the pointlights in the scene to determine the diffuse illumination
Color Renderer::ptLight(Hit const &hit, Ray const &ray, Scene const& scene, shared_ptr<Shape> const& nearestObject) {


    Color diffuse = Color{0.0,0.0,0.0};
    Color specular= Color{0.0,0.0,0.0};
    for(auto i : scene.light_){
        vec3 vecToLight = {i->getPos()-hit.hitpoint_};
        float distance;
        Hit h;
        Ray rayToLight = {hit.hitpoint_+0.01f, normalize(vecToLight)};
        
        float lightDistance = glm::distance(i->getPos(),hit.hitpoint_);
        bool shadow = false;
    
        for (auto i : scene.shapes_) {
          h = i->intersect(rayToLight, distance);          
          if (h.hit_ == true && nearestObject!=i && h.dist_>0.0f) {
              if (h.dist_ < lightDistance) {
                 shadow = true;
              }
          }
        }
        if(shadow == false)
        {
          diffuse += getDiffuseIllumination(hit, scene, nearestObject, i);
          specular += getSpecularIllumination(hit, ray, scene, nearestObject, i);
       
        }
        

        // cout << "spec: " << specular << endl;
    }
        
    
    return diffuse + specular;
}

//get diffuse color of the nearest object
Color Renderer::getDiffuseIllumination(Hit const &hit ,Scene const& scene, shared_ptr<Shape> const& nearestObject, shared_ptr<Light> const& light) {
    vec3 vecToLight = {light->getPos()-hit.hitpoint_};
    vec3 normVecToLight = glm::normalize(vecToLight);
    
    Color ip = light->getColor()*light->getBrightness();
    Color kd = nearestObject->getMaterial()->kd_;
    float vec = dot(normalize(hit.hitnormal_), normVecToLight);
    
    return ip*kd*vec;
}

Color Renderer::getSpecularIllumination(Hit const &hit, Ray const &ray, Scene const &scene, shared_ptr<Shape> const& nearestObject, shared_ptr<Light> const& light) {
      
    vec3 vecToLight = {light->getPos()-hit.hitpoint_};
    vec3 normVecToLight = glm::normalize(vecToLight);
    
    vec3 reflection = normalize(glm::reflect((-vecToLight),hit.hitnormal_));
    float factor = glm::max(0.0f, dot(reflection, normalize(-(ray.direction))));
    
    Color ip = light->getColor()*light->getBrightness();
    float m = nearestObject->getMaterial()->m_;
    Color ks = nearestObject->getMaterial()->ks_;
    
    Color final_specular = ks * pow(factor, m); 
    
    return final_specular;
  
}

Color Renderer::calculateReflection(Hit const& hit, Ray const& ray, Scene const& scene, shared_ptr<Shape> const& nearesObject, int steps){

  float distance;
  float dist = 1000000.0f;
  shared_ptr<Shape> newNearestObject;
  vec3 reflectionDir = normalize(glm::reflect((ray.direction),hit.hitnormal_));
  Ray reflectionRay = Ray(hit.hitpoint_, normalize(reflectionDir));
  Hit h;
  Hit nearestHit;
  for (auto i : scene.shapes_) {
    h = i->intersect(reflectionRay, distance);            
    if (h.hit_ == true && i!=nearesObject) {
      if (h.dist_ < dist) {
            newNearestObject = i;
            nearestHit = h;
      }
    }

  }
  if(nearestHit.hit_ == true)
  {
    Color refCol;
  if(steps > 0 && newNearestObject->getMaterial()->ref_>0){
    float ref_coefficient  = newNearestObject->getMaterial()->ref_;
    float negRef = 1.0f-ref_coefficient;
    refCol = calculateReflection(nearestHit, reflectionRay, scene, newNearestObject, steps-1)*ref_coefficient+(getNormalColor(hit,ray, scene,nearesObject) *negRef);
  }
  else{
    refCol = getNormalColor(nearestHit, reflectionRay, scene, newNearestObject); 
  }
  return refCol;

  }
  else {
    return nearesObject->getMaterial()->ka_*nearesObject->getMaterial()->ref_;
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
