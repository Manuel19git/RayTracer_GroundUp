#pragma once
#include "myRay.h"

extern int ShapeId;



class Shape
{
public:
	virtual vector<float> intersectionT(myRay ray)
	{
		cout << "No shape defined to intersect" << endl;

		vector<float> empty;
		empty.push_back(NULL);
		empty.push_back(NULL);

		return empty;
	}

	virtual myVector local_normal_at(myPoint localPoint)
	{
		cout << "No shape defined to calculate normal" << endl;

		myVector empty;
		return empty;
	}

	int id;
	MyMatrix transform;
	Material mat;
};

class Sphere : public Shape
{
public:
	Sphere() 
	{
		id = ShapeId;
		transform = I_Matrix();
		mat = material();
		//Unique value will alwyas be the next to the last Id
		ShapeId++;
	}

	//Intersects ray with sphere
	vector<float> intersectionT(myRay ray);

	//Return normal vector of given point on a sphere
	myVector local_normal_at(myPoint localPoint);
};

class Plane : public Shape
{
public:
	Plane()
	{
		id = ShapeId;
		transform = I_Matrix();
		mat = material();
		//Unique value will alwyas be the next to the last Id
		ShapeId++;
	}

	//Intersects ray with a XZ plane
	vector<float> intersectionT(myRay ray);

	//Return normal vector of given point on a XZ plane
	myVector local_normal_at(myPoint localPoint);
};

//Operations Sphere
bool operator==(const Shape& lhs, const Shape& rhs);



