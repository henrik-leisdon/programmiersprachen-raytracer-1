#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include "shape.hpp"
#include "sphere.hpp"
#include "box.hpp"
#include "hit.hpp"
#include <vector>



class Composite : public Shape{

    public:
        Composite();
        Composite(std::string const& name);
        void addShape(std::shared_ptr<Shape> const& newShape);
        
        std::vector<std::shared_ptr<Shape>> composition_;

    private:

};

#endif // !COMPOSITE_HPP
