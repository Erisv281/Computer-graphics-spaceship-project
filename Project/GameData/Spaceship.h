

#ifndef _SPACESHIP
#define _SPACESHIP

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include "LoadTGA.h"
#include <iostream>

class Spaceship{
private:
    Model* model;
    int health{0};
    float speed{0.0};
    vec3 position{0, 0, 0};
    bool canShoot{true};

public:
    // Constructors
    Spaceship();
    Spaceship(Model* model, int health, float speed, vec3 position);

    // Destructor
    ~Spaceship();

    // Copy and assign constructors
    Spaceship(const Spaceship& other);
    Spaceship& operator=(const Spaceship& other);

    // Getters & setters
    // Reduce health
    void takeDamage(int damage);

    Model* getModel();
    vec3 getPosition();

    // Collision detection
    void collisionEnemies();
    void collisionSurface();

    // Move to new position. Todo fix later
    void move(vec3 pos);

    // Shoot bullet
    void shoot();

    

};


#endif