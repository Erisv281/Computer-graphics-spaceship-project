
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

void Bullet::draw(GLuint program, mat4 worldMatrix){
    // Movement
    move(vec3{1.0f, direction.y, direction.z});	// todo check later

    // Set model_world matrix
    mat4 total = worldMatrix * T(position.x, position.y, position.z) * S(2.0, 2.0, 2.0);
    glUniformMatrix4fv(glGetUniformLocation(program, "model_world"), 1, GL_TRUE, total.m);

    // Draw model
    DrawModel(model, program, "in_Position", "in_Normal", "inTexCoord");
}
