#include "World.h"


//Non-member functions

//When comparing intersections we only look to the t value
bool comparisonItr(Itr a, Itr b)
{
	return a.t < b.t;
}

//Checks if a ray intersects with anything on the specified world
vector<Itr> intersect_world(World w, myRay r)
{
	vector<Itr> xs;
	float minimum_t = INFINITY;
	for (int i = 0; i < w.objects.size(); ++i)
	{
		//Check for intersection
		vector<Itr> auxXS = r.intersect(w.objects[i]);
		xs.push_back(auxXS[0]);
		xs.push_back(auxXS[1]);
	}
	//Sort result intersections in ascending order
	sort(xs.begin(), xs.end(), comparisonItr);

	return xs;
}

