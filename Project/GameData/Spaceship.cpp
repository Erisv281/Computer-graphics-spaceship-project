

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

bool Spaceship::isShootCooldownElapsed(double cooldown) const{
    auto currentTime = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration<double>(currentTime - shootTime).count();
    return elapsed >= cooldown;

}


// Returns the crosshair position by modifying xy position. 
vec3 Spaceship::getCrosshairPosition(){
    vec3 pos = this->position;
    pos.x += 3.0f;
	pos.y += 15.0f;
    return pos;
}


// Draw
void Spaceship::draw(GLuint program, mat4 worldMatrix){
    glUseProgram(program);

	// If spaceship is shooting and time has elapsed, then allow shooting again. 
	if (isShooting && isShootCooldownElapsed(SHOOTING_TIME)){
		setIsShooting(false);
	}

	// Set Model-world matrix
	mat4 total = worldMatrix * T(position.x, position.y, position.z) * Rx(rotAngleX) * Rz(rotAngleZ) * S(1.0, 1.0, 1.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_world"), 1, GL_TRUE, total.m);

	// Draw
	DrawModel(model, program, "in_Position", "in_Normal", "inTexCoord");

}