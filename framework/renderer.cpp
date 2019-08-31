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
  std::size_t const checker_pattern_size = 20;
  //read_sdf("/home/vanessaretz/Schreibtisch/raytracer/programmiersprachen-raytracer-1/framework/materials.sdf", scene);
  read_sdf("/home/IN/seso4651/Documents/raytracer/programmiersprachen-raytracer-1/framework/scene.sdf", scene);
  //read_sdf("/home/henrik/Google_Drive/Uni/git/buw_raytracer_new/programmiersprachen-raytracer-1/framework/scene.sdf", scene);

  shared_ptr<Camera> cam1 = scene.camera_;
  float d = (width_/2.0f) / (tan(cam1->getAngle()/2.0f*M_PI/180));
  int frames = 60;


  //#pragma omp parallel for
  for(int f = 0; f<frames; f++){

    shared_ptr<Camera> cam = scene.camera_;
    cam->translate(vec3{-1, 0, 1});
    cam->rotateY(-1.5);
    

  //#pragma omp parallel for
  for (unsigned y = 0; y < height_; ++y) {
    //#pragma omp parallel for
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x,y);
      int step = 2;
      
      Ray ray = cam->calc_cam_ray(x, y, height_, width_, d);
      //cout << "x: " << x << " y " << y <<endl;
      //vec3 origin = cam->getPos();
      //vec3 direction{x-width_/2.0f,y-height_/2.0f, -d};
      //vec3 normalizedDirection{glm::normalize(direction)};
      //Ray ray{origin, normalizedDirection};
      p.color = trace(ray, scene, step);

      write(p);
    }
  }
  cam = scene_.camera_;
  string number = to_string(f);
  string filename_frame = "raytracer_"+number;
  ppm_.save(filename_frame);
}
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
                hit.hit_ = h.hit_;
            }
        }
    }
    if (nearestObject != nullptr) {
      Color final;
      //refraction
      if(nearestObject->getMaterial()->opacity_>0 && step>=10){
        
        float opacity = nearestObject->getMaterial()->opacity_;
        float neg_opacity = 1-opacity;
        Color final = calculateRefraction(hit,ray, scene, nearestObject,nullptr, step)*opacity+(getNormalColor(hit,ray,scene,nearestObject)*neg_opacity);
        cout << "final color: " << final ;
        
      }
      //reflection
      else if(nearestObject->getMaterial()->ref_>0){

        float ref_coefficient  = nearestObject->getMaterial()->ref_;
        float negRef = 1.0f-ref_coefficient;
        final = calculateReflection(hit, ray, scene, nearestObject, step-1)*ref_coefficient+(getNormalColor(hit,ray, scene, nearestObject) *negRef);
        
      }
      //normal color/illumination
      else if(nearestObject->getMaterial()->opacity_== 0.0) {
        Color illumination = getNormalColor(hit, ray, scene, nearestObject);
        final =  illumination;  
        
      }
        return final;
        cout << "after final color: " << final << endl;
        
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
    return getNormalColor(hit, ray, scene, nearesObject);
  }
}

Color Renderer::calculateRefraction(Hit const& hit, Ray const& ray, Scene const& scene, shared_ptr<Shape> const& nearesObject , shared_ptr<Shape> const& prevObject, int steps){
  float eta = 1.0f;
  if(prevObject == nullptr){
     eta = 1.0/1.5;
  
  }else if(prevObject == nearesObject){
    eta = 1.5;
  }
  vec3 refractVec = normalize(glm::refract(normalize(ray.direction),hit.hitnormal_,eta));
  Ray refractRay = Ray(hit.hitpoint_-0.1f*hit.hitnormal_,refractVec);
  //cout << "steps:" <<steps <<" eta: " << eta <<endl;
  float distance1 = 0;
  float dist = 10000000.0f;
  Hit h;
  Hit newHit;
  shared_ptr<Shape> newNearestObject = nullptr;
  for(auto i : scene.shapes_){
    h = i->intersect(refractRay,distance1);
    if(h.hit_){
      if(h.dist_< dist){
        newNearestObject = i;
        newHit = h;
      }

    }
  }

  //if outside and hits shape behind:
  if(newNearestObject!= nearesObject && newNearestObject!=nullptr){
    //cout << "new nearest Object (object behind: " << newNearestObject->getName() << endl;
    Color bgCol = getNormalColor(newHit, refractRay, scene, newNearestObject);
    //cout << "backgroundObject color: " << bgCol << endl;
    return bgCol;
  }
  //if inside: 
  else if(newNearestObject == nearesObject){
    cout << "calc in to out \n";
    vec3 normal = newHit.hitnormal_;
    normal.x = -newHit.hitnormal_.x;
    normal.y = -newHit.hitnormal_.y;
    normal.z = -newHit.hitnormal_.z;
    newHit.hitnormal_ = normal;
    return calculateRefraction(newHit, refractRay, scene, newNearestObject ,nearesObject, steps-1);
  }
  else{
    cout << "get background --> no shape behind" << endl;
    return Color{0.1,0.1,0.1};
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
