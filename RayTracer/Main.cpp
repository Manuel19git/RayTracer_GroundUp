#include <iostream>
#include <chrono>
#include <Eigen/Dense>
#include "Canvas.h"
#include "Camera.h"
#include "World.h"

using namespace std;
using namespace Eigen;



struct projectile
{
    myPoint pos; //point
    myVector vel; //vector
};

struct environment
{
    myVector gravity; //vector
    myVector wind; //vector
};

projectile tick(environment env, projectile bullet)
{
    projectile newBullet;
    newBullet.pos = bullet.pos + bullet.vel;
    newBullet.vel = bullet.vel + env.gravity + env.wind;

    return newBullet;
}

void launchTest()
{
    Canvas canvas(900, 500);

    projectile myProj;
    myProj.pos = myPoint(0, 1, 0);
    myProj.vel = myVector(1, 1.8, 0).normalize() * 11.25f;

    environment env;
    env.gravity = myVector(0, -0.1, 0);
    env.wind = myVector(-0.01, 0, 0);

    while (myProj.pos.y > 0)
    {
        myProj = tick(env, myProj);

        //Paint the pixel of projectile red
        if (myProj.pos.x < canvas.width || myProj.pos.y > 0)
            canvas.pixel_matrix[(int)myProj.pos.x][canvas.height - (int)myProj.pos.y] = Color(1, 0, 0);
    }

    canvas.canvas_to_ppm();
}

void inverseTest()
{
    MyMatrix m1(4);
    MyMatrix m2(4);

    m1(0, 0) = 3; m1(0, 1) = -9; m1(0, 2) = 7; m1(0, 3) = 3;
    m1(1, 0) = 3; m1(1, 1) = -8; m1(1, 2) = 2; m1(1, 3) = -9;
    m1(2, 0) = -4; m1(2, 1) = 4; m1(2, 2) = 4; m1(2, 3) = 1;
    m1(3, 0) = -6; m1(3, 1) = 5; m1(3, 2) = -1; m1(3, 3) = 1;

    m2(0, 0) = 8; m2(0, 1) = 2; m2(0, 2) = 2; m2(0, 3) = 2;
    m2(1, 0) = 3; m2(1, 1) = -1; m2(1, 2) = 7; m2(1, 3) = 0;
    m2(2, 0) = 7; m2(2, 1) = 0; m2(2, 2) = 5; m2(2, 3) = 4;
    m2(3, 0) = 6; m2(3, 1) = -2; m2(3, 2) = 0; m2(3, 3) = 5;

    Tuple tuple(1, 2, 3, 1);

    MyMatrix m3(3);
    m3(0, 0) = 1; m3(0, 1) = 2; m3(0, 2) = 6;
    m3(1, 0) = -5; m3(1, 1) = 8; m3(1, 2) = -4;
    m3(2, 0) = 2; m3(2, 1) = 6; m3(2, 2) = 4;

    MyMatrix m4 = m1 * m2;
    MyMatrix clon_m1 = m4 * inverse(m2);

    if (m1 == clon_m1)
        cout << "IGUALES";
    else
        cout << "DISTINTOS";
}

void testTransformation()
{
    const char* test;
    //Multiplying by a translation matrix
    test = "Multiplying by a translation matrix";
    MyMatrix transform = translation(5, -3, 2);
    myPoint p(-3,4,5);
    if (transform * p == myPoint(2, 1, 7))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Multiplying by the inverse of a translation matrix
    test = "Multiplying by the inverse of a translation matrix";
    MyMatrix inv = inverse(transform);
    if (inv * p == myPoint(-8, 7, 3))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Translation does not affect vectors
    test = "Translation does not affect vectors";
    myVector v(-3, 4, 5);
    if (v == transform * v)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A scaling matrix applied to a point
    test = "A scaling matrix applied to a point";
    transform = scaling(2, 3, 4);
    p = myPoint(-4, 6, 8);
    if (transform * p == myPoint(-8, 18, 32))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A scaling matrix applied to a vector
    test = "A scaling matrix applied to a vector";
    v = myVector(-4, 6, 8);
    if (transform * v == myVector(-8, 18, 32))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Multiplying by the inverse of a scaling matrix
    test = "Multiplying by the inverse of a scaling matrix";
    inv = inverse(transform);
    if (inv * v == myVector(-2, 2, 2))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Reflection is scaling by a negative value
    test = "Reflection is scaling by a negative value";
    transform = scaling(-1, 1, 1);
    p = myPoint(2, 3, 4);
    if (transform * p == myPoint(-2, 3, 4))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Rotating a point around the x axis
    test = "Rotating a point around the x axis";
    p = myPoint(0, 1, 0);
    MyMatrix half_quarter = rotation_x(PI / 4);
    MyMatrix full_quarter = rotation_x(PI / 2);
    if (half_quarter * p == myPoint(0, sqrt(2) / 2, sqrt(2) /2) && full_quarter * p == myPoint(0,0,1))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The inverse of an x-rotation rotates in the opposite direction
    test = "The inverse of an x-rotation rotates in the opposite direction";
    inv = inverse(half_quarter);
    if (inv * p == myPoint(0, sqrt(2) / 2, -sqrt(2) / 2))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Rotating a point around the y axis
    test = "Rotating a point around the y axis";
    p = myPoint(0, 0, 1);
    half_quarter = rotation_y(PI / 4);
    full_quarter = rotation_y(PI / 2);
    if (half_quarter * p == myPoint(sqrt(2) / 2, 0, sqrt(2) / 2) && full_quarter * p == myPoint(1, 0, 0))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Rotating a point around the z axis
    test = "Rotating a point around the z axis";
    p = myPoint(0, 1, 0);
    half_quarter = rotation_z(PI / 4);
    full_quarter = rotation_z(PI / 2);
    if (half_quarter * p == myPoint(-sqrt(2) / 2, sqrt(2) / 2, 0) && full_quarter * p == myPoint(-1, 0, 0))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A shearing transformation moves x in proportion to y
    test = "A shearing transformation moves x in proportion to y";
    transform = shearing(1, 0, 0, 0, 0, 0);
    p = myPoint(2, 3, 4);
    if (transform * p == myPoint(5,3,4))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A shearing transformation moves x in proportion to z
    test = "A shearing transformation moves x in proportion to z";
    transform = shearing(0, 1, 0, 0, 0, 0);
    if (transform * p == myPoint(6, 3, 4))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A shearing transformation moves y in proportion to x
    test = "A shearing transformation moves y in proportion to x";
    transform = shearing(0, 0, 1, 0, 0, 0);
    if (transform * p == myPoint(2, 5, 4))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A shearing transformation moves y in proportion to z
    test = "A shearing transformation moves y in proportion to z";
    transform = shearing(0, 0, 0, 1, 0, 0);
    if (transform * p == myPoint(2, 7, 4))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A shearing transformation moves z in proportion to x
    test = "A shearing transformation moves z in proportion to x";
    transform = shearing(0, 0, 0, 0, 1, 0);
    if (transform * p == myPoint(2, 3, 6))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A shearing transformation moves z in proportion to y
    test = "A shearing transformation moves z in proportion to y";
    transform = shearing(0, 0, 0, 0, 0, 1);
    if (transform * p == myPoint(2, 3, 7))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Individual transformations vs chained transformations
    test = "Individual transformations are applied in sequence";
    p = myPoint(1, 0, 1);
    MyMatrix A = rotation_x(PI / 2);
    MyMatrix B = scaling(5, 5, 5);
    MyMatrix C = translation(10, 5, 7);
    myPoint p1, p2, p3, p3_c;
    p1 = A * p;
    p2 = B * p1;
    p3 = C * p2;
    p3_c = C * B * A * p;
    if (p3 == myPoint(15, 0, 7) && p3_c == p3)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);
   
}

//Paint with pixel each number of a clock
void clockTest()
{
    Canvas canvas(900, 500);

    myPoint origin, point;
    MyMatrix clockHand = translation(0, 10, 0);
    MyMatrix T = translation(450, 250, 0);
    float step = PI / 6;
    float radians = 0;
    
    //First pixel outside the loop
    for (int i = 0; i < 12; ++i)
    {
        point = T * rotation_z(radians) * clockHand * origin;

        canvas.pixel_matrix[(int)point.x][(int)point.y] = Color(1, 1, 1);

        radians += step;
    }
    

    canvas.canvas_to_ppm();
}

void testRay()
{
    //Computing a point from a distance
    const char* test = "Computing a point from a distance";
    myPoint p(2, 3, 4);
    myVector v(1, 0, 0);
    myRay ray(p,v);
    if (ray.position(0) == myPoint(2, 3, 4) && ray.position(1) == myPoint(3, 3, 4) && ray.position(-1) == myPoint(1, 3, 4) && ray.position(2.5) == myPoint(4.5, 3, 4))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A ray intersects a sphere at two points
    test = "A ray intersects a sphere at two points";
    p = myPoint(0, 0, -5);
    v = myVector(0, 0, 1);
    ray = myRay(p, v);
    Sphere s = sphere();
    vector<Itr> xs = ray.intersect(s);
    
    if (xs[0].t == 4 && xs[1].t == 6)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A ray intersects a sphere at a tangent
    test = "A ray intersects a sphere at a tangent";
    ray = myRay(myPoint(0, 1, -5), v);
    xs = ray.intersect(s);
    if (xs[0].t == 5 && xs[1].t == 5)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A ray misses a sphere
    test = "A ray misses a sphere";
    ray = myRay(myPoint(0, 2, -5), v);
    xs = ray.intersect(s);
    if (xs[0].t == NULL)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A ray originates inside a sphereç
    test = "A ray originates inside a sphere";
    ray = myRay(myPoint(0, 0, 0), v);
    xs = ray.intersect(s);
    if (xs[0].t == -1 && xs[1].t == 1)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A sphere is behind a ray
    test = "A sphere is behind a ray";
    ray = myRay(myPoint(0, 0, 5), v);
    xs = ray.intersect(s);
    
    if (xs[0].t == -6 && xs[1].t == -4)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Intersect sets the object on the intersection
    test = "Intersect sets the object on the intersection";
    if (xs[0].object == s && xs[1].object == s)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);


    //The hit, when all intersections have positive t
    test = "The hit, when all intersections have positive t";
    Sphere s2 = sphere();
    Itr i1, i2, i3, i4, i;
    i1.t = 1; i1.object = s;
    i2.t = 2; i2.object = s;
    vector<Itr> interactions = { i1, i2 };
    i = hit(interactions);
    
    if (i == i1)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The hit, when some intersections have negative t
    test = "The hit, when some intersections have negative t";
    i1.t = -1; i1.object = s;
    i2.t = 1; i2.object = s;
    interactions[0] = i1;
    interactions[1] = i2;
    i = hit(interactions);
    if (i == i2)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The hit, when all intersections have negative t
    test = "The hit, when all intersections have negative t";
    i1.t = -1; i1.object = s;
    i2.t = -1; i2.object = s;
    interactions[0] = i1;
    interactions[1] = i2;
    i = hit(interactions);
    if (i.t == NULL)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The hit is always the lowest nonnegative intersection
    test = "The hit is always the lowest nonnegative intersection";
    i1.t = 5; i1.object = s;
    i2.t = 7; i2.object = s;
    i3.t = -3; i3.object = s;
    i4.t = 2; i4.object = s;
    vector<Itr> interactions4 = { i1, i2, i3, i4 };
    i = hit(interactions4);
    if (i == i4)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Translating a ray
    test = "Translating a ray";
    ray = myRay(myPoint(1, 2, 3), myVector(0, 1, 0));
    MyMatrix m = translation(3,4,5);
    myRay ray2 = ray.transform(m);
    if (ray2.origin == myPoint(4,6,8) && ray2.direction == myVector(0,1,0))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Scaling a ray
    test = "Scaling a ray";
    m = scaling(2, 3, 4);
    ray2 = ray.transform(m);
    if (ray2.origin == myPoint(2, 6, 12) && ray2.direction == myVector(0, 3, 0))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Intersecting a scaled sphere with a ray
    test = "Intersecting a scaled sphere with a ray";
    ray = myRay(myPoint(0, 0, -5), myVector(0, 0, 1));
    s.transform = scaling(2, 2, 2);
    xs = ray.intersect(s);
    if (xs[0].t == 3 && xs[1].t == 7)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Intersecting a translated sphere with a ray
    test = "Intersecting a translated sphere with a ray";
    s.transform = translation(5, 0, 0);
    xs = ray.intersect(s);
    if (xs[0].t == NULL)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);
}

void testSphere()
{
    Canvas canvas(100, 100);
    Sphere s = sphere();

    //s.transform = shearing(1, 0, 0, 0, 0, 0) * scaling(0.5, 1, 1);

    myVector direction = myVector(0, 0, -1);
    myRay ray = myRay(myPoint(0,0,-5), direction);
    myRay r;
    float wall_z = 10;
    float wall_size = 7;
    float pixel_size = wall_size / canvas.width;
    float half = wall_size / 2;
    float world_y, world_x;

    vector<Itr> its;
    Itr i;
    for (int y = 0; y < canvas.height; ++y)
    {
        world_y = half - pixel_size * y;
        for(int x = 0; x < canvas.width; ++x)
        {
            world_x = -half + pixel_size * x;

            myVector point = myPoint(world_x, world_y, wall_z) - ray.origin;
            point = point.normalize();

            r = myRay(ray.origin, point);
            its = r.intersect(s);
            i = hit(its);

            if (i.t != NULL) //A hit was registered
                canvas.pixel_matrix[x][y] = Color(1, 0, 0);
            else //Didnt hit
                canvas.pixel_matrix[x][y] = Color(0, 0, 0);

        }
    }

    canvas.canvas_to_ppm();
}

void testLighShading()
{
    //The normal on a sphere at a nonaxial point
    const char* test = "The normal on a sphere at a nonaxial point";
    Sphere s = sphere();
    myVector n = normal_at(s, myPoint(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
    if (n == myVector(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);


    //Computing the normal on a translated sphere
    test = "Computing the normal on a translated sphere";
    s.transform = translation(0, 1, 0);
    n = normal_at(s, myPoint(0, 1.70711, -0.70711));
    if (n == myVector(0, 0.70711, -0.70711))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Computing the normal on a transformed sphere
    test = "Computing the normal on a transformed sphere";
    s.transform = scaling(1, 0.5, 1) * rotation_z(PI / 5);
    n = normal_at(s, myPoint(0, sqrt(2) / 2, -sqrt(2) / 2));
    if (n == myVector(0, 0.97014, -0.24254))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Reflecting a vector approaching at 45 degrees
    test = "Reflecting a vector approaching at 45°";
    myVector r = reflect(myVector(1, -1, 0), myVector(0, 1, 0));
    if (r == myVector(1, 1, 0))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Reflecting a vector off a slanted surface
    test = "Reflecting a vector off a slanted surface";
    r = reflect(myVector(0, -1, 0), myVector(sqrt(2) / 2, sqrt(2) / 2, 0));
    if (r == myVector(1, 0, 0))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);


    //Lightning
    Material m = material();
    myPoint position = myPoint(0, 0, 0);
    myVector eye, normal;
    Light light;
    Color result;

    //Lighting with the eye between the light and the surface
    test = "Lighting with the eye between the light and the surface";
    eye = myVector(0, 0, -1);
    normal = myVector(0, 0, -1);
    light.position = myPoint(0, 0, -10);
    light.intensity = Color(1, 1, 1);
    bool in_shadow = false;
    result = lighting(m, light, position, eye, normal, in_shadow);
    
    if (result == Color(1.9, 1.9, 1.9))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Lighting with the eye between light and surface, eye offset 45 degrees
    test = "Lighting with the eye between light and surface, eye offset 45 degrees";
    eye = myVector(0, sqrt(2) / 2, -sqrt(2) / 2);
    normal = myVector(0, 0, -1);
    result = lighting(m, light, position, eye, normal, in_shadow);
    if (result == Color(1.0, 1.0, 1.0))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Lighting with eye opposite surface, light offset 45 degrees
    test = "Lighting with eye opposite surface, light offset 45 degrees";
    eye = myVector(0, 0, -1);
    normal = myVector(0, 0, -1);
    light.position = myPoint(0, 10, -10);
    result = lighting(m, light, position, eye, normal, in_shadow);
    
    if (result == Color(0.7364, 0.7364, 0.7364))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Lighting with eye in the path of the reflection vector
    test = "Lighting with eye in the path of the reflection vector";
    eye = myVector(0, -sqrt(2) / 2, -sqrt(2) / 2);
    normal = myVector(0, 0, -1);
    light.position = myPoint(0, 10, -10);
    result = lighting(m, light, position, eye, normal, in_shadow);
    if (result == Color(1.6364, 1.6364, 1.6364))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Lighting with the light behind the surface
    test = "Lighting with the light behind the surface";
    eye = myVector(0, 0, -1);
    normal = myVector(0, 0, -1);
    light.position = myPoint(0, 0, 10);
    result = lighting(m, light, position, eye, normal, in_shadow);
    if (result == Color(0.1, 0.1, 0.1))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Lighting with the surface in shadow
    test = "Lighting with the surface in shadow";
    light.position = myPoint(0, 0, -10);
    light.intensity = Color(1, 1, 1);
    in_shadow = true;
    result = lighting(m, light, position, eye, normal, in_shadow);
    if (result == Color(0.1, 0.1, 0.1))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);
}

void testSphereShading()
{
    //Creation of canvas
    Canvas canvas(100, 100);

    //Defining sphere and material
    Sphere s = sphere();
    s.material = material();
    s.material.color = Color(1, 0.2, 1);
    //s.transform = shearing(1, 0, 0, 0, 0, 0) * scaling(0.5, 1, 1);

    //Define light
    Light l = light();
    l.position = myPoint(-10, 10, -10);
    l.intensity = Color(1, 1, 1);

    //Define ray
    myVector direction = myVector(0, 0, -1);
    myRay ray = myRay(myPoint(0, 0, -5), direction);
    myRay r;

    //Define wall where we are going to draw circle
    float wall_z = 10;
    float wall_size = 7;
    float pixel_size = wall_size / canvas.width;
    float half = wall_size / 2;
    float world_y, world_x;

    vector<Itr> its;
    Itr i;
    for (int y = 0; y < canvas.height; ++y)
    {
        world_y = half - pixel_size * y;
        for (int x = 0; x < canvas.width; ++x)
        {
            world_x = -half + pixel_size * x;

            myVector point = myPoint(world_x, world_y, wall_z) - ray.origin;
            point = point.normalize();

            r = myRay(ray.origin, point);
            its = r.intersect(s);
            i = hit(its);

            if (i.t != NULL) //A hit was registered
            {
                myPoint p = r.position(i.t);
                myVector normal = normal_at(i.object, p);
                myVector eye = -r.direction;
                bool in_shadow = false;
                Color color = lighting(i.object.material, l, p, eye, normal, in_shadow);
                canvas.pixel_matrix[x][y] = color;
            }
                
            else //Didnt hit
                canvas.pixel_matrix[x][y] = Color(0, 0, 0);

        }
    }

    canvas.canvas_to_ppm();
}

void testWorld()
{
    World w;
    //Intersect a world with a ray
    const char* test = "Intersect a world with a ray";
    myRay r = myRay(myPoint(0, 0, -5), myVector(0, 0, 1));
    //Array -> vector of intersections
    vector<Itr> xs = intersect_world(w, r);
    if (xs[0].t == 4 && xs[1].t == 4.5 && xs[2].t == 5.5 && xs[3].t == 6)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The hit, when an intersection occurs on the outside
    test = "The hit, when an intersection occurs on the outside";
    Sphere shape = sphere();
    Itr it;
    it.t = 4; it.object = shape;
    ItrComps comps = prepare_computations(it, r);
    if (!comps.inside)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The hit, when an intersection occurs on the inside
    test = "The hit, when an intersection occurs on the inside";
    r.origin = myPoint(0, 0, 0);
    it.t = 1;
    comps = prepare_computations(it, r);
    if (comps.point == myPoint(0,0,1) && comps.eye == myVector(0,0,-1) && comps.inside == true && comps.normal == myVector(0,0,-1))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Shading an intersection
    test = "Shading an intersection";
    r.origin = myPoint(0, 0, -5);
    it.t = 4; it.object = w.objects[0];
    comps = prepare_computations(it, r);
    Color color = shade_hit(w, comps);
    if (color == Color(0.38066, 0.47583, 0.2855))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Shading an intersection from the inside
    test = "Shading an intersection from the inside";
    w._light = light();
    w._light.position = myPoint(0, 0.25, 0);
    w._light.intensity = Color(1, 1, 1);
    r.origin = myPoint(0, 0, 0);
    it.t = 0.5; it.object = w.objects[1];
    comps = prepare_computations(it, r);
    color = shade_hit(w, comps);

    if (color == Color(0.90498, 0.90498, 0.90498))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The color when a ray misses
    test = "The color when a ray misses";
    r = myRay(myPoint(0, 0, -5), myVector(0, 1, 0));
    color = color_at(w, r);
    if (color == Color(0,0,0))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The color when a ray hits
    World default_world;
    test = "The color when a ray hits";
    r = myRay(myPoint(0, 0, -5), myVector(0, 0, 1));
    color = color_at(default_world, r);
    if (color == Color(0.38066, 0.47583, 0.2855))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The color with an intersection behind the ray
    test = "The color with an intersection behind the ray";
    Sphere* outer = &default_world.objects[0];
    outer->material.ambient = 1;
    Sphere* inner = &default_world.objects[1];
    inner->material.ambient = 1;
    r = myRay(myPoint(0, 0, 0.75), myVector(0, 0, -1));
    color = color_at(default_world, r);
    if (color == inner->material.color)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The transformation matrix for the default orientation
    test = "The transformation matrix for the default orientation";
    myPoint from = myPoint(0, 0, 0);
    myPoint to = myPoint(0, 0, -1);
    myVector up = myVector(0, 1, 0);
    MyMatrix view = view_transform(from, to, up);
    if (view == I_Matrix())
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //A view transformation matrix looking in positive z direction
    test = "A view transformation matrix looking in positive z direction";
    from = myPoint(0, 0, 0);
    to = myPoint(0, 0, 1);
    up = myVector(0, 1, 0);
    view = view_transform(from, to, up);
    if (view == scaling(-1, 1, -1))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);
    
    //The view transformation moves the world
    test = "The view transformation moves the world";
    from = myPoint(0, 0, 8);
    to = myPoint(0, 0, 0);
    up = myVector(0, 1, 0);
    view = view_transform(from, to, up);
    if (view == translation(0, 0, -8))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //An arbitrary view transformation
    test = "An arbitrary view transformation";
    from = myPoint(1, 3, 2);
    to = myPoint(4, -2, 8);
    up = myVector(1, 1, 0);
    view = view_transform(from, to, up);
    MyMatrix m = I_Matrix();
    m(0, 0) = -0.50709; m(0, 1) = 0.50709; m(0, 2) = 0.67612; m(0, 3) = -2.36643;
    m(1, 0) = 0.76772; m(1, 1) = 0.60609; m(1, 2) = 0.12122; m(1, 3) = -2.82843;
    m(2, 0) = -0.35857; m(2, 1) = 0.59761; m(2, 2) = -0.71714;
    if (view == m)
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The pixel size for a horizontal canvas
    test = "The pixel size for a horizontal canvas";
    Camera c = Camera(200, 125, PI / 2);
    if (equalFloat(c.pixel_size, 0.01))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The pixel size for a vertical canvas
    test = "The pixel size for a vertical canvas";
    c = Camera(125, 200, PI / 2);
    if (equalFloat(c.pixel_size, 0.01))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Constructing a ray through the center of the canvas
    test = "Constructing a ray through the center of the canvas";
    c = Camera(201, 101, PI / 2);
    r = c.ray_for_pixel(100, 50);
    if (r.origin == myPoint(0,0,0) && r.direction == myVector(0,0,-1))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Constructing a ray through a corner of the canvas
    test = "Constructing a ray through a corner of the canvas";
    r = c.ray_for_pixel(0, 0);
    if (r.origin == myPoint(0, 0, 0) && r.direction == myVector(0.66519, 0.33259, -0.66851))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Constructing a ray when the camera is transformed
    test = "Constructing a ray when the camera is transformed";
    c.transform = rotation_y(PI / 4) * translation(0, -2, 5);
    r = c.ray_for_pixel(100, 50);
    if (r.origin == myPoint(0, 2, -5) && r.direction == myVector(sqrt(2) / 2, 0, -sqrt(2) / 2))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //Rendering a world with a camera
    test = "Rendering a world with a camera";
    c = Camera(11, 11, PI / 2);
    from = myPoint(0, 0, -5);
    to = myPoint(0, 0, 0);
    up = myVector(0, 1, 0);
    c.transform = view_transform(from, to, up);
    default_world = World();
    Canvas canvas = c.render(default_world);
    if (canvas.pixel_matrix[5][5] == Color(0.38066, 0.47583, 0.2855))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //There is no shadow when nothing is collinear with point and light
    test = "There is no shadow when nothing is collinear with point and light";
    if(!is_shadowed(default_world, myPoint(0,10,0)))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //The shadow when an object is between the point and the light
    test = "The shadow when an object is between the point and the light";
    if (is_shadowed(default_world, myPoint(10, -10, 10)))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //There is no shadow when an object is behind the light
    test = "There is no shadow when an object is behind the light";
    if (!is_shadowed(default_world, myPoint(-20, 20, -20)))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //There is no shadow when an object is behind the point
    test = "There is no shadow when an object is behind the point";
    if (!is_shadowed(default_world, myPoint(-2, 2, -2)))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);

    //shade_hit() is given an intersection in shadow
    test = "shade_hit() is given an intersection in shadow";
    Sphere s1 = sphere();
    Sphere s2 = sphere();
    s2.transform = translation(0, 0, 10);
    vector<Sphere> objects = { s1, s2 };
    Light l = light();
    l.position = myPoint(0,0,-10);
    l.intensity = Color(1,1,1);
    w = World(objects,l);
    r = myRay(myPoint(0, 0, 5), myVector(0, 0, 1));
    it.t = 4; it.object = s2;
    comps = prepare_computations(it, r);
    color = shade_hit(w, comps);
    if (color == Color(0.1, 0.1, 0.1))
        printf("%s: TEST PASSED\n", test);
    else
        printf("%s: TEST FAILED\n", test);



}

void testSceneShading()
{
    Sphere floor, left_wall, right_wall;

    floor = sphere();
    floor.transform = scaling(10, 0.01, 10);
    floor.material = material();
    floor.material.color = Color(1, 0.9, 0.9);
    floor.material.specular = 0;

    left_wall = sphere();
    left_wall.transform = translation(0, 0, 5) * rotation_y(-PI / 4) * rotation_x(PI / 2) * scaling(10, 0.01, 10);
    left_wall.material = floor.material;

    right_wall = sphere();
    right_wall.transform = translation(0, 0, 5) * rotation_y(PI / 4) * rotation_x(PI / 2) * scaling(10, 0.01, 10);
    right_wall.material = floor.material;

    Sphere middle, right, left;

    middle = sphere();
    middle.material = material();
    middle.transform = translation(-0.5, 1, 0.5);
    middle.material.color = Color(0.1, 1, 0.5);
    middle.material.diffuse = 0.7;
    middle.material.specular = 0.3;

    right = sphere();
    right.material = material();
    right.transform = translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5);
    right.material.color = Color(0.5, 1, 0.1);
    right.material.diffuse = 0.7;
    right.material.specular = 0.3;

    left = sphere();
    left.material = material();
    left.transform = translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33);
    left.material.color = Color(1, 0.8, 0.1);
    left.material.diffuse = 0.7;
    left.material.specular = 0.3;

    vector<Sphere> objects = {floor, left_wall, right_wall, middle, right, left};
    Light l = light();
    l.position = myPoint(-10, 10, -10);
    
    World world = World(objects, l);
    
    Camera cam = Camera(320, 180, PI / 3);
    cam.transform = view_transform(myPoint(0, 1.5, -5), myPoint(0,1,0), myVector(0,1,0));
    
    Canvas canvas = cam.render(world);
    
    canvas.canvas_to_ppm();
}

int main()
{
    auto start = chrono::high_resolution_clock::now();
    //launchTest();

    //testTransformation();

    //clockTest();

    //testRay();

    //testSphere();

    //testLighShading();

    //testSphereShading();

    //testWorld();

    testSceneShading();

    auto stop = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::seconds>(stop - start).count() << " seconds" << endl;

    return 0;
}