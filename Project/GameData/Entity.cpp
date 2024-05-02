

#include "Entity.h"



// Constructors
Entity::Entity() : model{nullptr}, health{0}, speed{0.0}, position{vec3(0,0,0)}, damage{0}, radius{0.0f} {}
Entity::Entity(Model* model, int health, float speed, vec3 position, int damage, float radius) : model{new Model{*model}},
                                                                    health{health},
                                                                    speed{speed},
                                                                    position{position},
                                                                    damage{damage},
                                                                    radius{radius} {}

// Destructor
Entity::~Entity(){
    delete this->model;
}

// Copy constructor
Entity::Entity(const Entity& other){
    // Model set
    this->model = new Model(*other.model);

    this->health = other.health;
    this->speed = other.speed;
    this->position = other.position;
    this->damage = other.damage;
    this->radius = other.radius;
}

// copy assign operator
Entity& Entity::operator=(const Entity& other){
    Entity temp{other};
    
    // Set model
    delete this->model;
    this->model = (temp.model != nullptr) ? new Model(*temp.model) : nullptr;

    std::swap(this->health, temp.health);
    std::swap(this->speed, temp.speed);
    std::swap(this->position, temp.position);
    std::swap(this->damage, temp.damage);
    std::swap(this->radius, temp.radius);

    return *this;
}


// Setters
void Entity::setHealth(int health){
    this->health = health;
}
void Entity::setModel(Model* m){
    this->model = m;
}
void Entity::setPosition(vec3 pos){
    this->position = pos;
}
void Entity::setSpeed(float speed){
    this->speed = speed;
}
void Entity::setIsDead(bool isDead){
    this->isDead = isDead;
}

void Entity::setDamage(int damage){
    this->damage = damage;
}
void Entity::setRadius(float radius){
    this->radius = radius;
}

// Decrease health from damage. 
void Entity::takeDamage(int damage){
    if (this->health - damage <= 0){
        this->health = 0;
        die();
    }
    else{
        this->health -= damage;
    }
    
}

// Moving the position
void Entity::move(vec3 pos){
    // Placeholder. 
}

void Entity::draw(GLuint program, mat4 worldMatrix){
    // Placeholder
}

vec3 Entity::getCenterPosition(){
    // Placeholder
}


// Setting isDead to true. 
void Entity::die(){
    setIsDead(true);
}

