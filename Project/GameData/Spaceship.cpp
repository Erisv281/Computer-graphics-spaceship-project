

#include "Spaceship.h"

// Constructors
Spaceship::Spaceship() : Entity(), isShooting{false} {}
Spaceship::Spaceship(Model* model, int health, float speed, vec3 position, int damage) : Entity(model, health, speed, position, damage), 
                    isShooting{false} {}


// Copy constructor
Spaceship::Spaceship(const Spaceship& other) : Entity(other){
    this->isShooting = other.isShooting;
}

// Copy-assign constructor
Spaceship& Spaceship::operator=(const Spaceship& other){
    Spaceship temp{other};
    Entity::operator=(temp);
    std::swap(this->isShooting, temp.isShooting);

    return *this;
}

// Collision detection
void Spaceship::collisionEnemies(){}
void Spaceship::collisionSurface(){}

// Move to new position. Todo fix later
void Spaceship::move(vec3 pos){
    this->position += pos;
}


// Setters
void Spaceship::setRotAngleX(float angle){
    this->rotAngleX = angle;
}
void Spaceship::setRotAngleZ(float angle){
    this->rotAngleZ = angle;
}

void Spaceship::setIsShooting(bool isShooting){
    this->isShooting = isShooting;
}


// Shoot bullet
void Spaceship::shoot(){
    this->isShooting = true;
    this->shootTime = std::chrono::system_clock::now();
}

bool Spaceship::isShootCooldownWlapsed(double cooldown) const{
    auto currentTime = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration<double>(currentTime - shootTime).count();
    return elapsed >= cooldown;

}