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

    MyMatrix m(2);

    m(0, 0) = -3;
    m(0, 1) = 5;
    m(1, 0) = 1;
    m(1, 1) = -2;

    cout << m << endl;
    
}