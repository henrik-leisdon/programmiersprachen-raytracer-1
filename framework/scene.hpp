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
#include "camera.hpp"
#include "light.hpp"

using namespace std;
using namespace glm;

struct Scene
{
    public:
        void read_sdf(string const& path, Scene& scene);

        Color getPixelColor(Ray const& ray);
        

        map <string, shared_ptr<Material>> materialMap;

};

shared_ptr<Material> findMaterial(string materialName, Scene& scene)
{

}

static void read_sdf(string const& path, Scene& scene)
{
    ifstream file(path);
    string line;

    if(!file.is_open()) 
    {
        cout << "Error. Couldn't find file";
    } else {
        cout << "was able to read file";
    }
    
    while(getline(file,line)) 
    {
        vector<string> lineparts;
        lineparts.push_back(line);

        if(lineparts[0] == "define")
        {
            if(lineparts[1] == "material")
            {
                string name = lineparts[2];
                Color ka = {stof(lineparts[3]), stof(lineparts[4]), stof(lineparts[5])};
                Color kd = {stof(lineparts[6]), stoi(lineparts[7]), stof(lineparts[8])};
                Color ks = {stof(lineparts[9]), stof(lineparts[10]), stof(lineparts[11])};
                float m = stof(lineparts[12]);
                
                shared_ptr<Material> matPtr = make_shared<Material>(name, ka, kd, ks, m);
                
            }
            if(lineparts[1] == "shape") 
            {
                if(lineparts[2] == "box") 
                {
                    vec3 boxMin = {stof(lineparts[4]), stof(lineparts[5]), stof(lineparts[6])};
                    vec3 boxMax = {stof(lineparts[7]), stof(lineparts[8]), stof(lineparts[9])};
                    Box box = Box(lineparts[3], boxMin, boxMax, scene.materialMap.at(lineparts[10]));

                    shared_ptr<Box> boxPtr = make_shared<Box>(box);
                }
                
                if(lineparts[2] == "sphere") 
                {
                vec3 center = {stof(lineparts[4]), stof(lineparts[5]), stof(lineparts[6])};
                float radius = stof(lineparts[7]);

                Sphere sphere = Sphere(lineparts[3], center, radius, scene.materialMap.at(lineparts[8]));
                
                }
            }
            if(lineparts[1] == "light") 
            {
                vec3 postition = {stof(lineparts[3]), stof(lineparts[4]), stof(lineparts[5])};
                Color color = {stof(lineparts[6]), stof(lineparts[7]), stof(lineparts[8])};

                Light light = Light(lineparts[2], postition, color, stoi(lineparts[9]));

            }
            if(lineparts[1] == "camera") 
            {
                vec3 defaultPosition = {0.0f,0.0f,0.0f};
                Camera camera = Camera(lineparts[2], stof(lineparts[3]), defaultPosition);
            }
            
        }
    }
}