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

	bool operator==(const Shape& rhs);

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

class Cube : public Shape
{
public:
	Cube()
	{
		id = ShapeId;
		transform = I_Matrix();
		mat = material();
		//Unique value will alwyas be the next to the last Id
		ShapeId++;
	}

	//Intersects ray with a Cube
	vector<float> intersectionT(myRay ray);

	//Return normal vector of given point on a Cube
	myVector local_normal_at(myPoint localPoint);
};

class Cylinder : public Shape
{
public:
	Cylinder()
	{
		id = ShapeId;
		transform = I_Matrix();
		mat = material();
		//Unique value will alwyas be the next to the last Id
		ShapeId++;

		//Truncate cylinder
		minimum = -INFINITY;
		maximum = INFINITY;

		//Cylinder is open by default
		closed = false;
	}

	//Intersects ray with a Cylinder
	vector<float> intersectionT(myRay ray);

	//Return normal vector of given point on a Cylinder
	myVector local_normal_at(myPoint localPoint);

	//Insert in xs intersection with caps if cylinder is closed
	void intersect_caps(myRay ray, vector<float>* xs);

	float minimum;
	float maximum;
	bool closed;
};

class Cone : public Shape
{
public:
	Cone()
	{
		id = ShapeId;
		transform = I_Matrix();
		mat = material();
		//Unique value will alwyas be the next to the last Id
		ShapeId++;

		//Truncate cylinder
		minimum = -INFINITY;
		maximum = INFINITY;

		//Cylinder is open by default
		closed = false;
	}

	//Intersects ray with a Cone
	vector<float> intersectionT(myRay ray);

	//Return normal vector of given point on a Cone
	myVector local_normal_at(myPoint localPoint);

	//Insert in xs intersection with caps if cylinder is closed
	void intersect_caps(myRay ray, vector<float>* xs);

	float minimum;
	float maximum;
	bool closed;
};

class Triangle : public Shape
{
public:
	Triangle(myPoint p1, myPoint p2, myPoint p3) : p1(p1), p2(p2), p3(p3)
	{
		id = ShapeId;
		transform = I_Matrix();
		mat = material();
		//Unique value will alwyas be the next to the last Id
		ShapeId++;

		//Triangle edges
		e1 = p2 - p1;
		e2 = p3 - p1;

		//Triangle normal
		normal = cross(e2, e1).normalize();
	}

	//Intersects ray with a triangle (Möller–Trumbore algorithm)
	vector<float> intersectionT(myRay ray);

	//Return normal vector of given point on a triangle
	myVector local_normal_at(myPoint localPoint);

	myPoint p1, p2, p3;
	myVector e1, e2;
	myVector normal;
};

//Operations Sphere
bool operator==(const Shape& lhs, const Shape& rhs);



