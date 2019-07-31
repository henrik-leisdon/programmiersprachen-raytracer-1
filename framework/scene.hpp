#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <memory>



class Scene
{
    public:
        void read_sdf(std::string const& path, Scene& scene);

};