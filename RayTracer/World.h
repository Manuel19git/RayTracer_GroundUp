#pragma once
#include "Shape.h"

//------------------Intersection Info------------------
struct Itr
{
	//t for a ray intersecting with an object
	float t;
	//object
	Shape* object;
};
bool operator==(const Itr& lhs, const Itr& rhs);

//Computations (More Info about Intersections)
struct ItrComps
{
	//Intersection info
	float t;
	Shape* object;

	//Usefull values
	myPoint point;
	myPoint over_point;
	myVector eye;
	myVector normal;
	bool inside;
};


class World
{
public:
	World() 
	{
		//Creates a default world
		_light = light();
		_light.position = myPoint(-10, 10, -10);

		//Sphere 1
		Sphere s1;
		s1.mat.color = Color(0.8, 1.0, 0.6);
		s1.mat.diffuse = 0.7;
		s1.mat.specular = 0.2;

		//Sphere 2
		Sphere s2;
		s2.transform = scaling(0.5, 0.5, 0.5);

		objects.push_back(&s1);
		objects.push_back(&s2);

	}

	World(vector<Shape*> listObjects, Light newLight)
	{
		objects = listObjects;
		_light = newLight;
	}


	vector<Shape*> objects;
	Light _light;

};
//Intersect ray with a shape
vector<Itr> intersect(myRay ray, Shape* object);

//Generate an array of interactions
vector<Itr> intersections(Itr it1, Itr it2);

//Returns the closest valid intersection
Itr hit(vector<Itr> xs);

//Returns color at a position
Color lighting(Material m, Light l, myPoint p, myVector eye, myVector normal, bool inShadow);

//Checks if a ray intersects with anything on the specified world
vector<Itr> intersect_world(World world, myRay ray);

//Precompute computation structure with intersection info
ItrComps prepare_computations(Itr intersection, myRay ray);

//Color of the intersection encapsulated in the given world
Color shade_hit(World world, ItrComps comps);

//Returns color of intersection by a ray in the given world
Color color_at(World world, myRay ray);

//Check if a point is in shadows
bool is_shadowed(World world, myPoint point);