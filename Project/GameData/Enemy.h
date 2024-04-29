

#ifndef _ENEMY
#define _ENEMY

#include "Entity.h"
#include <chrono>

class Enemy : public Entity{

public:
    // Constructors
    Enemy();
    Enemy(Model* model, int health, float speed, vec3 position, int damage);

    // Move to new position.
    void move(vec3 pos) override;

    

};


#endif