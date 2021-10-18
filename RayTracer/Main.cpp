#include <iostream>
#include <Eigen/Dense>
#include "tuples.h"

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

int main()
{
    /*
    projectile myProj;
    myProj.pos = Tuple(0,1,0,1);
    myProj.vel = Tuple(1,1,0,0);
    
    environment env;
    env.gravity = Tuple(0, -0.1, 0, 0);
    env.wind = Tuple(-0.01, 0, 0, 0);
    
    while (myProj.pos.y > 0)
    {
        myProj = tick(env, myProj);

        cout << "Bullet position: " << myProj.pos << endl;
        cout << "Bullet velocity: " << myProj.vel << endl;
    }
    */

    myVector a(1, 0, 0);
    myVector b(0, 1, 0);

    Color c1(1, 0.2, 0.4);
    Color c2(0.9, 1, 0.1);

    c1.x = 0.5;
    cout << c1.r << endl;
}