#include <glm-0.9.5.3/glm/glm.hpp>
#include "box.hpp"
#include "plane.hpp"
#include <vector>

using namespace std;
using namespace glm;

Box::Box():
    Shape(),
    boxMin_({0.0,0.0,0.0}),
    boxMax_({0.0,0.0,0.0})
    {};

Box::Box(string name, vec3 const& boxMin, vec3 const& boxMax, shared_ptr<Material> material):
    Shape{name, material},
    boxMin_{boxMin},
    boxMax_{boxMax}
    {};

Box::~Box() {};

//getter
vec3 Box::getBoxMin()
{
    return boxMin_;
}

vec3 Box::getBoxMax()
{
    return boxMax_;
}

double Box::area() const 
{
    double length{boxMax_.x-boxMin_.x};
    double height{boxMax_.y-boxMin_.y};
    double width{boxMax_.z-boxMin_.z};

    return 2*length*height+2*length*width+2*height*width;
}

double Box::volume() const {

    double length{boxMax_.x-boxMin_.x};
    double height{boxMax_.y-boxMin_.y};
    double width{boxMax_.z-boxMin_.z};

    double volume = length*height*width;
    return volume;
}

Hit Box::intersect(Ray const &ray, float &t) {
    
if (boxMin_.x > boxMax_.x) std::swap(boxMin_.x, boxMax_.x);
if (boxMin_.y > boxMax_.y) std::swap(boxMin_.y, boxMax_.y);
if (boxMin_.z > boxMax_.z) std::swap(boxMin_.z, boxMax_.z);

//cout << "boxMin z" << boxMin_.z << " boxMax z" << boxMax_.z << endl;

vec3 dirNormaized = normalize(ray.direction);

float xMinDist = (boxMin_.x-ray.origin.x)/dirNormaized.x;
float xMaxDist = (boxMax_.x-ray.origin.x)/dirNormaized.x;
float yMinDist = (boxMin_.y-ray.origin.y)/dirNormaized.y;
float yMaxDist = (boxMax_.y-ray.origin.y)/dirNormaized.y;
float zMaxDist = (boxMin_.z-ray.origin.z)/dirNormaized.z;
float zMinDist = (boxMax_.z-ray.origin.z)/dirNormaized.z;
//cout << "min dist: " << zMinDist << " max dist " << zMaxDist  <<endl;
//cout << "dir x min: " << xMinDist << " dir x max: " << xMaxDist << "dir y min: " << yMinDist << " dir y max: " << yMaxDist << "dir z min: " << zMinDist << " dir z max: " << zMaxDist << endl;

//possible hitpoints:
vec3 xMinHitP = ray.origin + xMinDist*(dirNormaized);
vec3 xMaxHitP = ray.origin + xMaxDist*(dirNormaized);
vec3 yMinHitP = ray.origin + yMinDist*(dirNormaized);
vec3 yMaxHitP = ray.origin + yMaxDist*(dirNormaized);
vec3 zMinHitP = ray.origin + zMinDist*(dirNormaized);
vec3 zMaxHitP = ray.origin + zMaxDist*(dirNormaized);

Hit hitpoints[6];


//cout << "xMinHitPoint: " << xMinHitP.x << " " << xMinHitP.y << " " << xMinHitP.z << endl;

//left:

hitpoints[0].hit_ = false;
if(xMinHitP.y <= boxMax_.y && xMinHitP.y >= boxMin_.y && 
    xMinHitP.z <= boxMax_.z && xMinHitP.z >= boxMin_.z && xMinDist > 0)
{
    //cout << "hit inleft";
    hitpoints[0].hit_ = true;
    hitpoints[0].hitpoint_ = xMinHitP;
    hitpoints[0].dist_ = xMinDist;
    hitpoints[0].hitnormal_ = vec3{-1.0f,0.0f,0.0f};
}


//right:
hitpoints[1].hit_ = false;
if(xMaxHitP.y <=boxMax_.y && xMaxHitP.y >= boxMin_.y
    && xMaxHitP.z <= boxMax_.z && xMaxHitP.z >= boxMin_.z && xMaxDist > 0)
{
    hitpoints[1].hit_ = true;
    hitpoints[1].hitpoint_ = xMaxHitP;
    hitpoints[1].dist_ = xMaxDist;
    hitpoints[1].hitnormal_ = vec3{1.0f,0.0f,0.0f};
}
//bottom
hitpoints[2].hit_ = false;
if(yMinHitP.x <= boxMax_.x && yMinHitP.x >= boxMin_.x 
    && yMinHitP.z <= boxMax_.z && yMinHitP.z >= boxMin_.z && yMinDist > 0)
{
    hitpoints[2].hit_ = true;
    hitpoints[2].hitpoint_ = yMinHitP;
    hitpoints[2].dist_ = yMinDist;
    hitpoints[2].hitnormal_ = vec3{0.0f,-1.0f,0.0f};
}

//top:
hitpoints[3].hit_ = false;
if(yMaxHitP.x <= boxMax_.x && yMaxHitP.x >= boxMin_.x
    && yMaxHitP.z <= boxMax_.z && yMaxHitP.z >= boxMin_.z && yMaxDist > 0)
{
    hitpoints[3].hit_ = true;
    hitpoints[3].hitpoint_ = yMaxHitP;
    hitpoints[3].dist_ = yMaxDist;
    hitpoints[3].hitnormal_ = vec3{0.0f,1.0f,0.0f};
    
}


//front:

hitpoints[4].hit_ = false;
if(zMinHitP.x <=boxMax_.x && zMinHitP.x >= boxMin_.x 
    && zMinHitP.y <= boxMax_.y && zMinHitP.y >= boxMin_.y && zMinDist > 0)
{
    hitpoints[4].hit_ = true;
    hitpoints[4].hitpoint_ = zMinHitP;
    hitpoints[4].dist_ = zMinDist;
    hitpoints[4].hitnormal_ = vec3{0.0f,0.0f,1.0f};
   
}

//back:
hitpoints[5].hit_ = false;
if(zMaxHitP.x <=boxMax_.x && zMaxHitP.x >= boxMin_.x 
    && zMaxHitP.y <= boxMax_.y && zMaxHitP.y >= boxMin_.y && zMaxDist > 0)
{
    hitpoints[5].hit_ = true;
    hitpoints[5].hitpoint_ = zMaxHitP;
    hitpoints[5].dist_ = zMaxDist;
    hitpoints[5].hitnormal_ = vec3{0.0f,0.0f,-1.0f};
    //cout << "zmax dist: " << zMaxDist << endl;
}

float smallestDistance = 10000000.0f;
Hit closestHit;
 closestHit.hit_ = false;
 for(int i = 0; i< 5; i++){
     if(hitpoints[i].dist_<smallestDistance && hitpoints[i].hit_==true){
         //cout << "closest hit: " << hitpoints[i].hitpoint_.x << " " << hitpoints[i].hitpoint_.y << " " << hitpoints[i].hitpoint_.z << endl;
         closestHit = hitpoints[i];
         smallestDistance = hitpoints[i].dist_;
         //cout << "kante: " << i << " distance: " << smallestDistance << endl;
     }
 }

return closestHit;

	/*Hit hit[6];
	hit[0] = surfacehit(ray,glm::vec3{ boxMax_.x,boxMax_.y,boxMax_.z }, glm::vec3{ boxMax_.x,boxMin_.y,boxMax_.z }, glm::vec3{ boxMin_.x,boxMin_.y,boxMax_.z }, glm::vec3{ boxMin_.x,boxMax_.y,boxMax_.z });
	//back
	hit[1] = surfacehit(ray,glm::vec3{ boxMin_.x,boxMin_.y,boxMin_.z }, glm::vec3{ boxMax_.x,boxMin_.y,boxMin_.z }, glm::vec3{ boxMax_.x,boxMax_.y,boxMin_.z }, glm::vec3{ boxMin_.x,boxMax_.y,boxMin_.z });
	//right
	hit[2] = surfacehit(ray, glm::vec3{ boxMax_.x,boxMax_.y,boxMax_.z }, glm::vec3{ boxMax_.x,boxMax_.y,boxMin_.z }, glm::vec3{ boxMax_.x,boxMin_.y,boxMin_.z }, glm::vec3{ boxMax_.x,boxMin_.y,boxMax_.z });
	//left
	hit[3] = surfacehit(ray, glm::vec3{ boxMin_.x,boxMin_.y,boxMin_.z }, glm::vec3{ boxMin_.x,boxMax_.y,boxMin_.z }, glm::vec3{ boxMin_.x,boxMax_.y,boxMax_.z }, glm::vec3{ boxMin_.x,boxMin_.y,boxMax_.z });
	//top
	hit[4] = surfacehit(ray, glm::vec3{ boxMax_.x,boxMax_.y,boxMax_.z }, glm::vec3{ boxMin_.x,boxMax_.y,boxMax_.z }, glm::vec3{ boxMin_.x,boxMax_.y,boxMin_.z }, glm::vec3{ boxMax_.x,boxMax_.y,boxMin_.z });
	//bottom
	hit[5] = surfacehit(ray, glm::vec3{ boxMin_.x,boxMin_.y,boxMin_.z }, glm::vec3{ boxMin_.x,boxMin_.y,boxMax_.z }, glm::vec3{ boxMax_.x,boxMin_.y,boxMax_.z }, glm::vec3{ boxMax_.x,boxMin_.y,boxMin_.z });
	//front 
	Hit nearest;


	for (int i = 0; i < 6; i++) {
		if (hit[i].hit_ && 0.0001 < hit[i].dist_ && hit[i].dist_ < nearest.dist_) {
			nearest = hit[i];
		}
	}
	if (nearest.hit_) {
		
		return nearest;
	}
	else
	{
		return Hit{};
	}*/
}

Hit Box::surfacehit(Ray const& ray, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)const {
	Hit hit;
	glm::vec3 norm{ glm::normalize(glm::cross(p4 - p1,p2 - p1)) };
    //cout << "norm: " << norm.x << " " << norm.y << " " << norm.z << endl;
	float denominator = glm::dot(norm, ray.direction);
    //cout << "denominator: " << denominator << endl;
 	if (denominator != 0) {
		float distance = (-(norm.x*(ray.origin.x - p1.x)) - (norm.y*(ray.origin.y - p1.y))
			- (norm.z*(ray.origin.z - p1.z))) / denominator;//ebenenintersect
        if (distance > 0.001)
		{
           // cout << "distance: " << distance << endl;
		    
			glm::vec3 object_position = ray.origin + (distance * ray.direction);

			hit.hitpoint_ = object_position;
			{
				if (glm::dot(p4 - p1, p1 - hit.hitpoint_) <= 0 && glm::dot(p1 - p2, p2 - hit.hitpoint_) <= 0 && glm::dot(p2 - p3, p3 - hit.hitpoint_) <= 0 && glm::dot(p3 - p4, p4 - hit.hitpoint_) <= 0)
				{//flaechenbegrenzung
					
					hit.hit_ = true;
					hit.hitnormal_ = vec3{0.0,0.0,0.0};
					hit.dist_ = distance;
				}
			}
		}
	}
	if (hit.hit_) {
		return hit;
	}
	else { return Hit{}; }
}




/*


    Hit result;
    bool test;
    vec3 bounds[2];
    int sign[3];
    
    float distance;

    vector<vec3> cut_pts;
    vector<vec3> cut_norms;

    bounds[0] = boxMin_;
    bounds[1] = boxMax_;

    vec3 inverseDirection = {1.0f/ray.direction.x , 1.0f/ray.direction.y , 1.0f/ray.direction.z};

    sign[0] = (inverseDirection.x < 0);
    sign[1] = (inverseDirection.y < 0);
    sign[2] = (inverseDirection.z < 0);

    //distances
    float txMin, txMax, tyMin, tyMax, tzMin, tzMax;
    txMin = (bounds[sign[0]].x - ray.origin.x)*inverseDirection.x;
    txMax = (bounds[1 - sign[0]].x - ray.origin.x)*inverseDirection.x;

    tyMin = (bounds[sign[1]].y - ray.origin.y)*inverseDirection.y;
    tyMax = (bounds[1 - sign[1]].y - ray.origin.y)*inverseDirection.y;

    tzMin = (bounds[sign[2]].z - ray.origin.z)*inverseDirection.z;
    tzMax = (bounds[1 - sign[2]].z - ray.origin.z)*inverseDirection.z;

    cout << "distances: " << "txMin: " << txMin << " txMax: " << txMax << " tyMin: " << tyMin << " tyMax: " << tyMax << " tzMin: " << tzMin << " tzMax: " << tzMax << endl;

    if( (txMin > tyMax) || (tyMin > txMax)) { test = false;}
    if( tyMin > txMin) { txMin = tyMin;}
    if( tyMax < txMax) { txMax = tyMax;}

    if( (txMin > tzMax) || (tzMin > txMax)) { test = false;}
    if( tzMin > txMin) { txMin = tzMin;}
    if( tzMax < txMax) { txMax = tzMax;}

    if( (txMin >= t) && (txMax <= t)) { test = true; }

    vec3 cut_pt;
    vec3 cut_norm;

    if(txMin != 0){
        cut_pt = ray.origin + txMin * ray.direction;
        cut_norm = {-1.0f, 0.0f, 0.0f};
        if(cut_pt.y <= boxMax_.y && cut_pt.y >= boxMin_.y && cut_pt.z <= boxMax_.z && cut_pt.z >= boxMin_.z) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    if(txMax != 0){
        cut_pt = ray.origin + txMax * ray.direction;
        cut_norm = {1.0f, 0.0f ,0.0f};
        if(cut_pt.y <= boxMax_.y && cut_pt.y >= boxMin_.y && cut_pt.z <= boxMax_.z && cut_pt.z >= boxMin_.z) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    if(tyMin != 0){
        cut_pt = ray.origin + tyMin * ray.direction;
        cut_norm = {0.0f, -1.0f, 0.0f};
        if(cut_pt.x <= boxMax_.x && cut_pt.x >= boxMin_.x && cut_pt.z <= boxMax_.z && cut_pt.z >= boxMin_.z) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    if(tyMax != 0){
        cut_pt = ray.origin + tyMax * ray.direction;
        cut_norm = {0.0f, 1.0f, 0.0f};
       if(cut_pt.x <= boxMax_.x && cut_pt.x >= boxMin_.x && cut_pt.z <= boxMax_.z && cut_pt.z >= boxMin_.z) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    if(tzMin != 0){
        cut_pt = ray.origin + tzMin * ray.direction;
        cut_norm = {0.0f, 0.0f, -1.0f};
       if(cut_pt.x <= boxMax_.x && cut_pt.x >= boxMin_.x && cut_pt.y <= boxMax_.y && cut_pt.y >= boxMin_.y) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    if(tzMax != 0){
        cut_pt = ray.origin + tzMax * ray.direction;
        cut_norm = {0.0f, 0.0f, 1.0f};
        if(cut_pt.x <= boxMax_.x && cut_pt.x >= boxMin_.x && cut_pt.y <= boxMax_.y && cut_pt.y >= boxMin_.y) {
            cut_pts.push_back(cut_pt);
            cut_norms.push_back(cut_norm);
            test = true;
        }
    }

    vec3 closest_cut;
    vec3 closest_norm;

    if(cut_pts.size() > 0){
        closest_cut = cut_pts.at(0);
        closest_norm = cut_norms.at(0);
        //closest_norm = cut_norms.at(0);
        for(auto i = 0; i < cut_pts.size(); ++i){
            if(glm::length(cut_pts.at(i) - ray.origin) < glm::length(closest_cut - ray.origin)){
                closest_cut = cut_pts.at(i);
                closest_norm = cut_norms.at(i);
               // cout << "----------------- got normal ----------------" << closest_norm.x << " " << closest_norm.y << " " << closest_norm.z << "\n";
            }
        }

        vec3 directionR = ray.direction;
        vec3 normDirection = glm::normalize(directionR);

        distance = glm::length(cut_pt - ray.origin);
        result={test, distance, cut_pt, normDirection, closest_norm};

       // cout << "test " << test << "\n";
       // cout << "distance " << distance << "\n";
       // cout << "hitpoint " << cut_pt.x << " " << cut_pt.y << " " << cut_pt.z << "\n";
       // cout << "direction " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << "\n";
       // cout << "normalized direction " << normDirection.x << " " << normDirection.y << " " << normDirection.z << "\n";
       // cout << "normal " << closest_norm.x << " " << closest_norm.y << " " << closest_norm.z << "\n";
        return result;
    }

}
*/
ostream& Box::print(ostream &os) const {
    Shape::print(os);
    os
    << "Position Min : " << boxMin_.x << ", " << boxMin_.y << ", " << boxMin_.z << "\n"
    << "Position Max : " << boxMax_.x << ", " << boxMax_.y << ", " << boxMax_.z << "\n";

    return os;
}

ostream& operator << (ostream& os, const Box& b) {
    return b.print(os);
}

/*float finalDistance;
    bool test;
    Hit result;

    vec3 normDir;
    vec3 cutting_pt;
    vec3 closest_cut;
    vec3 closest_normal;
    vector<vec3> cutting_pts;
    vector<vec3> cutting_norm;

    Plane planeOne   {boxMin_, vec3{-1,0,0} };
    Plane planeTwo   {boxMin_, vec3{0,0,-1} };
    Plane planeThree {boxMin_, vec3{0,-1,0} };
    Plane planeFour  {boxMax_, vec3{1,0,0 } };
    Plane planeFive  {boxMax_, vec3{0,0,1 } };
    Plane planeSix   {boxMax_, vec3{0,1,0 } };

    float distance1 = (dot(planeOne.normal, planeOne.origin) - dot(ray.origin, planeOne.normal)) / (dot(ray.direction, planeOne.normal));
    float distance2 = (dot(planeTwo.normal, planeTwo.origin) - dot(ray.origin, planeTwo.normal)) / (dot(ray.direction, planeTwo.normal));
    float distance3 = (dot(planeThree.normal, planeThree.origin) - dot(ray.origin, planeThree.normal)) / (dot(ray.direction, planeThree.normal));
    float distance4 = (dot(planeFour.normal, planeFour.origin) - dot(ray.origin, planeFour.normal)) / (dot(ray.direction, planeFour.normal));
    float distance5 = (dot(planeFive.normal, planeFive.origin) - dot(ray.origin, planeFive.normal)) / (dot(ray.direction, planeFive.normal));
    float distance6 = (dot(planeSix.normal, planeSix.origin) - dot(ray.origin, planeSix.normal)) / (dot(ray.direction, planeSix.normal));

    normDir = normalize(ray.direction);

    if(distance1 != 0) {
        cutting_pt={ray.origin + distance1 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeOne.normal);
            test= true;
        }
    }

    if(distance4 != 0) {
        cutting_pt={ray.origin + distance4 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeFour.normal);
            test= true;
        }
    }

    if(distance2 != 0) {
        cutting_pt={ray.origin + distance2 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeTwo.normal);
            test= true;
        }
    }

    if(distance5 != 0) {
        cutting_pt={ray.origin + distance5 * ray.direction};
        if(cutting_pt.y < boxMax_.y && cutting_pt.y > boxMin_.y && cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeFive.normal);
            test= true;
        }
    }normDir = normalize(ray.direction);

    if(distance3 != 0) {
        cutting_pt={ray.origin + distance3 * ray.direction};
        if(cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeThree.normal);
            test= true;
        }
    }

    if(distance6 > 0) {
        cutting_pt={ray.origin + distance6 * ray.direction};
        if(cutting_pt.x < boxMax_.x && cutting_pt.x > boxMin_.x && cutting_pt.z < boxMax_.z && cutting_pt.z > boxMin_.z){
            cutting_pts.push_back(cutting_pt);
            cutting_norm.push_back(planeSix.normal);
            test= true;
        }
    }

    if(cutting_pts.size() > 0) {
        closest_cut = cutting_pts.at(0);
        closest_normal = cutting_norm.at(0);
        for(auto i = 0; i < cutting_pts.size(); ++i){
            if(glm::length(cutting_pts.at(i) - ray.origin) < glm::length(closest_cut - ray.origin)){
                closest_cut = cutting_pts.at(i);
                closest_normal = cutting_norm.at(i);
                cout << "dist " << finalDistance << "\n";
                cout << "hitpoint " << closest_cut.x << " " << closest_cut.y << " " << closest_cut.z << "\n";
                cout << "normal " << closest_normal.x << " " << closest_normal.y << " " << closest_normal.z << "\n";
                test = true;
            }
        }

        finalDistance = glm::length(cutting_pt - ray.origin);

        //cout << "dist " << finalDistance << "\n";
        //cout << "hitpoint " << closest_cut.x << " " << closest_cut.y << " " << closest_cut.z << "\n";
        //cout << "normal " << closest_normal.x << " " << closest_normal.y << " " << closest_normal.z << "\n";

    }

    result = {test, finalDistance, closest_cut, normDir , closest_normal};
    return result;
     */
