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

	virtual myVector normal_at(myPoint worldPoint)
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
	myVector normal_at(myPoint worldPoint);
};

//Operations Sphere
bool operator==(const Shape& lhs, const Shape& rhs);



