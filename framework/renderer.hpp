// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#ifndef BUW_RENDERER_HPP
#define BUW_RENDERER_HPP

#include "color.hpp"
#include "pixel.hpp"
#include "scene.hpp"
#include "ppmwriter.hpp"
#include "camera.hpp"
#include <string>
#include <glm/glm.hpp>
#include <omp.h>

class Renderer
{
public:
  Renderer(Scene const& scene, unsigned w, unsigned h, std::string const& file); //

  void raycast();
  Color trace(Ray const& ray, Scene const& scene, int step);
  
  Color getAmbientIllumination(Hit const& hit,Scene const& scene, shared_ptr<Shape> const& nearestObject);
  Color getDiffuseIllumination(Hit const& hit, Scene const& scene, shared_ptr<Shape> const& nearestObject, shared_ptr<Light> const& light);
  Color getSpecularIllumination(Hit const& hit, Ray const& ray, Scene const& scene, shared_ptr<Shape> const& nearestObject, shared_ptr<Light> const& light);
  
  Color ptLight(Hit const &hit, Ray const &ray, Scene const& scene, shared_ptr<Shape> const& nearestObject);
  
  Color getNormalColor(Hit const& hit, Ray const& ray, Scene const& scene, shared_ptr<Shape> const& nearestObject);
  Color calculateReflection(Hit const& hit, Ray const& ray, Scene const& scene, shared_ptr<Shape> const& nearesObject, int steps);
  Color calculateRefraction(Hit const& hit, Ray const& ray, Scene const& scene, shared_ptr<Shape> const& nearesObject, shared_ptr<Shape> const& prevObject, int steps);
  
  void write(Pixel const& p);
  

  inline std::vector<Color> const& color_buffer() const
  {
    return color_buffer_;
  }

private:
  Scene scene_;
  unsigned width_;
  unsigned height_;
  std::vector<Color> color_buffer_;
  std::string filename_;
  PpmWriter ppm_;
};

#endif // #ifndef BUW_RENDERER_HPP
