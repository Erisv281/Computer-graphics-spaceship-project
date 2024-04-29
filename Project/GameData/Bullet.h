

#ifndef _BULLET
#define _BULLET

#include "Entity.h"

class Bullet : public Entity{
private:
    // Shooting 
    vec3 direction{vec3{0,0,0}};

public:
    // Constructors
    Bullet();
    Bullet(Model* model, int health, float speed, vec3 position, int damage, vec3 direction);

    // Collision detection
    void collisionEnemies();

    // Move to new position.
    void move(vec3 pos) override;

    // Getter
    inline vec3 getDirection() const { return this->direction; }
    

};


#endif