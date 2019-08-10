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
//#include "camera.hpp"
#include <string>
#include <glm/glm.hpp>

class Renderer
{
public:
  Renderer(Scene const& scene, unsigned w, unsigned h, std::string const& file); //

  void render();
  Color getPixelColor(Ray const& ray, Scene const& scene);
  Color getIllumination(Hit const& hit, shared_ptr<Shape> nearestObj, Scene const& scene);

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
