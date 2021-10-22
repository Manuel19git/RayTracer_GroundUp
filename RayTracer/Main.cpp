#include <iostream>
#include <Eigen/Dense>
#include "Canvas.h"
#include "MyMatrix.h"

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


int main()
{
    //launchTest();

    //testTransformation();

    clockTest();

    return 0;
}