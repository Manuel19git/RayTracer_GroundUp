#pragma once

#include "MyMatrix.h";

class myRay
{
public:
	
	myRay()
	{
		origin = myPoint(0,0,0);
		direction = myVector(0, 0, 0);
	}
	myRay(myPoint o, myVector d) : origin(o), direction(d) {}

	myPoint origin;
	myVector direction;
};