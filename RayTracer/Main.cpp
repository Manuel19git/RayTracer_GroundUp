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



int main()
{
    //launchTest();

    MyMatrix m1(4);
    MyMatrix m2(4);

    m1(0, 0) = 1; m1(0, 1) = 2; m1(0, 2) = 3; m1(0, 3) = 4;
    m1(1, 0) = 5; m1(1, 1) = 6; m1(1, 2) = 7; m1(1, 3) = 8;
    m1(2, 0) = 9; m1(2, 1) = 8; m1(2, 2) = 7; m1(2, 3) = 6;
    m1(3, 0) = 5; m1(3, 1) = 4; m1(3, 2) = 3; m1(3, 3) = 2;

    m2(0, 0) = -2; m2(0, 1) = 1; m2(0, 2) = 2; m2(0, 3) = 3;
    m2(1, 0) = 3; m2(1, 1) = 2; m2(1, 2) = 1; m2(1, 3) = -1;
    m2(2, 0) = 4; m2(2, 1) = 3; m2(2, 2) = 6; m2(2, 3) = 5;
    m2(3, 0) = 1; m2(3, 1) = 2; m2(3, 2) = 7; m2(3, 3) = 8;

    cout << m1 << endl;
    cout << m2 << endl;


    cout << m1 * m2;


    return 0;
}