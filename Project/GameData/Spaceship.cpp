

#include "Spaceship.h"

// Constructors
Spaceship::Spaceship(){
    this->model = nullptr;
    this->health = 0;
    this->speed = 0.0f;
    this->position = vec3(0,0,0);
}
Spaceship::Spaceship(Model* model, int health, float speed, vec3 position){
    this->model = new Model(*model);
    this->health = health;
    this->speed = speed;
    this->position = position;
}

// Destructor
Spaceship::~Spaceship(){
    delete this->model;
}

// Copy constructor
Spaceship::Spaceship(const Spaceship& other){
    // Model set
    this->model = new Model(*other.model);

    this->health = other.health;
    this->speed = other.speed;
    this->position = other.position;

}

// Copy-assign constructor
Spaceship& Spaceship::operator=(const Spaceship& other){
    Spaceship temp{other};
    
    // Set model
    delete this->model;
    this->model = (other.model != nullptr) ? new Model(*other.model) : nullptr;

    std::swap(this->health, temp.health);
    std::swap(this->speed, temp.speed);
    std::swap(this->position, temp.position);

    return *this;
}

// Collision detection
void Spaceship::collisionEnemies(){}
void Spaceship::collisionSurface(){}

// Move to new position. Todo fix later
void Spaceship::move(vec3 pos){
    this->position += pos;
}

// Shoot bullet
void Spaceship::shoot(){}

// Reduce health
void Spaceship::takeDamage(int damage){}

Model* Spaceship::getModel(){
    return this->model;
}
vec3 Spaceship::getPosition(){
    return this->position;
}

 float Spaceship::getSpeed(){
    return this->speed;
 }

void Spaceship::setRotAngleX(float angle){
    this->rotAngleX = angle;
}
void Spaceship::setRotAngleZ(float angle){
    this->rotAngleZ = angle;
}
float Spaceship::getRotAngleX(){
    return rotAngleX;
}
float Spaceship::getRotAngleZ(){
    return rotAngleZ;
}