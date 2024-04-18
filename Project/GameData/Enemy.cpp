
#include "Enemy.h"



// Constructors
Enemy::Enemy() : Entity() {}
Enemy::Enemy(Model* model, int health, float speed, vec3 position, int damage) : Entity(model, health, speed, position, damage) {}


// Move to new position.
void Enemy::move(vec3 pos){this->position += pos;}
