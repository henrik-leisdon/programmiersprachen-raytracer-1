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
  read_sdf("/home/henrik/Google_Drive/Uni/git/buw_raytracer_new/programmiersprachen-raytracer-1/framework/materials.sdf", scene);
  int i = scene.shapes_.size() ;
  //  for(auto i : scene.camera_p){
  //      a = i->getAngle();

  //      cout << "fov" << " " << a << "\n";
  //      d = (width_/2.0f) / (tan(a/2.0f));
  //      cout << "d " << " " << d << "\n";

  // }

  int a;
  float d;
  //Camera cam = scene.camera_;
  //d = (width_/2.0f) / (tan(cam.getAngle()/2.0f));
  d = (width_/2.0f) / (tan(70/2.0f*M_PI/180));

  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x,y);

      vec3 origin{0.0f,0.0f, 0.0f};
      vec3 direction{x-width_/2.0f,y-height_/2.0f, -d};
      vec3 normalizedDirection{glm::normalize(direction)};
      Ray ray{origin, normalizedDirection};


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

    float distance = 0.0f;
    float dist = 100000.0f;

    shared_ptr<Shape> nearestObject;
    Hit h;

    for (auto i : scene.shapes_) {
        h = i->intersect(ray, distance);
        //cout << "distPtr value: " << distance << "\n";
        if (h.hit_ == true) {
            //cout<< " name " << i->getName() << " dist " << h.dist_ << " distance value  " << dist << " in hit \n ";
            if (h.dist_ < dist || dist == 0) {//h.dist_ < dist || dist == 0.0f) {
                dist = h.dist_; //h.dist_;
                nearestObject = i;
            }
        }
    }

    if (nearestObject != nullptr) {
        //cout << nearestObject->getName() << " " << scene.shapes_.size() <<"\n";
        Color col = ptLight(h, ray, scene);
        Color ambiance = nearestObject->getMaterial()->ka_;
        return ambiance+col; //ambobj;
    } else {
        return Color{1.0f, 1.0f, 1.0f};
    }
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

Color Renderer::ptLight(Hit const &hit, Ray const &ray, Scene const& scene) {
    vec3 normVecToLight;
    vec3 lightPos;
    vec3 vecToLight;
    Color diffuse;

    for(auto i : scene.light_){
      lightPos = i->getPos();
      vecToLight = {lightPos.x-hit.hitpoint_.x, lightPos.y-hit.hitpoint_.y,lightPos.z-hit.hitpoint_.z};
      normVecToLight = glm::normalize(vecToLight);
      diffuse = getDiffuseIllumination(hit, normVecToLight, scene);
            }
              return diffuse;
        }


Color Renderer::getDiffuseIllumination(Hit const &hit , vec3 normVecToLight, Scene const& scene) {
    Color diff_col;
    int bright;
    Color ip;
    Color kd;
    Color diffuse_color;
    float vec;

    for (auto i : scene.light_) {
    vec = dot(normalize(hit.direction_), normVecToLight);
    diff_col = i->getColor();
    bright = i->getBrightness();
    ip = diff_col * bright;
        for (auto j : scene.shapes_) {
          kd = j->getMaterial()->kd_;
            diffuse_color = ip * kd * vec;
            }
        }
    return diffuse_color;
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
