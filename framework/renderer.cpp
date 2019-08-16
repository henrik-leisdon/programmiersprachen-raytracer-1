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
  read_sdf("/home/vanessaretz/Schreibtisch/raytracer/programmiersprachen-raytracer-1/framework/materials.sdf", scene);
  //read_sdf("/home/henrik/Google_Drive/Uni/git/buw_raytracer_new/programmiersprachen-raytracer-1/framework/materials.sdf", scene);
  int i = scene.shapes_.size() ;
 
  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x,y);
      //Ray pixRay = scene.camera_.camRay(x, y, width_, height_);
      //float d = (width_/2.0f) / tan((scene.camera_.getAngle()/2.0f)); //* M_PI/180.0f
      //float dTwo = (width_/2.0f) / tan((scene.camera_.getAngle()/2.0f * (float)M_PI/180.0f)); //
      //cout << "d = " << d << "\n";
      //cout << "d2 = " << dTwo << "\n";

      vec3 origin{0.0f,0.0f,0.0f};
      vec3 direction{x-width_/2.0f,y-height_/2.0f, -101.0f};
      vec3 normalizedDirection{normalize(direction)};
      Ray ray{origin, normalizedDirection};
      //std::cout << width_ << " " << height_ << std::endl;
      //std::cout << direction.x  << " " << direction.y << " " << direction.z << std::endl;
      //std::cout << normalizedDirection.x << " " << normalizedDirection.y << " " << normalizedDirection.z << std::endl;
      vec3 rayColor {(normalizedDirection+vec3{1})*vec3{0.5}};
      //p.color = Color{x/float(width_), y/float(height_), 0.0f};//(direction+vec3{1})*vec3{0.5}
      //p.color = Color{rayColor.x, rayColor.y, rayColor.z};

      p.color = trace(ray, scene);
      write(p);
      //cout << "height: " << x << " width: " <<y <<"\n";
    }
  }
  ppm_.save(filename_);
}

//get the color for the pixel (temporary just normal color without shadow)
Color Renderer::trace(Ray const &ray, Scene const &scene) {
  //cout << "in get pixel Color \n";
  float delta = 1;
  float distance = 0.0f;
  float dist = 0.0f;

  shared_ptr<Shape> nearestObject;

  for (auto i : scene.shapes_) {
    Hit h = i->intersect(ray, distance);
    //cout << "distPtr value: " << distance << "\n";
    if (h.hit_ == true) {
      //cout<< " name " << i->getName() << " dist " << h.dist_ << " distance value  " << dist << " in hit \n ";
      if (h.dist_ < dist || dist == 0.0f) {
        dist = h.dist_;
        nearestObject = i;
        //cout << " nearest (name) " << nearestObject->getName() << "\n";

          if (nearestObject != nullptr) {

              //cout << nearestObject->getName() << " " << scene.shapes_.size() <<"\n";
              //diff_color;//getIllumination(h, i, scene); //
              Color amb = getAmbientIllumination(h, scene);
              Color col = ptLight(h, ray, scene, delta);
              return col*delta; //amb+(col*delta);
          } else {
              return Color{1.0f, 1.0f, 1.0f};
          }
      }
    }
    //Color pt = ptLight(h, ray, scene, delta); JUST the light
      //return pt;
  }
  /* if (nearestObject != nullptr) {

    return nearestObject->getMaterial()->ka_;
  } else {
    return Color{1.0f, 1.0f, 1.0f};
  } */
}

Color Renderer::getAmbientIllumination(Hit const& hit, Scene const& scene){
  for(auto i : scene.light_) {
    Color ia = i->getColor();
    for(auto j : scene.shapes_) {
      Color ka = j->getMaterial()->ka_;
      Color amb_col = ia*ka;
      return amb_col;
    }
  }
}

Color Renderer::ptLight(Hit const &hit, Ray const &ray, Scene const& scene ,float delta) {
    for(auto i : scene.light_){
      vec3 lightPos = i->getPos();
      vec3 vecToLight = {lightPos.x-hit.hitpoint_.x, lightPos.y-hit.hitpoint_.y,lightPos.z-hit.hitpoint_.z};
      vec3 normVecToLight = glm::normalize(vecToLight);

      //float distance = glm::length(hit.hitpoint_ - i->getPos());
      Color diffuse = getDiffuseIllumination(hit, normVecToLight, scene);
      return diffuse;
    }
}

Color Renderer::getDiffuseIllumination(Hit const &hit , vec3 normVecToLight, Scene const& scene)
{
  for(auto i : scene.light_) {
    float vec = dot(normalize(hit.direction_), normVecToLight);

    Color diff_col = i->getColor();
    int bright = i->getBrightness();
    Color ip = diff_col * bright;
    for(auto j : scene.shapes_) {
        Color kd = j->getMaterial()->kd_;
        Color diffuse_color = ip * kd * vec;

        return diffuse_color;
    }
  }
}


void Renderer::write(Pixel const& p)
{
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
