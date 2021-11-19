#pragma once
#include "Pattern.h"
#include <list>

//------------------Material------------------
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

	bool showPattern;
	Pattern* pattern;
};



//Point light
struct Light
{
	myPoint position;
	Color intensity;
};


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

	//Returns a new ray with transformation matrix applied
	myRay transform(MyMatrix matrix);

	myPoint origin;
	myVector direction;
};

//Non-member functions for structures
Material material();

//Creates a default light
Light light();

//Reflects the in vector using normal as reference
myVector reflect(myVector in, myVector normal);










