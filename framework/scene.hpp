#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <memory>

#include "material.hpp"
#include "shape.hpp"
#include "box.hpp"
#include "sphere.hpp"

using namespace std;
//using namespace glm;

class Scene
{
    public:
        void read_sdf(string const& path, Scene& scene);

        Color getPixelColor(Ray const& ray);
        shared_ptr<Material> findMaterial(Ray const& ray);

        map <string, shared_ptr<Material>> materialMap;

};