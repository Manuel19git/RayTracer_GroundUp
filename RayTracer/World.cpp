#include "World.h"


//Non-member functions


//When comparing intersections we only look to the t value
bool comparisonItr(Itr a, Itr b)
{
	return a.t < b.t;
}

//Sorting by method bubble
void bubbleSort(vector<Itr> &xs)
{
	for (int i = 0; i < xs.size(); ++i)
	{
		for (int j = i + 1; j < xs.size(); ++j)
		{
			if (xs[j].t < xs[i].t)
			{
				Itr aux;
				aux.t = xs[i].t;
				aux.object = xs[i].object;

				xs[i].t = xs[j].t;
				xs[i].object = xs[j].object;

				xs[j].t = aux.t;
				xs[j].object = aux.object;
			}
		}
	}
}

//Checks if a ray intersects with anything on the specified world
vector<Itr> intersect_world(World world, myRay ray)
{
	vector<Itr> xs;
	float minimum_t = INFINITY;
	for (int i = 0; i < world.objects.size(); ++i)
	{
		//Check for intersection
		
		vector<Itr> auxXS = ray.intersect(world.objects[i]);

		if (auxXS[0].t != NULL) //Dont enter interaction if didnt interact
		{
			xs.push_back(auxXS[0]);
			xs.push_back(auxXS[1]);
		}
		
	}
	
	//Sort result intersections in ascending order
	//sort(xs.begin(), xs.end(), comparisonItr);
	bubbleSort(xs);

	return xs;
}

//Color of the intersection encapsulated in the given world
Color shade_hit(World world, ItrComps comps)
{
	bool shadowed = is_shadowed(world, comps.over_point);

	return lighting(comps.object.material, world._light, comps.point, comps.eye, comps.normal, shadowed);
}

//Returns color of intersection by a ray in the given world
Color color_at(World world, myRay ray)
{
	//Find intersections
	vector<Itr> its = intersect_world(world, ray);

	
	//Find the closest, real hit inside the intersections
	Itr i = hit(its);
	
	//Return black if there is no hit
	if (i.t == NULL)
		return Color(0, 0, 0);
	else
	{
		//Precompute extra intersection values
		ItrComps comps = prepare_computations(i, ray);

		//Color the hit
		return shade_hit(world, comps);
	}
}

//Check if a point is in shadows
bool is_shadowed(World world, myPoint point)
{
	//Compute distance from point to light
	myVector point_to_light = world._light.position - point;
	float distance = point_to_light.magnitude();

	//Create a ray normalized from point to light
	point_to_light = point_to_light.normalize();
	myRay ray = myRay(point, point_to_light);

	//Intersect world with ray
	vector<Itr> its = intersect_world(world, ray);

	//Check if there was an intersection
	Itr i = hit(its);

	if (i.t != NULL && i.t < distance)
		return true;
	else
		return false;
	
}




