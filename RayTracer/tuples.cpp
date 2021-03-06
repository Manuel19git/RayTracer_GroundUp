#include <iostream>
#include "tuples.h"


using namespace std;


ostream& operator<<(ostream& out, const Tuple& tuple)
{
	if (tuple.w == 1)
		out << "Point (" << tuple.x << ", " << tuple.y << ", " << tuple.z << ")" << "\n";
	else if (tuple.w == 0)
		out << "Vector (" << tuple.x << ", " << tuple.y << ", " << tuple.z << ")" << "\n";
	else if (tuple.w == 1000)
		out << "Color (" << tuple.x << ", " << tuple.y << ", " << tuple.z << ")" << "\n";
	else
		out << "Tuple (" << tuple.x << ", " << tuple.y << ", " << tuple.z << ", " << tuple.w << ")" << "\n";

	return out;
}



//---------------Tuple--------------------
void Tuple::operator=(const Tuple& tuple)
{
	x = tuple.x;
	y = tuple.y;
	z = tuple.z;
	w = tuple.w;
}

bool Tuple::operator==(const Tuple& tuple)
{
	return (equalFloat(x, tuple.x) && equalFloat(y, tuple.y) && equalFloat(z, tuple.z) && equalFloat(w, tuple.w));
}
bool Tuple::operator!=(const Tuple& tuple)
{
	bool equal = *this == tuple;
	return !equal;
}

Tuple Tuple::operator+(const Tuple& otherTuple)
{
	Tuple result;


	result.x = x + otherTuple.x;
	result.y = y + otherTuple.y;
	result.z = z + otherTuple.z;
	result.w = w + otherTuple.w; //Adding points will make no sense

	return result;
}

Tuple Tuple::operator-(const Tuple& otherTuple)
{
	Tuple result;



	result.x = x - otherTuple.x;
	result.y = y - otherTuple.y;
	result.z = z - otherTuple.z;
	result.w = w - otherTuple.w; //Adding points will make no sense

	return result;
}

Tuple Tuple::operator-()
{
	Tuple result(-x, -y, -z, -w);
	return result;
}

float Tuple::magnitude()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
}
Tuple Tuple::normalize()
{
	float mag = magnitude();
	Tuple result = Tuple(x, y, z, w) / mag;

	return (magnitude() != 0) ? result : Tuple();
}

float dot(Tuple a, Tuple b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

Tuple cross(Tuple a, Tuple b)
{
	//operation is always done between vectors
	return Tuple(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
		0);
}

//Convert degrees to radians
float deg2rad(float degrees)
{

	return (degrees * PI / 180.0f);
}





//---------------myVector--------------------
void myVector::operator=(const Tuple& tuple)
{
	if (tuple.w == 0) //Its a vector
	{
		x = tuple.x;
		y = tuple.y;
		z = tuple.z;
		w = 0;
	}
	else
	{
		printf("Error: assign must be done with a vector type tuple \n");
	}

}

myVector myVector::operator+(const myVector& otherVector)
{
	myVector result;


	result.x = x + otherVector.x;
	result.y = y + otherVector.y;
	result.z = z + otherVector.z;

	return result;
}

myVector myVector::operator-(const myVector& vector)
{
	myVector result;

	result.x = x - vector.x;
	result.y = y - vector.y;
	result.z = z - vector.z;

	return result;
}
myVector myVector::operator-(const Tuple& tuple)
{
	myVector result;

	result.x = x - tuple.x;
	result.y = y - tuple.y;
	result.z = z - tuple.z;

	return result;
}

//Negative
myVector myVector::operator-()
{
	return myVector(-x, -y, -z);
}


myVector cross(myVector a, myVector b)
{
	//operation is always done between vectors
	return myVector(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

//---------------myPoint--------------------
void myPoint::operator=(const Tuple& tuple)
{
	if (tuple.w == 1) //Its a point
	{
		x = tuple.x;
		y = tuple.y;
		z = tuple.z;
		w = 1;

	}
	else
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
		printf("Error: assign must be done with a point type tuple \n");
		
	}



}

myVector myPoint::operator-(const myPoint& point)
{
	myVector result;

	result.x = x - point.x;
	result.y = y - point.y;
	result.z = z - point.z;

	return result;
}

myPoint myPoint::operator-(const myVector& vector)
{
	myPoint result;

	result.x = x - vector.x;
	result.y = y - vector.y;
	result.z = z - vector.z;

	return result;
}

//---------------Color--------------------
void Color::operator=(const Color& color)
{
	if (color.w == 1000) //Its a color
	{
		x = color.x;
		y = color.y;
		z = color.z;
	}
}

void Color::operator=(const Tuple& tuple)
{
	if (tuple.w == 1000) //Its a color
	{
		x = tuple.x;
		y = tuple.y;
		z = tuple.z;
	}
	else
	{
		printf("Error: assign must be done with a color type tuple \n");
	}

}


Color Color::operator+(const Color& otherColor)
{
	Color result;


	result.x = x + otherColor.x;
	result.y = y + otherColor.y;
	result.z = z + otherColor.z;

	return result;
}

Color Color::operator-(const Color& otherColor)
{
	Color result;


	result.x = x - otherColor.x;
	result.y = y - otherColor.y;
	result.z = z - otherColor.z;

	return result;
}

Color Color::operator*(const Color& otherColor)
{
	Color result;


	result.x = x * otherColor.x;
	result.y = y * otherColor.y;
	result.z = z * otherColor.z;

	return result;
}

Color Color::hadamard_product(Color c1, Color c2)
{
	Color result;


	result.x = c1.x * c2.x;
	result.y = c1.y * c2.y;
	result.z = c1.z * c2.z;

	return result;
}

bool equalFloat(float a, float b)
{
	const float epsilon = 0.0001;
	if (abs(a - b) < epsilon) return true;
	else return false;
}