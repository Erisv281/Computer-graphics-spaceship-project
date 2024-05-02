

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

    void move(vec3 pos) override;
    void draw(GLuint program, mat4 worldMatrix) override;

    vec3 getCenterPosition() override;
    

};


#endif