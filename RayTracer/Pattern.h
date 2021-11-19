#pragma once
#include "MyMatrix.h"

class Pattern
{
public:
	Pattern()
	{
		a = Color(1, 1, 1);
		b = Color(0.8, 0, 0);
		transform = I_Matrix();
	}

	virtual Color pattern_at(myPoint patternPoint)
	{
		cout << "No pattern defined" << endl;
		return Color(0,0,0);
	}

	Color a;
	Color b;
	MyMatrix transform;
};

class Stripe : public Pattern
{
public:
	//Inherit constructor
	Stripe() : Pattern() {}

	//Return color of a stripe pattern in a localPoint
	Color pattern_at(myPoint patternPoint);
};

class Gradient : public Pattern
{
public:
	//Inherit constructor
	Gradient() : Pattern() {}

	//Return color of a gradient pattern in a localPoint
	Color pattern_at(myPoint patternPoint);
};

class Ring : public Pattern
{
public:
	//Inherit constructor
	Ring() : Pattern() {}

	//Return color of a ring pattern in a localPoint
	Color pattern_at(myPoint patternPoint);
};

class Checker : public Pattern
{
public:
	//Inherit constructor
	Checker() : Pattern() {}

	//Return color of a 3d checker pattern in a localPoint
	Color pattern_at(myPoint patternPoint);
};