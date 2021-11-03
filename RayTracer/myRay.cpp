#include "myRay.h"

int sphereId = 0;

//Struct operations
bool operator==(const Sphere& lhs, const Sphere& rhs)
{
	MyMatrix A = lhs.transform;
	MyMatrix B = rhs.transform;
	return lhs.id == rhs.id && A == B;
}
bool operator==(const Itr& lhs, const Itr& rhs)
{
	return lhs.t == rhs.t && lhs.object == rhs.object;
}


//Compute the position of a point at a time t
myPoint myRay::position(float t)
{
	myPoint p;
	p = origin + (direction * t);

	return p;
}

//Give values of intersection between ray and object
vector<Itr> myRay::intersect(Sphere& s)
{
	MyMatrix inv = inverse(s.transform);
	myRay invRay = this->transform(inv);
	//Itr xs[2] ----> Result ERROR(local - data segment or stack)
	//Itr *xs = new Itr[2] ------> Result OK (dynamically allocated memory - heap)
	vector<Itr> xs;
	Itr it1, it2;
	xs.push_back(it1);
	xs.push_back(it2);
	
	xs[0].object = s;
	xs[1].object = s;
	
	//Calculate sphere to ray vector
	myVector sphere2Ray = invRay.origin - myPoint(0, 0, 0);
	float a = dot(invRay.direction, invRay.direction);
	float b = 2 * dot(invRay.direction, sphere2Ray);
	float c = dot(sphere2Ray, sphere2Ray) - 1;

	float discriminant = pow(b, 2) - 4 * a * c;
	
	if (discriminant < 0)
	{
		xs[0].t = NULL;
		xs[1].t = NULL;
	}
	else
	{
		xs[0].t = (-b - sqrt(discriminant)) / (2 * a);
		xs[1].t = (-b + sqrt(discriminant)) / (2 * a);
	}

	inv.remove();
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
	

	comps.normal = normal_at(comps.object, comps.point);
		
	if (dot(comps.normal, comps.eye) < 0)
	{
		comps.inside = true;
		comps.normal = -comps.normal;
	}
	else
		comps.inside = false;

	return comps;
}


//Returns a new ray with transformation matrix applied
myRay myRay::transform(MyMatrix matrix)
{
	myRay newRay;

	newRay.origin = matrix * origin;
	newRay.direction = matrix * direction;

	return newRay;
}

//Non-member functions for structures
//Creates a default material
Material material()
{
	Material m;
	m.color = Color(1, 1, 1);
	m.ambient = 0.1; 
	m.diffuse = 0.9;
	m.specular = 0.9;
	m.shininess = 200.0;

	return m;
}

//Creates a default light
Light light()
{
	Light l;
	l.intensity = Color(1, 1, 1);
	l.position = myPoint(0, 0, 0);

	return l;
}

//Returns a sphere type object
Sphere sphere()
{
	Sphere s;
	s.id = sphereId;
	s.transform = I_Matrix();
	s.material = material();
	//Unique value will alwyas be the next to the last Id
	sphereId++;

	return s;
}

//Return normal vector of given point on an object
myVector normal_at(Sphere object, myPoint worldPoint)
{
	myPoint objectPoint;
	myVector objectNormal, worldNormal;
	//Transform world coord to object coord
	MyMatrix inv = inverse(object.transform);
	objectPoint = inv * worldPoint;

	//Calculate normal of object (center of sphere is zero for now)
	objectNormal = objectPoint - myPoint(0, 0, 0);

	
	//Transform object normal to world normal
	MyMatrix trans = transpose(inv);

	//First assing to tuple 
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

//Reflects the in vector using normal as reference
myVector reflect(myVector in, myVector normal)
{
	return in - (normal * 2 * dot(in, normal));
}

//Returns color at a position
Color lighting(Material m, Light l, myPoint p, myVector eye, myVector normal)
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

	
	if (light_dot_normal < 0) //Light on other side of surface
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

//Non-member functions for ray
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

