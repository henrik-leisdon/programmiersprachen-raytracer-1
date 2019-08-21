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
//#include "composite.hpp"

using namespace std;
using namespace glm;

struct Scene
{
    public:
       // shared_ptr<Material> findMaterial(string MaterialName, Scene& scene);
        void read_sdf(string const& path, Scene& scene);

        Color getPixelColor(Ray const& ray);
        

        map <string, shared_ptr<Material>> materialMap;
        vector<shared_ptr<Shape>> shapes_;
        //shared_ptr<Composite> composite_;
        vector<shared_ptr<Light>> light_;
        Camera camera_;
        vector<shared_ptr<Camera>> camera_p;


};

/* shared_ptr<Material> findMaterial(string materialName, Scene& scene)
{
    if(scene.materialMap.find(materialName)!=scene.materialMap.end())
    {
        return scene.materialMap.find(materialName)->second;
    }
    else
    {
        return nullptr;
    }
    
}*/

static void read_sdf(string const& path, Scene& scene) {
    ifstream file(path);
    string line;

    if (!file.is_open()) {
        cout << "Error. Couldn't find file";
    } else {
        cout << "was able to read file";
    }

    while (getline(file, line)) {
        istringstream ss(line);
        vector<string> lineparts;
        do {
            string word;
            ss >> word;
            lineparts.push_back(word);
        } while (ss);

        if (lineparts[0] == "define") {
            cout << "\n in define \n";
            if (lineparts[1] == "material") {
                string name = lineparts[2];
                Color ka = {stof(lineparts[3]), stof(lineparts[4]), stof(lineparts[5])};
                Color kd = {stof(lineparts[6]), stof(lineparts[7]), stof(lineparts[8])};
                Color ks = {stof(lineparts[9]), stof(lineparts[10]), stof(lineparts[11])};
                float m = stof(lineparts[12]);

                shared_ptr<Material> matPtr = make_shared<Material>(name, ka, kd, ks, m);
                scene.materialMap.insert(pair<string, shared_ptr<Material>>(name, matPtr));

            }
            if (lineparts[1] == "shape") {
                if (lineparts[2] == "box") {
                    vec3 boxMin = {stof(lineparts[4]), stof(lineparts[5]), stof(lineparts[6])};
                    vec3 boxMax = {stof(lineparts[7]), stof(lineparts[8]), stof(lineparts[9])};
                    //shared_ptr<Material> mat = findMaterial(lineparts[10],scene);
                    shared_ptr<Material> mat = scene.materialMap.at(lineparts[10]);

                    Box box = Box(lineparts[3], boxMin, boxMax, mat);

                    shared_ptr<Box> boxPtr = make_shared<Box>(box);
                    scene.shapes_.push_back(boxPtr);
                    cout << " box loaded ";
                }

                if (lineparts[2] == "sphere") {
                    vec3 center = {stof(lineparts[4]), stof(lineparts[5]), stof(lineparts[6])};
                    float radius = stof(lineparts[7]);
                    shared_ptr<Material> mat = scene.materialMap.at(lineparts[8]);
                    //shared_ptr<Material> mat = findMaterial(lineparts[8],scene);

                    Sphere sphere = Sphere(lineparts[3], center, radius, mat);
                    shared_ptr<Sphere> spherePtr = make_shared<Sphere>(sphere);
                    scene.shapes_.push_back(spherePtr);
                    cout << " sphere loaded ";
                }
            }
            if (lineparts[1] == "light") {
                vec3 postition = {stof(lineparts[3]), stof(lineparts[4]), stof(lineparts[5])};
                Color color = {stof(lineparts[6]), stof(lineparts[7]), stof(lineparts[8])};
                //Color ambColor = {stof(lineparts[9]), stof(lineparts[10]), stof(lineparts[11])};

                Light light = Light(lineparts[2], postition, color, stoi(lineparts[9]));
                shared_ptr<Light> lightPtr = make_shared<Light>(light);
                scene.light_.push_back(lightPtr);

            }

            /*if(lineparts[1] == "camera")
            {
                vec3 defaultPos = {0.0f, 0.0f, 0.0f};
                float defaultFov = 45.0f;
                Camera camera = Camera(lineparts[2], defaultFov, defaultPos);
            }*/


            if (lineparts[1] == "camera") {
                vec3 position = {stof(lineparts[4]), stof(lineparts[5]), stof(lineparts[6])};
                float fov = {stof(lineparts[3])};
                Camera camera = Camera((lineparts[2]), fov, position);
                
                shared_ptr<Camera> camptr = make_shared<Camera>(camera);
                scene.camera_p.push_back(camptr);
                cout << "camera loaded ";
                cout << "fov : " << camera.getAngle() << "\n";
            }

        }
    }

}