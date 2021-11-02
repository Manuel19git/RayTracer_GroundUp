#include "World.h"


//Non-member functions


//When comparing intersections we only look to the t value
bool comparisonItr(Itr a, Itr b)
{
	return a.t < b.t;
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
		xs.push_back(auxXS[0]);
		xs.push_back(auxXS[1]);
	}
	//Sort result intersections in ascending order
	sort(xs.begin(), xs.end(), comparisonItr);

	return xs;
}

//Color of the intersection encapsulated in the given world
Color shade_hit(World world, ItrComps comps)
{
	return lighting(comps.object.material, world._light, comps.point, comps.eye, comps.normal);
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


