
// This class override the Entity class and represents bullets shot from spaceship in the game. 
// Has the additional parameter called direaction, which is used to project the bullet at correct direction. 

#ifndef _BULLET
#define _BULLET

#include "Entity.h"

class Bullet : public Entity{
private:
    // Shot direction 
    vec3 direction{vec3{0,0,0}};

public:
    // Constructors
    Bullet();
    Bullet(Model* model, int health, float speed, vec3 position, int damage, float radius, vec3 direction);

    // Getter
    inline vec3 getDirection() const { return this->direction; }

    // Overriden methods
    void move(vec3 const pos) override;
    void draw(GLuint program, mat4 worldMatrix) override;
    vec3 getCenterPosition() const override;
    

};


#endif