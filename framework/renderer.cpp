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
void Renderer::render()
{
  Scene scene;
  std::size_t const checker_pattern_size = 20;
  read_sdf("/home/henrik/Google_Drive/Uni/git/buw_raytracer_new/programmiersprachen-raytracer-1/framework/materials.sdf", scene);
  //cout << ". file loaded \n";
  int i = scene.shapes_.size() ;
  //cout <<  "scene size: " << i << "\n";

  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {      
      Pixel p(x,y);

      vec3 origin{0.0f,0.0f,0.0f};
      vec3 direction{x,y,-50.0f};
      Ray ray{origin, normalize(direction)};

      p.color = getPixelColor(ray, scene);
      write(p);
      //cout << "height: " << x << " width: " <<y <<"\n";
    }
  }
  ppm_.save(filename_);
}

//get the color for the pixel (temporary just normal color without shadow)
Color Renderer::getPixelColor(Ray const& ray, Scene const& scene)
{
  //cout << "in get pixel Color \n";
  float distance = 100000.0f;
  shared_ptr<Shape> nearestObject;
  
  for(auto i : scene.shapes_)
  {
    Hit h= i->intersect(ray, distance);
    if(h.hit_ == true){
        return  i->getMaterial()->ka_; //getIllumination(h, i, scene); //

    } else{
        return Color{1.0f,1.0f,1.0f};
    }
  }

  
}

Color Renderer::getIllumination(Hit const& hit, shared_ptr<Shape> nearestObj, Scene const& scene) 
{
  for(auto i : scene.light_)
  {
    vec3 lightPos = i->getPos();
    vec3 vecToLight = {lightPos.x-hit.hitpoint_.x, lightPos.y-hit.hitpoint_.z,lightPos.z-hit.hitpoint_.z};
    float vec = dot(normalize(hit.direction_), vecToLight);

    Color b = i->getColor()*i->getBrightness(); //Color{col.r*bright, col.g*bright, col.b*bright};
    //cout << "after calc color b";
    Color c = b*nearestObj->getMaterial()->getColor()*vec;
    //cout << "color: " << c.g << "\n" ;
    return c;
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
