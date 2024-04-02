

#ifndef _SPACESHIP
#define _SPACESHIP

#include "GL_utilities.h"
#include "MicroGlut.h"

class Spaceship{
private:
    Model* model;
    int health{0};
    float speed{0.0};
    vec3 position{};
    bool canShoot{true};

public:
    // Constructors
    Spaceship();
    Spaceship(Model* model, int health, float speed, vec3 position);

    // Destructor
    ~Spaceship();

    // Collision detection
    void collisionEnemies();
    void collisionSurface();

    // Move to new position. Todo fix later
    void move(vec3 pos);

    // Shoot bullet
    void Shoot();

    // Reduce health
    void takeDamage(int damage);

};


#endif