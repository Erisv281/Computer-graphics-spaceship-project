

#ifndef _BULLET
#define _BULLET

#include "Entity.h"

class Bullet : public Entity{

public:
    // Constructors
    Bullet();
    Bullet(Model* model, int health, float speed, vec3 position, int damage);

    // Collision detection
    void collisionEnemies();

    // Move to new position.
    void move(vec3 pos) override;
    

};


#endif