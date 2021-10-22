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

}



int main()
{
    //launchTest();

    testTransformation();


    return 0;
}