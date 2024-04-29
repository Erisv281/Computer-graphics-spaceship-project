
#include "Bullet.h"

// Constructors
Bullet::Bullet() : Entity() {this->direction = vec3{0,0,0};}
Bullet::Bullet(Model* model, int health, float speed, vec3 position, int damage, vec3 direction) : Entity(model, health, speed, position, damage) {
    this->direction = direction;
}


// Move continously along pos. 
void Bullet::move(vec3 pos){
    this->position += pos * speed;
}