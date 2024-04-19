// Project main file



// Includes
#include "GL_utilities.h"
#include "MicroGlut.h"
#define MAIN
#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include "LoadTGA.h"
#include <cmath>
#include <cstdlib> // For random
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <csignal>

// My own includes
// Todo add to makefile aswell
#include "./GameData/Spaceship.h"
#include "./GameData/Bullet.h"
#include "./GameData/Enemy.h"



// Lookat
// Todo fix later
vec3 cameraPoint;
vec3 lookatPoint;
mat4 look, modelView, projectionMatrix;

// Timer
GLfloat t;


// Models
Model* world;
Model* spaceshipModel;
Model* bulletModel;
Model* enemyModel;

// GameData
Spaceship spaceship;
std::vector<Bullet*> bullets;
std::vector<Enemy*> enemies;

// Constants
const float GAME_SPEED = 0.25f;
const float MOVE_SPEED = 2.0f;
const float BULLET_SPEED = 0.5f;

// Shooting
const double SHOOTING_TIME = 2.0;
const int BULLET_DAMAGE = 1;

// For enemies
const std::pair<int, int> SPAWNER_COOLDOWN {2, 7};
const int ENEMY_HEALTH = 1;
const int ENEMY_DAMAGE = 1;
const int ENEMY_SPEED = 1.0f;
std::chrono::time_point<std::chrono::system_clock> enemySpawnTime;
float enemySpawnCooldown = 2.0f;


// Screen offset boundaries
const std::pair<int, int> OFFSET_SCREEN_Z {-9, 17};
const std::pair<int, int> OFFSET_SCREEN_Y {-6, 20};



// World
const double X_FAR = 16.0;
#define kGroundSize 100.0f
vec3 vertices[] =
{
 vec3(-kGroundSize,0.0f,-kGroundSize),
 vec3(-kGroundSize,0.0f,kGroundSize),
 vec3(kGroundSize,-0.0f,-kGroundSize),
 vec3(kGroundSize,-0.0f,kGroundSize)
};

vec3 vertex_normals[] =
{
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f)
};

vec2 tex_coords[] =
{
  vec2(0.0f,0.0f),
  vec2(0.0f,20.0f),
  vec2(20.0f,0.0f), 
  vec2(20.0f,20.0f)
};
GLuint indices[] =
{
  0, 1, 2, 1, 3, 2
};

vec3 colors[] = 
{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};


// Programs
GLuint program;

// Prototypes
void loadModels();
void initTextures();

void handleInputs();
void handleInputsAngles();
void handleMovement(float& velocity, char moveKey, float acceleration);
void handleAngle(float& angle, char angleKey, float acceleration);


bool isOutsideFrustum(vec3 const otherPos);
bool isOutsideFrustumNear(vec3 const otherPos);


void enemySpawner();
void spawnEnemy();

void drawWorld();
void drawSpaceship();
void drawBullet(Bullet* b);
void drawEnemy(Enemy* e);

// Todo testing, rename!
float velY = 0;
float velZ = 0;
float rotAngleX{0.0f};
float rotAngleZ{0.0f};
vec3 nextPosition{0,0,0};


const float ACCELERATION_VERTICAL = 0.2f;
const float ACCELERATION_HORIZONTAL = 0.1f;
const float MAX_VELOCITY_HORIZONTAL = 3.0f;
const float MAX_VELOCITY_VERTICAL = 3.0f;
const float FRICTION_COEFFICIENT_ACC = 3.0f;
const float FRICTION_COEFFICIENT_DEC = 0.5f;

const float ANGULAR_ACCELERATION_X = 0.3f; 
const float ANGULAR_ACCELERATION_Z = 0.15f;
const float MAX_ROT_X = 1.0f; 	
const float MAX_ROT_Z = 0.5f;	
const float ANGULAR_FRICTION_ACC = 0.95;
const float ANGULAR_FRICTION_DEC = 0.95;



void handleAcceleration(float& movement, char moveKey, float acceleration, float friction){
	if (glutKeyIsDown(moveKey)){
		movement += acceleration * friction;
	}
}


// If not pressing either key1 nor key2. Then apply deacceleration friction to the data. 
void handleDeacceleration(float& movement, char key1, char key2, float friction){
	if (!(glutKeyIsDown(key1) || glutKeyIsDown(key2))) {
		movement *= friction;
		if (fabs(movement) < 0.01f) {
			movement = 0.0f;
		}
	}
}

void handleInputsAngles(){
	vec3 spaceshipPos = spaceship.getPosition();
	rotAngleX = spaceship.getRotAngleX();
	rotAngleZ = spaceship.getRotAngleZ();

	// Handle rotations
	if (spaceshipPos.y < OFFSET_SCREEN_Y.second){
		handleAcceleration(rotAngleZ, 'w', ANGULAR_ACCELERATION_Z, ANGULAR_FRICTION_ACC);
	}
	if (spaceshipPos.y > OFFSET_SCREEN_Y.first){
		handleAcceleration(rotAngleZ, 's', -ANGULAR_ACCELERATION_Z, ANGULAR_FRICTION_ACC);
	}
	if (spaceshipPos.z > OFFSET_SCREEN_Z.first){
		handleAcceleration(rotAngleX, 'a', -ANGULAR_ACCELERATION_X, ANGULAR_FRICTION_ACC);
	}
	if (spaceshipPos.z < OFFSET_SCREEN_Z.second){
		handleAcceleration(rotAngleX, 'd', ANGULAR_ACCELERATION_X, ANGULAR_FRICTION_ACC);
	}

	// Angle friction deacceleration
	handleDeacceleration(rotAngleX, 'a', 'd', ANGULAR_FRICTION_DEC);
	handleDeacceleration(rotAngleZ, 'w', 's', ANGULAR_FRICTION_DEC);

	// Max Angle checks. 
	rotAngleZ = rotAngleZ < -MAX_ROT_Z ? -MAX_ROT_Z : rotAngleZ;
	rotAngleZ = rotAngleZ > MAX_ROT_Z ? MAX_ROT_Z : rotAngleZ;

	rotAngleX = rotAngleX < -MAX_ROT_X ? -MAX_ROT_X : rotAngleX;
	rotAngleX = rotAngleX > MAX_ROT_X ? MAX_ROT_X : rotAngleX;
	

}

// WS = Move up and down
// AD = Move left and right
void handleInputs(){
	// Next position of spaceship. 
	nextPosition = vec3{GAME_SPEED,0,0};
	vec3 spaceshipPos = spaceship.getPosition();

	// Move Up W
	if (spaceshipPos.y < OFFSET_SCREEN_Y.second){
		handleAcceleration(velY, 'w', ACCELERATION_VERTICAL, FRICTION_COEFFICIENT_ACC);
	}
	else{
		if (!glutKeyIsDown('s')){velY = 0.0f;}
	}


	// Move Down S
	if (spaceshipPos.y > OFFSET_SCREEN_Y.first){
		handleAcceleration(velY, 's', -ACCELERATION_VERTICAL, FRICTION_COEFFICIENT_ACC);
	}
	else{
		if (!glutKeyIsDown('w')){velY = 0.0f;}
	}
	

	// Move Left A
	if (spaceshipPos.z > OFFSET_SCREEN_Z.first){
		handleAcceleration(velZ, 'a', -ACCELERATION_HORIZONTAL, FRICTION_COEFFICIENT_ACC);		
	}
	else{
		if (!glutKeyIsDown('d')){velZ = 0.0f;}
	}

	// Move Right D
	if (spaceshipPos.z < OFFSET_SCREEN_Z.second){
		handleAcceleration(velZ, 'd', ACCELERATION_HORIZONTAL, FRICTION_COEFFICIENT_ACC);	
	}
	else{
		if (!glutKeyIsDown('a')){velZ = 0.0f;}
	}

	// Handle friction deacceleration
	handleDeacceleration(velZ, 'a', 'd', FRICTION_COEFFICIENT_DEC);
	handleDeacceleration(velY, 'w', 's', FRICTION_COEFFICIENT_DEC);

	
	// If Pressing space and can shoot
	if (glutKeyIsDown(32) && !spaceship.getIsShooting()){
		// Spawn bullet as my position
		spaceship.shoot();
		bullets.push_back(new Bullet(bulletModel, 1, BULLET_SPEED, spaceship.getPosition(), BULLET_DAMAGE));
	}

	// Max velocity checks. 
	velZ = velZ < -MAX_VELOCITY_HORIZONTAL ? -MAX_VELOCITY_HORIZONTAL : velZ;
	velZ = velZ > MAX_VELOCITY_HORIZONTAL ? MAX_VELOCITY_HORIZONTAL : velZ;

	velY = velY < -MAX_VELOCITY_VERTICAL ? -MAX_VELOCITY_VERTICAL : velY;
	velY = velY > MAX_VELOCITY_VERTICAL ? MAX_VELOCITY_VERTICAL : velY;


	// Here cameraPoint.z += velZ and y!
	nextPosition.y += velY;
	nextPosition.z += velZ;

	// send camera position to shader, camera pos already in world cooridnates. 
	//glUniform3f(glGetUniformLocation(program, "camera_pos"), cameraPoint.x, cameraPoint.y, cameraPoint.z);

	// Lookat here
	cameraPoint.x += GAME_SPEED;	// Game increase x-axis
	look = lookAtv(cameraPoint, cameraPoint + lookatPoint, vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(program, "lookat"), 1, GL_TRUE, look.m);
}

void loadModels(){
	world = LoadDataToModel(vertices, vertex_normals, tex_coords, colors, indices, 4, 6);	// Or 4*3

	spaceshipModel = LoadModel("../Models/teapot.obj");
	bulletModel = LoadModel("../Models/groundsphere.obj");
	enemyModel = LoadModel("../Models/teddy.obj");

	
	// Todo add more models here
}

void initTextures(){
	// todo add textures here. 
}



// Init stuff
void GLInits(){
    //dumpInfo();
	glClearColor(0.2,0.2,0.5,0);

	// Enable Z-buffer and culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	printError("GL inits");
}

// Init shaders and compile
void loadShaders(){
    program = loadShaders("shader.vert", "shader.frag");
	printError("init shader");
}

void init(void)
{
	loadModels();
	initTextures();
	GLInits();
    loadShaders();	

	// Projection
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix.m);

	// Lookat matrix init
	cameraPoint = vec3(-30.3, 10.4, 1.42);
	lookatPoint = vec3(2.82, 0, 0.19);
	look = lookAtv(cameraPoint, cameraPoint + lookatPoint, vec3(0, 1, 0));

	// Model-view
	modelView = IdentityMatrix();

	// Spaceship
	spaceship = Spaceship(spaceshipModel, 100, MOVE_SPEED, vec3(5.0f, 0.0f, 3.0f), 0);

	// Enemies
	enemySpawnTime = std::chrono::system_clock::now();
	srand(time(nullptr));	// Random number generator


	
	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	
	// Predisplay
	printError("pre display");
	glClear(GL_COLOR_BUFFER_BIT);	// clear the screen

	// Continous rotation
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	// Input handler
	handleInputs();
	handleInputsAngles();
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, modelView.m);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix.m);

	// Todo bind textures here. 

	// Enemy spawner handler
	enemySpawner();

	// Draw the furthest objects first
	drawWorld();

	// Draw enemies
	// Using a seperate loop to avoid pointer/iterator invalidation. 
	for (auto it = enemies.begin(); it != enemies.end();) {
		 Enemy* e = *it;
		if (isOutsideFrustumNear(e->getPosition())){
			std::cout << "FREE!!";
			delete e;
			it = enemies.erase(it);
		}
		else{
			it++;
		}
	}

	// Detect enemy collisions
	for (Enemy* e : enemies){
		drawEnemy(e);
	}


	// Bullet free check. 
	// Using a seperate loop to avoid pointer/iterator invalidation. 
	for (auto it = bullets.begin(); it != bullets.end();) {
		 Bullet* bullet = *it;
		if (isOutsideFrustum(bullet->getPosition())){
			std::cout << "FREE!!";
			delete bullet;
			it = bullets.erase(it);
		}
		else{
			it++;
		}
	}


	// Detect bullet collisions
	for (Bullet* b : bullets){
		drawBullet(b);
		// Todo check world-bullet collision?
	}

	drawSpaceship();

	// Post display
	printError("display");
	glutSwapBuffers();
}

// Freeing memory when exiting the window. 
void signalHandler(int signum){
	for (Bullet* b : bullets){
		delete b;
	}
	bullets.clear();	// Pointer invalidation clear

	for (Enemy* e : enemies){
		delete e;
	}
	enemies.clear();
	exit(signum);
}


int main(int argc, char *argv[])
{
	signal(SIGINT, signalHandler);	// Free dynamic memory

	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);

	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitContextVersion(3, 2);

	glutInitWindowSize(600, 600);
	glutCreateWindow ("Project");

	// Animation
	glutRepeatingTimer(150);

	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}


// Frustum far plane
bool isOutsideFrustum(vec3 const otherPos){
	return abs(spaceship.getPosition().x - otherPos.x) > X_FAR;
}
// Near plane
bool isOutsideFrustumNear(vec3 const otherPos){
	return otherPos.x - spaceship.getPosition().x < -37.0f;	// Todo weird constant
}


// Enemy spawn:
void enemySpawner(){
	auto now = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration<double>(now - enemySpawnTime).count();
	if (elapsed >= enemySpawnCooldown){
		// Spawn new Enemy and push back to vector. 
		spawnEnemy();

		// Reset cooldown and spawn time.
		enemySpawnCooldown = rand() % SPAWNER_COOLDOWN.second + SPAWNER_COOLDOWN.first;
		enemySpawnTime = std::chrono::system_clock::now();
	}

	
}
void spawnEnemy(){
	// Set random position within y[1,2] and z[10, 11.8]
	// Random decimals via y [10,20] then divided by 10. 
	vec3 pos = spaceship.getPosition();
	pos.x += X_FAR;
	int randZ = rand() % 10 + 11;
	int randY = rand() % 19 + 100;

	std::cout << "y: " << randY/10.0f << ", z: " << randZ/10.0f << std::endl;

	pos.y = randY / 10.0f;
	pos.z = randZ / 10.0f;

	enemies.push_back(new Enemy(enemyModel, ENEMY_HEALTH, 0.0f, pos, ENEMY_DAMAGE));
}


// Draw functions


// Todo fix using other code
void drawWorld(){
	glUseProgram(program);
	mat4 trans = T(0.0f, 0, 0.0f);
	mat4 scale = S(500.0f, 0.5f, 500.0f);
	mat4 rotation = Ry(0);
	mat4 modification = trans * scale * rotation;
	mat4 total = modification;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, total.m);


	DrawModel(world, program, "in_Position", "in_Normal", "inTexCoord");

}

void drawSpaceship(){
	glUseProgram(program);

	// If spaceship is shooting and time has elapsed, then allow shooting again. 
	if (spaceship.getIsShooting() && spaceship.isShootCooldownWlapsed(SHOOTING_TIME)){
		spaceship.setIsShooting(false);
	}

	// Move and rotate
	spaceship.move(nextPosition);
	spaceship.setRotAngleX(rotAngleX);
	spaceship.setRotAngleZ(rotAngleZ);

	// Set Model-view matrix
	vec3 pos = spaceship.getPosition();
	mat4 modification = T(pos.x, pos.y, pos.z);
	mat4 rotation = Rx(rotAngleX) * Rz(rotAngleZ);
	mat4 total = modification * rotation;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, total.m);

	// Draw
	DrawModel(spaceship.getModel(), program, "in_Position", "in_Normal", "inTexCoord");

}

void drawBullet(Bullet* bullet){
	// Move
	bullet->move(vec3{BULLET_SPEED,0,0});

	// Set model-view matrix
	vec3 pos = bullet->getPosition();
	mat4 modification = T(pos.x, pos.y, pos.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, modification.m);

	// Draw
	DrawModel(bullet->getModel(), program, "in_Position", "in_Normal", "inTexCoord");
}

void drawEnemy(Enemy* e){
	// Move
	e->move(vec3{-ENEMY_SPEED,0,0});	// Todo add lerping here (not moving x, moving yz)

	// Set model-view matrix
	vec3 pos = e->getPosition();
	mat4 modification = T(pos.x, pos.y, pos.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, modification.m);

	// Draw
	DrawModel(e->getModel(), program, "in_Position", "in_Normal", "inTexCoord");
}
