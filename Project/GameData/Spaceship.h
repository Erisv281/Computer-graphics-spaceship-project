

#ifndef _SPACESHIP
#define _SPACESHIP

#include "Entity.h"
#include <chrono>

class Spaceship : public Entity{
private:
    // Shooting 
    bool isShooting{false};
    // Code inspired by https://www.geeksforgeeks.org/chrono-in-c/
    std::chrono::time_point<std::chrono::system_clock> shootTime;


    // Rotation
    float rotAngleX{0.0f};
    float rotAngleZ{0.0f};

public:
    // Constructors
    Spaceship();
    Spaceship(Model* model, int health, float speed, vec3 position, int damage);

    // Copy and assign constructors
    Spaceship(const Spaceship& other);
    Spaceship& operator=(const Spaceship& other);

    // Inline Getters
    inline float getRotAngleX() const { return this->rotAngleX; }
    inline float getRotAngleZ() const { return this->rotAngleZ; }
    inline bool getIsShooting() const { return this->isShooting; }

    // Setters
    void setRotAngleX(float angle);
    void setRotAngleZ(float angle);
    void setIsShooting(bool isShooting);
        

    // Collision detection
    void collisionEnemies();
    void collisionSurface();

    // Move to new position.
    void move(vec3 pos) override;

    // Start timer for shooting. 
    void shoot();

    // Check if cooldown has passed. 
    bool isShootCooldownWlapsed(double cooldown) const;


    

};


#endif