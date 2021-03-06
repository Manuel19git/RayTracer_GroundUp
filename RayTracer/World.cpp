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

//Checks if a shape is inside a vector of shapes
bool contains(vector<Shape*>* objects, Shape* object)
{
	Shape o;
	//We have to pass objects and object by reference
	for (int i = 0; i < objects->size(); ++i)
	{
		o = *objects->at(i);
		
		if (o == *object)
		{
			//We eliminate if an element is found
			objects->erase(next(objects->begin(), i));
			return true;
		}
			
	}
	
	objects->push_back(object);
	return false;
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

//Returns normal of a point in a shape in world coordinates
myVector normal_at(myPoint worldPoint, Shape* object)
{
	myPoint localPoint;
	myVector objectNormal, worldNormal;

	//Transform world coord to object coord
	MyMatrix inv = inverse(object->transform);
	localPoint = inv * worldPoint;

	//Calculate object normal
	objectNormal = object->local_normal_at(localPoint);

	//Transform object normal to world normal
	MyMatrix trans = transpose(inv);
	Tuple worldNormalTuple = trans * objectNormal;
	//world normal w value to zero. Avoids problems later
	worldNormalTuple.w = 0;
	worldNormal = worldNormalTuple;

	//Delete matrices
	trans.remove();
	inv.remove();

	//Normalize result normal vector
	worldNormal = worldNormal.normalize();

	return worldNormal;
}

//Returns color based on pattern of a shape and its transform
Color pattern_at_shape(Pattern* pattern, Shape* object, myPoint worldPoint)
{
	
	MyMatrix invObject = inverse(object->transform);
	MyMatrix invPattern = inverse(pattern->transform);

	myPoint localPoint, patternPoint;

	//transform point to local point of object
	localPoint = invObject * worldPoint;

	//transform localPoint to pattern point
	patternPoint = invPattern * localPoint;

	//Delete matrices
	invObject.remove();
	invPattern.remove();
	
	return pattern->pattern_at(patternPoint);
}


//Returns color at a position
Color lighting(Shape* object, Light l, myPoint p, myVector eye, myVector normal, bool inShadow)
{
	Color objectColor, ambientColor, diffuseColor, specularColor;
	Material m = object->mat;

	//Check if an object has a pattern to be displayed
	if (object->mat.showPattern)
		objectColor = pattern_at_shape(m.pattern, object, p);
	else
		objectColor = m.color;
	
	//Combine surface color with light intensity
	Color effective_color = objectColor * l.intensity;

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
ItrComps prepare_computations(Itr intersection, myRay ray, vector<Itr> xs)
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
	comps.normal = normal_at(comps.point, comps.object);

	if (dot(comps.normal, comps.eye) < 0)
	{
		comps.inside = true;
		comps.normal = -comps.normal;
	}
	else
		comps.inside = false;

	//Compute a slightly offset along normal point
	//This is done to avoid floating point number errors when detecting if a point is shadowed
	float epsilon = 0.01; //Depending on this number, there could be artifacts in the scene
	comps.over_point = comps.point + comps.normal * epsilon;

	//Reflect vector
	comps.reflectV = reflect(ray.direction, comps.normal);

	//Refraction
	//Calculating refraction indices
	vector<Shape*> container;
	for (Itr i : xs)
	{
		if (i == intersection)
		{
			if (container.empty())
				comps.n1 = 1;
			else
				comps.n1 = container.back()->mat.refractive_index;
		}
		
		//Check if the container has the object of the intersection
		//If true, it erases it
		//If false, it inserts it
		contains(&container, i.object);

		if (i == intersection)
		{
			if (container.empty())
				comps.n2 = 1;
			else
				comps.n2 = container.back()->mat.refractive_index;

			//No need to continue the loop
			break;
		}
	}
	//The same as over point, we need to make sure that to calculate refraction,
	//the intersection point starts inside the object
	//(If I dont cast type to a tuple the substraction does not work)
	
	comps.under_point = (Tuple)comps.point - comps.normal * epsilon;

	return comps;
}


//Returns color of intersection by a ray in the given world
Color color_at(World world, myRay ray, int remaining)
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
		ItrComps comps = prepare_computations(i, ray, its);

		//Color the hit
		return shade_hit(world, comps, remaining);
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

//Color of the intersection encapsulated in the given world
Color shade_hit(World world, ItrComps comps, int remaining)
{
	bool shadowed = is_shadowed(world, comps.over_point);
	
	Color surface = lighting(comps.object, world._light, comps.point, comps.eye, comps.normal, shadowed);
	Color reflect = reflected_color(world, comps, remaining);
	Color refract = refracted_color(world, comps, remaining);

	Material material = comps.object->mat;

	if (material.reflective > 0 && material.transparency > 0)
	{
		float reflectance = schlick(comps);
		return surface + reflect * reflectance + refract * (1 - reflectance);
	}
	else
		return surface + reflect + refract;

	
}

//Return the color of a point given its reflectance value
Color reflected_color(World world, ItrComps comps, int remaining)
{
	Color reflectColor;
	//If no reflectance, returns black
	if (comps.object->mat.reflective == 0 || remaining <= 0)
		return Color(0, 0, 0);
	else
	{
		myRay reflect_ray = myRay(comps.over_point, comps.reflectV);
		reflectColor = color_at(world, reflect_ray, remaining - 1);
	}

	return reflectColor * comps.object->mat.reflective;
}

//Returns the color of a point in a refractive object
Color refracted_color(World world, ItrComps comps, int remaining)
{
	if (comps.object->mat.transparency == 0 || remaining <= 0)
		return Color(0, 0, 0);
	else
	{
		//Total internal reflection (Snells Law)
		//First find ratio of refractive indexes
		float n_ratio = comps.n1 / comps.n2;

		//cos(theta_i) of incident ray is the dot product between ray with noraml
		float cos_i = dot(comps.eye, comps.normal);

		//Find sin(theta_t)^2 with trigonometry identity
		float sin2_t = pow(n_ratio, 2) * (1 - pow(cos_i, 2));

		if (sin2_t > 1) //Total internal reflection, so we return black color
			return Color(0, 0, 0);
		else //Compute refraction
		{
			//Find cos(theta_t) via trigonometric identity​
			float cos_t = sqrt(1.0 - sin2_t);

			//Compute direction of refracted ray
			myVector direction;
			direction = comps.normal * (n_ratio * cos_i - cos_t) - comps.eye * n_ratio;

			//Refracted ray
			myRay refractRay = myRay(comps.under_point, direction);

			return color_at(world, refractRay, remaining - 1) * comps.object->mat.transparency;
		}
	}
}

//Returns a number called reflectance, which is the fraction of light being reflected
float schlick(ItrComps comps)
{
	//Cosine of angle between eye and normal
	float cos = dot(comps.eye, comps.normal);

	//Total internal reflection can only occur when n1 > n2
	if (comps.n1 > comps.n2)
	{
		float n_ratio = comps.n1 / comps.n2;
		float sin2_t = pow(n_ratio, 2) * (1 - pow(cos, 2));

		if (sin2_t > 1)
			return 1.0;

		//Find cos(theta_t) via trigonometric identity​
		float cos_t = sqrt(1.0 - sin2_t);

		//When n1 > n2 use cos(theta_t) instead of cos
		cos = cos_t;

	}
	
	float r0 = pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2);
	return r0 + (1 - r0) * pow(1 - cos, 5);
}



