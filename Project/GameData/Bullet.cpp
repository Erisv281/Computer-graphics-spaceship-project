
#include "Bullet.h"

// Constructors
Bullet::Bullet() : Entity() {this->direction = vec3{0,0,0};}
Bullet::Bullet(Model* model, int health, float speed, vec3 position, int damage, vec3 direction) : Entity(model, health, speed, position, damage) {
    this->direction = direction;
}

// Collision detection
void Bullet::collisionEnemies(){
    // Add later
}

// Move continously, omit the pos parameter
void Bullet::move(vec3 pos){
    this->position += pos * speed;
}