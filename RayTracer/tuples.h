#pragma once
#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

# define PI 3.14159265358979323846

class Tuple
{

public:

	//Constructors
	Tuple() : x(0.0f), y(0.0f), z(0.0f), w(0){}
	Tuple(float newX, float newY, float newZ, int newW): x(newX), y(newY), z(newZ), w(newW) {}

	//Operations
	//Output
	friend ostream& operator<<(ostream& out, const Tuple& tuple);

	//Assign operator
	void operator=(const Tuple& tuple);

	//Comparison operators
	bool operator==(const Tuple& tuple);
	bool operator!=(const Tuple& tuple);

	//Addition
	Tuple operator+(const Tuple& tuple);

	//Susbtraction
	Tuple operator-(const Tuple& tuple);

	//Negative
	Tuple operator-();

	//Scalar multiplication and division (scalar left side)
	template<typename T>
	Tuple operator*(T const& scalar)
	{
		//Cant use template in cpp so I implement it here
		Tuple result(x * scalar, y * scalar, z * scalar, w * scalar);
		return result;
	}

	template<typename T>
	Tuple operator/(T const& scalar)
	{
		//Cant use template in cpp so I implement it here
		Tuple result(x / scalar, y / scalar, z / scalar, w / scalar);
		return result;
	}

	//Magnitude and normalize
	float magnitude();
	Tuple normalize();

	
	
	//Attributes
	float x;
	float y;
	float z;
	int w; //1 = point / 0 = vector
};

//Non member functions
//Scalar multiplication and division (scalar right side)
template<typename T>
Tuple operator*(T const& scalar, Tuple tuple)
{
	Tuple result(tuple.x * scalar, tuple.y * scalar, tuple.z * scalar, tuple.w * scalar);
	return result;
}

template<typename T>
Tuple operator/(T const& scalar, Tuple tuple)
{
	Tuple result(tuple.x / scalar, tuple.y / scalar, tuple.z / scalar, tuple.w / scalar);
	return result;
}

//Dot product
float dot(Tuple a, Tuple b);

//Cross product
Tuple cross(Tuple a, Tuple b);

//Equal floating point numbers
bool equalFloat(float a, float b);

//Convert degrees to radians
float deg2rad(float degrees);


//------------------------Subclass Vector------------------------
class myVector : public Tuple
{
public:
	//Constructors
	myVector()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0;
	}
	myVector(float newX, float newY, float newZ)
	{
		x = newX;
		y = newY;
		z = newZ;
		w = 0;
	}

	//Assign operator
	void operator=(const Tuple& tuple);
	

	//Operations
	//Addition
	myVector operator+(const myVector& tuple);
	//Substraction
	myVector operator-(const myVector& tuple);

};
//Non member functions
myVector cross(myVector a, myVector b);

//------------------------Subclass Point------------------------
class myPoint : public Tuple
{
public:
	//Constructors
	myPoint()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1;
	}
	myPoint(float newX, float newY, float newZ)
	{
		x = newX;
		y = newY;
		z = newZ;
		w = 1;
	}

	//Assign operator
	void operator=(const Tuple& tuple);

	//Operations

	//Substraction
	myVector operator-(const myPoint& point); //Substraction of two points = vector
	myPoint operator-(const myVector& vector); //Substraction of a vector from point = point

};

//------------------------Subclass Color------------------------
class Color : public Tuple
{
	
public:
	//Constructor: w  = 1000 temporal solution for differenciating color from vectors and points
	Color()
		: Tuple(0.0f, 0.0f, 0.0f, 1000), r(x), g(y), b(z)
	{};
	Color(float Red, float Green, float Blue)
		: Tuple(Red, Green, Blue, 1000), r(x), g(y), b(z)
	{};

	//Assign operator
	void operator=(const Color& tuple);
	void operator=(const Tuple& tuple);

	//Addition
	Color operator+(const Color& color);
	//Substraction
	Color operator-(const Color& color);
	//Multiplication
	Color operator*(const Color& color);
	//Scalar multiplication and division (scalar left side)
	template<typename T>
	Color operator*(T const& scalar)
	{
		//Cant use template in cpp so I implement it here
		Color result(x * scalar, y * scalar, z * scalar);
		return result;
	}

	//Hadamard
	Color hadamard_product(Color c1, Color c2);

	float& r;
	float& g;
	float& b;

};

//Non member functions
//Scalar multiplication and division (scalar right side)
template<typename T>
Color operator*(T const& scalar, Color color)
{
	Tuple result(color.x * scalar, color.y * scalar, color.z * scalar, color.w * scalar);
	return result;
}
