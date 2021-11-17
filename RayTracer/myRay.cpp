#include "myRay.h"




//Compute the position of a point at a time t
myPoint myRay::position(float t)
{
	myPoint p;
	p = origin + (direction * t);

	return p;
}


//Returns a new ray with transformation matrix applied
myRay myRay::transform(MyMatrix matrix)
{
	myRay newRay;

	newRay.origin = matrix * origin;
	newRay.direction = matrix * direction;

	return newRay;
}

//Non member functions for ray
//Creates a default material
Material material()
{
	Material m;
	m.color = Color(1, 1, 1);
	m.ambient = 0.1;
	m.diffuse = 0.9;
	m.specular = 0.9;
	m.shininess = 200.0;

	return m;
}

//Creates a default light
Light light()
{
	Light l;
	l.intensity = Color(1, 1, 1);
	l.position = myPoint(0, 0, 0);

	return l;
}


//Reflects the in vector using normal as reference
myVector reflect(myVector in, myVector normal)
{
	return in - (normal * 2 * dot(in, normal));
}





