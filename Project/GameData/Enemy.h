

// This class represents Enemies of the game. 

#ifndef _ENEMY
#define _ENEMY

#include "Entity.h"
#include <chrono>

class Enemy : public Entity{

public:
    // Constructors
    Enemy();
    Enemy(Model* model, int health, float speed, vec3 position, int damage, float radius);

    // Overriden methods.
    void move(vec3  const pos) override;
    void draw(GLuint program, mat4 worldMatrix) override;
    vec3 getCenterPosition() const override;

};


#endif