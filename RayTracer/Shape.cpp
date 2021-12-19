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
bool Shape::operator==(const Shape& rhs)
{
	MyMatrix A = transform;
	MyMatrix B = rhs.transform;

	return A == B && id == rhs.id;
}


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
myVector Sphere::local_normal_at(myPoint localPoint)
{
	//Calculate normal of object (center of sphere is zero for now)
	return localPoint - myPoint(0, 0, 0);
}

//--------------Surface--------------
vector<float> Plane::intersectionT(myRay ray)
{
	MyMatrix inv = inverse(transform);
	myRay invRay = ray.transform(inv);

	vector<float> t;

	if (abs(invRay.direction.y) < 0.01)
	{
		t.push_back(NULL);
		t.push_back(NULL);
	}
	else
	{
		float value = -invRay.origin.y / invRay.direction.y;
		
		t.push_back(value);
		t.push_back(value);
	}

	return t;
}

//Return normal vector of given point on a XZ plane
myVector Plane::local_normal_at(myPoint localPoint)
{
	return myVector(0,1,0);
}

//--------------Cube--------------
float* check_axis(float origin, float direction)
{
	float* t = new float[2];

	float tmin_numerator = -1 - origin;
	float tmax_numerator = 1 - origin;

	if (abs(direction) >= FLT_EPSILON)
	{
		t[0] = tmin_numerator / direction; //tmin
		t[1] = tmax_numerator / direction; //tmax
	}
	else
	{
		t[0] = tmin_numerator * INFINITY; //tmin
		t[1] = tmax_numerator * INFINITY; //tmax
	}

	//Swap in case of tmin(t[0]) being greater that tmax(t[1])
	
	if (t[0] > t[1])
	{
		float tAux = t[0];
		t[0] = t[1];
		t[1] = tAux;
	}
	

	return t;
}

//Intersects ray with a Cube
vector<float> Cube::intersectionT(myRay ray)
{
	MyMatrix inv = inverse(transform);
	myRay invRay = ray.transform(inv);

	vector<float> t;
	float* xt;
	float* yt;
	float* zt;

	xt = check_axis(invRay.origin.x, invRay.direction.x);
	yt = check_axis(invRay.origin.y, invRay.direction.y);
	zt = check_axis(invRay.origin.z, invRay.direction.z);
	
	//The largest minimum of square 
	float tmin = max({ xt[0], yt[0], zt[0] });

	//The smallest maximum of square 
	float tmax = min({ xt[1], yt[1], zt[1] });

	//In case tmin is greater than tmax, the ray misses the cube
	if (tmin > tmax)
	{
		t.push_back(NULL);
		t.push_back(NULL);
	}
	else
	{
		t.push_back(tmin);
		t.push_back(tmax);
	}

	return t;

}

//Return normal vector of given point on a Cube
myVector Cube::local_normal_at(myPoint localPoint)
{

	//Compute max component (if point is 1 or close, we will have the normal!)
	float maxc = max({ abs(localPoint.x), abs(localPoint.y), abs(localPoint.z) });

	if (maxc == abs(localPoint.x)) return myVector(localPoint.x, 0, 0);
	else if (maxc == abs(localPoint.y)) return myVector(0, localPoint.y, 0);
	else if (maxc == abs(localPoint.z)) return myVector(0, 0, localPoint.z);
	else return myVector(0, 1, 0);

}

//--------------Cylinder--------------
//Helper function to reduce duplication. 
bool check_cap_cylinder(myRay ray, float t)
{
	float x = ray.origin.x + t * ray.direction.x;
	float z = ray.origin.z + t * ray.direction.z;

	return (pow(x, 2) + pow(z, 2)) <= 1;
}

//Insert in xs intersection with caps if cylinder is closed
void Cylinder::intersect_caps(myRay ray, vector<float>* xs)
{
	//Compute intersection with caps if cylinder is closed
	if (closed)
	{
		//Check intersection with lower cap
		float t = (minimum - ray.origin.y) / ray.direction.y;
		if (check_cap_cylinder(ray, t))
			xs->push_back(t);

		//Check intersection with upper cap
		t = (maximum - ray.origin.y) / ray.direction.y;
		if (check_cap_cylinder(ray, t))
			xs->push_back(t);
	}

}

//Intersects ray with a Cylinder
vector<float> Cylinder::intersectionT(myRay ray)
{
	MyMatrix inv = inverse(transform);
	myRay invRay = ray.transform(inv);

	vector<float> t;

	float a = pow(invRay.direction.x, 2) + pow(invRay.direction.z, 2);

	//Ray parallel to the y axis if 'a' is close to zero
	if (a > FLT_EPSILON)
	{

		float b = 2 * invRay.origin.x * invRay.direction.x +
			2 * invRay.origin.z * invRay.direction.z;

		float c = pow(invRay.origin.x, 2) + pow(invRay.origin.z, 2) - 1;

		float discriminant = pow(b, 2) - 4 * a * c;

		//Ray does not intersect with cylinder
		if (discriminant < 0)
		{
			t.push_back(NULL);
			t.push_back(NULL);
			return t;
		}

		float t0 = (-b - sqrt(discriminant)) / (2 * a);
		float t1 = (-b + sqrt(discriminant)) / (2 * a);

		//Swap so that t0 is always smaller than t1
		if (t0 > t1)
		{
			float tAux = t0;
			t0 = t1;
			t1 = tAux;
		}

		float y0 = invRay.origin.y + t0 * invRay.direction.y;
		if (minimum < y0 && y0 < maximum)
			t.push_back(t0);

		float y1 = invRay.origin.y + t1 * invRay.direction.y;
		if (minimum < y1 && y1 < maximum)
			t.push_back(t1);

	}

	if(a > 0)
		intersect_caps(invRay, &t);
	
	if (t.size() == 1)
	{
		t.push_back(t[t.size() - 1]);
	}
	if (t.size() == 0)
	{
		t.push_back(NULL);
		t.push_back(NULL);
	}
	return t;

}

//Return normal vector of given point on a Cylinder
myVector Cylinder::local_normal_at(myPoint localPoint)
{
	//Square distance from y axis
	float dist = pow(localPoint.x, 2) + pow(localPoint.z, 2);

	if (dist < 1 && localPoint.y >= maximum - 0.01)
		return myVector(0, 1, 0);

	else if (dist < 1 && localPoint.y <= minimum + 0.01)
		return myVector(0, -1, 0);

	else
		return myVector(localPoint.x, 0, localPoint.z);
}

//----------------Cone----------------
//Helper function to reduce duplication. 
bool check_cap_cone(myRay ray, float t)
{
	float x = ray.origin.x + t * ray.direction.x;
	float y = ray.origin.y + t * ray.direction.y;
	float z = ray.origin.z + t * ray.direction.z;

	return (pow(x, 2) + pow(z, 2)) <= y;
}

//Insert in xs intersection with caps if cone is closed
void Cone::intersect_caps(myRay ray, vector<float>* xs)
{
	//Compute intersection with caps if cylinder is closed
	if (closed)
	{
		//Check intersection with lower cap
		float t = (minimum - ray.origin.y) / ray.direction.y;
		if (check_cap_cone(ray, t))
			xs->push_back(t);

		//Check intersection with upper cap
		t = (maximum - ray.origin.y) / ray.direction.y;
		if (check_cap_cone(ray, t))
			xs->push_back(t);
	}

}


//Intersects ray with a Cone
vector<float> Cone::intersectionT(myRay ray)
{
	MyMatrix inv = inverse(transform);
	myRay invRay = ray.transform(inv);

	vector<float> t;

	float a = pow(invRay.direction.x, 2) - pow(invRay.direction.y, 2) + pow(invRay.direction.z, 2);

	float b = (2 * invRay.origin.x * invRay.direction.x) -
		(2 * invRay.origin.y * invRay.direction.y) +
		(2 * invRay.origin.z * invRay.direction.z);

	float c = pow(invRay.origin.x, 2) - pow(invRay.origin.y, 2) + pow(invRay.origin.z, 2);

	//Ray parallel to the y axis if 'a' is close to zero
	if ((a > FLT_EPSILON || a < -FLT_EPSILON) && (b > FLT_EPSILON || b < -FLT_EPSILON))
	{
		float discriminant = pow(b, 2) - 4 * a * c;

		//Ray does not intersect with cylinder
		if (discriminant < 0)
		{
			t.push_back(NULL);
			t.push_back(NULL);
			return t;
		}

		float t0 = (-b - sqrt(discriminant)) / (2 * a);
		float t1 = (-b + sqrt(discriminant)) / (2 * a);

		//Swap so that t0 is always smaller than t1
		if (t0 > t1)
		{
			float tAux = t0;
			t0 = t1;
			t1 = tAux;
		}
		
		float y0 = invRay.origin.y + t0 * invRay.direction.y;
		if (minimum < y0 && y0 < maximum)
			t.push_back(t0);

		float y1 = invRay.origin.y + t1 * invRay.direction.y;
		if (minimum < y1 && y1 < maximum)
			t.push_back(t1);

		
	}
	else if (a > -FLT_EPSILON && a < FLT_EPSILON && (b > FLT_EPSILON || b < -FLT_EPSILON))
	{
		t.push_back(-c / (2 * b));
	}

	
	if (a > 0)
		intersect_caps(invRay, &t);

	if (t.size() == 1)
	{
		t.push_back(t[t.size() - 1]);
	}
	if (t.size() == 0)
	{
		t.push_back(NULL);
		t.push_back(NULL);
	}
	return t;

}

//Return normal vector of given point on a Cone
myVector Cone::local_normal_at(myPoint localPoint)
{
	//Square distance from y axis
	float dist = pow(localPoint.x, 2) + pow(localPoint.z, 2);
	float y = (localPoint.y > 0) ? -sqrt(dist) : sqrt(dist);

	if (dist < 1 && localPoint.y >= maximum - 0.01)
		return myVector(0, 1, 0);

	else if (dist < 1 && localPoint.y <= minimum + 0.01)
		return myVector(0, -1, 0);

	else
		return myVector(localPoint.x, y, localPoint.z);
}


//--------------Triangle--------------
//Intersects ray with a triangle (Möller–Trumbore algorithm)
vector<float> Triangle::intersectionT(myRay ray)
{
	MyMatrix inv = inverse(transform);
	myRay invRay = ray.transform(inv);

	myVector dir_cross_e2 = cross(invRay.direction, e2);
	float determinant = dot(e1, dir_cross_e2);

	vector<float> t;

	//If ray parallel to triangle return an empty list
	if (abs(determinant) < FLT_EPSILON) return t;

	
	float f = 1 / determinant;

	myVector p1_to_origin = invRay.origin - p1;
	float u = f * dot(p1_to_origin, dir_cross_e2);
	//If ray misses over p1-p3 edge
	if (u < 0 || u > 1)
	{
		t.push_back(NULL);
		t.push_back(NULL);
		return t;
	}

	myVector origin_cross_e1 = cross(p1_to_origin, e1);
	float v = f * dot(invRay.direction, origin_cross_e1);
	//If ray misses over p1-p2 and p2-p3 edges
	if (v < 0 || (u + v) > 1)
	{
		t.push_back(NULL);
		t.push_back(NULL);
		return t;
	}

	float tValue = f * dot(e2, origin_cross_e1);
	//If ray hits triangle
	t.push_back(tValue);
	t.push_back(tValue);

	return t;
}


//Return precomputed normal vector of triangle
myVector Triangle::local_normal_at(myPoint localPoint)
{
	return normal;
}