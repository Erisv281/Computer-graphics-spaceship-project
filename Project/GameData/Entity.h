

#ifndef _ENTITY
#define _ENTITY

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include "LoadTGA.h"
#include <iostream>



class Entity{
protected:
    Model* model{nullptr};
    int health{0};
    float speed{0.0};
    vec3 position{0, 0, 0};
    bool isDead{false};
    int damage{0};
    float radius{0.0f};

public:
    // Constructors
    Entity();
    Entity(Model* model, int health, float speed, vec3 position, int damage, float radius);

    // Destructor
    virtual ~Entity();

    // Copy and assign constructors
    Entity(const Entity& other);
    Entity& operator=(const Entity& other);

    // Inline Getters
    inline int getHealth() const{ return this->health; }
    inline Model* getModel() const { return this->model; }
    inline vec3 getPosition() const { return this->position; }
    inline float getSpeed() const { return this->speed; }
    inline bool getIsDead() const { return this->isDead; }
    inline int getDamage() const { return this->damage; }
    inline int getRadius() const { return this->radius; }

    // Setters
    void setHealth(int health);
    void setModel(Model* m);
    void setPosition(vec3 pos); 
    void setSpeed(float speed);
    void setIsDead(bool isDead);
    void setDamage(int damage);
    void setRadius(float radius);

    // Reduce HP. If damage exceeds health then die. 
    void takeDamage(int damage);

    // Move to new position.
    virtual void move(vec3 pos);

    // Draw function to new position.
    virtual void draw(GLuint program, mat4 worldMatrix);

    // When no HP left, 
    virtual void die();

    // Get the center position of model
    virtual vec3 getCenterPosition();


};



#endif
