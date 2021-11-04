#pragma once

#include "MyMatrix.h"
#include <list>

extern int sphereId;

//Material
struct Material
{
	//Color of the material
	Color color;
	//Ambient reflection [0,1]
	float ambient;
	//Diffuse reflection [0,1]
	float diffuse;
	//Specular reflection [0,1]
	float specular;
	//Shininess [10,200]
	float shininess;
};

//Point light
struct Light
{
	myPoint position;
	Color intensity;
};

//Objects
struct Sphere
{
	int id;
	MyMatrix transform = MyMatrix(4); //It doestn have default constructor
	Material material;
};

//Intersection info
struct Itr
{
	//t for a ray intersecting with an object
	float t;
	//object
	Sphere object;
};

//Computations (More Info about Intersections)
struct ItrComps
{
	//Intersection info
	float t;
	Sphere object;

	//Usefull values
	myPoint point;
	myPoint over_point;
	myVector eye;
	myVector normal;
	bool inside;
};

bool operator==(const Sphere& lhs, const Sphere& rhs);
bool operator==(const Itr& lhs, const Itr& rhs);

class myRay
{
public:
	myRay()
	{
		origin = myPoint(0, 0, 0);
		direction = myVector(0, 0, 0);
	}
	myRay(myPoint o, myVector d) : origin(o), direction(d) {}

	//Compute the position of a point at a time t
	myPoint position(float t);

	//Return intersection between ray and object
	vector<Itr> intersect(Sphere& s);

	//Returns a new ray with transformation matrix applied
	myRay transform(MyMatrix matrix);

	myPoint origin;
	myVector direction;
};

//Non-member functions for structures
//Creates a default material
Material material();

//Creates a default light
Light light();

//Returns a sphere structure 
Sphere sphere();

//Return normal vector of given point on an object
myVector normal_at(Sphere object, myPoint worldPoint);

//Reflects the in vector using normal as reference
myVector reflect(myVector in, myVector normal);

//Returns color at a position
Color lighting(Material m, Light l, myPoint p, myVector eye, myVector normal, bool inShadow);

//Non-member functions for ray
//Generate an array of interactions
vector<Itr> intersections(Itr it1, Itr it2);

//Returns the closest valid intersection
Itr hit(vector<Itr> xs);




