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
  cout << ". file loaded \n";
  int i = scene.shapes_.size() ;
  cout <<  "scene size: " << i << "\n";

  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      
      Pixel p(x,y);
      vec3 origin{400.0f,300.0f,0.0f};
      vec3 direction{x-origin.x/2,y-origin.y/2,-50.0f};
      Ray ray{origin, normalize(direction)};

      p.color = getPixelColor(ray, scene);

      /* if ( ((x/checker_pattern_size)%2) != ((y/checker_pattern_size)%2)) {
        p.color = Color{1.0f, 1.0f, float(y)/height_};
      } else {
        p.color = Color{1.0f, 0.0f, float(y)/width_};
      }
      */
      write(p);
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
      return i->getMaterial()->getColor();

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
