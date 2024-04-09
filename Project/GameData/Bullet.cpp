
#include "Bullet.h"

// Constructors
Bullet::Bullet() : Entity() {}
Bullet::Bullet(Model* model, int health, float speed, vec3 position) : Entity(model, health, speed, position) {}

// Collision detection
void Bullet::collisionEnemies(){
    // Add later
}

// Move continously, omit the pos parameter
void Bullet::move(vec3 pos){
    this->position.x += speed;
}