
#include "Enemy.h"



// Constructors
Enemy::Enemy() : Entity() {}
Enemy::Enemy(Model* model, int health, float speed, vec3 position, int damage) : Entity(model, health, speed, position, damage) {}


// Move along pos. 
void Enemy::move(vec3 pos){
    this->position += pos;
}

void Enemy::draw(GLuint program, mat4 worldMatrix){
    // Move
	move(vec3{-speed, 0, 0});	// Todo maybe add lerping here (not moving x, moving yz)

	// Set model-world matrix
	mat4 total = worldMatrix * T(position.x, position.y, position.z) * S(10.0, 10.0, 10.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_world"), 1, GL_TRUE, total.m);

	// Draw
	DrawModel(model, program, "in_Position", "in_Normal", "inTexCoord");
}
