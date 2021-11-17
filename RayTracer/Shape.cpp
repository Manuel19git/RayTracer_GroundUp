#include "Shape.h"

//Struct operations
int ShapeId = 0;

bool operator==(const Shape& lhs, const Shape& rhs)
{
	MyMatrix A = lhs.transform;
	MyMatrix B = rhs.transform;

	return A == B && lhs.id == rhs.id;
}



//--------------Shape--------------


//--------------Sphere--------------
//Intersects ray with sphere
vector<float> Sphere::intersectionT(myRay ray)
{
	MyMatrix inv = inverse(transform);
	myRay invRay = ray.transform(inv);
	//Itr xs[2] ----> Result ERROR(local - data segment or stack)
	//Itr *xs = new Itr[2] ------> Result OK (dynamically allocated memory - heap)
	vector<float> t;


	//Calculate sphere to ray vector
	myVector sphere2Ray = invRay.origin - myPoint(0, 0, 0);
	float a = dot(invRay.direction, invRay.direction);
	float b = 2 * dot(invRay.direction, sphere2Ray);
	float c = dot(sphere2Ray, sphere2Ray) - 1;

	float discriminant = pow(b, 2) - 4 * a * c;

	if (discriminant < 0)
	{
		t.push_back(NULL);
		t.push_back(NULL);
	}
	else
	{
		t.push_back((-b - sqrt(discriminant)) / (2 * a));
		t.push_back((-b + sqrt(discriminant)) / (2 * a));
	}

	inv.remove();
	return t;
}

//Return normal vector of given point on a sphere
myVector Sphere::normal_at(myPoint worldPoint)
{
	myPoint objectPoint;
	myVector objectNormal, worldNormal;
	//Transform world coord to object coord

	MyMatrix inv = inverse(transform);
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


