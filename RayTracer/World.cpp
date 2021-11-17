#include "World.h"


//Non-member functions
bool operator==(const Itr& lhs, const Itr& rhs)
{
	return lhs.t == rhs.t && lhs.object == rhs.object;
}

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

//Intersect ray with a shape
vector<Itr> intersect(myRay ray, Shape* object)
{
	vector<float> t;
	//intersectionT will change along with object
	t = object->intersectionT(ray);

	vector<Itr> xs;
	Itr it1, it2;
	it1.t = t[0]; it2.t = t[1];
	it1.object = object; it2.object = object;

	xs.push_back(it1);
	xs.push_back(it2);

	return xs;
}

//Generate an array of interactions
vector<Itr> intersections(Itr it1, Itr it2)
{
	vector<Itr> xs;

	xs.push_back(it1);
	xs.push_back(it2);

	return xs;
}

//Returns the closest valid intersection
Itr hit(vector<Itr> xs)
{

	int id = -1;
	float value = INFINITY;

	for (int i = 0; i < xs.size(); ++i)
	{
		if (xs[i].t > 0 && xs[i].t < value)
		{
			id = i;
			value = xs[i].t;
		}
	}

	if (id == -1)
	{
		Itr i;
		i.t = NULL;
		return i;
	}
	else
		return xs[id];

}


//Returns color at a position
Color lighting(Material m, Light l, myPoint p, myVector eye, myVector normal, bool inShadow)
{
	Color ambientColor, diffuseColor, specularColor;

	//Combine surface color with light intensity
	Color effective_color = m.color * l.intensity;

	//Find direction to light source from point being iluminated
	myVector light_dir = l.position - p;
	light_dir = light_dir.normalize();

	//Ambient contribution
	ambientColor = effective_color * m.ambient;

	//Cosine of angle between ligth dir and normal vector
	float light_dot_normal = dot(light_dir, normal);


	if (light_dot_normal < 0 || inShadow) //Light on other side of surface
	{
		diffuseColor = Color(0, 0, 0);
		specularColor = Color(0, 0, 0);
	}
	else
	{
		//Diffuse contribution
		diffuseColor = effective_color * m.diffuse * light_dot_normal;

		//Cosine of angle between reflect vector and eye vector
		myVector r = reflect(-light_dir, normal);
		float reflect_dot_eye = dot(r, eye);

		if (reflect_dot_eye <= 0) //Light reflects away from eye
			specularColor = Color(0, 0, 0);
		else
		{
			//Specular contribution
			float factor = pow(reflect_dot_eye, m.shininess);
			specularColor = l.intensity * m.specular * factor;
		}
	}

	return ambientColor + diffuseColor + specularColor;
}


//Checks if a ray intersects with anything on the specified world
vector<Itr> intersect_world(World world, myRay ray)
{
	vector<Itr> xs;
	float minimum_t = INFINITY;
	for (int i = 0; i < world.objects.size(); ++i)
	{
		//Check for intersection
		Shape *object = world.objects[i];
		vector<Itr> auxXS = intersect(ray, object);

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

//Precompute computation structure with intersection info
ItrComps prepare_computations(Itr intersection, myRay ray)
{
	//Data structure
	ItrComps comps;

	//Intersection properties
	comps.t = intersection.t;
	comps.object = intersection.object;

	//Usefull values
	comps.point = ray.position(intersection.t);
	comps.eye = -ray.direction;

	//we have to make sure to change the normal if the eye is inside the object
	comps.normal = comps.object->normal_at(comps.point);

	if (dot(comps.normal, comps.eye) < 0)
	{
		comps.inside = true;
		comps.normal = -comps.normal;
	}
	else
		comps.inside = false;

	//Compute a slightly offset along normal point
	//This is done to avoid floating point number errors when detecting if a point is shadowed
	float epsilon = 0.1;
	comps.over_point = comps.point + comps.normal * epsilon;

	return comps;
}

//Color of the intersection encapsulated in the given world
Color shade_hit(World world, ItrComps comps)
{
	bool shadowed = is_shadowed(world, comps.over_point);

	return lighting(comps.object->mat, world._light, comps.point, comps.eye, comps.normal, shadowed);
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




