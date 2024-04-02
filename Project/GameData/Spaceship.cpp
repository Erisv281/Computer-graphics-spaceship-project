

#include "Spaceship.h"


// Constructors
Spaceship::Spaceship();
Spaceship::Spaceship(Model* model, int health, float speed, vec3 position);

// Destructor
Spaceship::~Spaceship();

// Collision detection
void Spaceship::collisionEnemies();
void Spaceship::collisionSurface();

// Move to new position. Todo fix later
void Spaceship::move(vec3 pos);

// Shoot bullet
void Spaceship::Shoot();

// Reduce health
void Spaceship::takeDamage(int damage);