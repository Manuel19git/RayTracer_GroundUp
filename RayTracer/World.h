#pragma once
#include <iostream>
#include "myRay.h"


class World
{
public:
	World() 
	{
		//Creates a default world
		_light = light();
		_light.position = myPoint(-10, 10, -10);

		//Sphere 1
		Sphere s1 = sphere();
		s1.material.color = Color(0.8, 1.0, 0.6);
		s1.material.diffuse = 0.7;
		s1.material.specular = 0.2;

		//Sphere 2
		Sphere s2 = sphere();
		s2.transform = scaling(0.5, 0.5, 0.5);

		objects.push_back(s1);
		objects.push_back(s2);

	}

	World(vector<Sphere> listObjects, Light newLight)
	{
		objects = listObjects;
		_light = newLight;
	}


	vector<Sphere> objects;
	Light _light;

};

//Checks if a ray intersects with anything on the specified world
vector<Itr> intersect_world(World w, myRay r);