// Project main file


// Includes
#define MAIN
#include <cmath>
#include <cstdlib> // For random
#include <algorithm>
#include <vector>
#include <csignal>

// My own includes
#include "./GameData/Spaceship.h"
#include "./GameData/Bullet.h"
#include "./GameData/Enemy.h"
#include "./GameData/frustum_culling.h"
#include "./GameData/Controls.h"



// Lookat
vec3 cameraPoint;
vec3 lookatPoint;
mat4 look, worldMatrix, projectionMatrix;

// Timer
GLfloat t;

// Planes
FrustumCulling frustumCulling;


// Models
Model* world;
Model* spaceshipModel;
Model* bulletModel;
Model* enemyModel;
Model* crosshairModel;

// GameData
Spaceship spaceship;
std::vector<Bullet*> bullets;
std::vector<Enemy*> enemies;

// Constants
const float GAME_SPEED = 0.25f;
const float MOVE_SPEED = 2.0f;
const float BULLET_SPEED = 4.0f;

// Shooting
const double SHOOTING_TIME = 2.0;
const int BULLET_DAMAGE = 1;

// For enemies
const std::pair<int, int> SPAWNER_COOLDOWN {7, 10};	
const int ENEMY_HEALTH = 1;
const int ENEMY_DAMAGE = 1;
const int ENEMY_SPEED = 1.0f;
std::chrono::time_point<std::chrono::system_clock> enemySpawnTime;
float enemySpawnCooldown = 2.0f;


// World
const double PROJECTION_FAR = 200.0;
const double PROJECTION_NEAR = 0.1;
const double SPAWN_DISTANCE = 40.0;
mat4 spaceshipModelMatrix;


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

// For movement controls
float velocityY{0.0f};
float velocityZ{0.0f};
float rotAngleX{0.0f};
float rotAngleZ{0.0f};
vec3 nextPosition{0,0,0};


// Programs
GLuint program;

// Prototypes

// Enemy spawn
void enemySpawner();
void spawnEnemy();

// Draw functions
void drawWorld();
void drawSpaceship();
void drawEnemy(Enemy* e);

// Collision functions
void detectEnemySpaceshipCollision(Enemy* e, float radius);
bool checkEnemyBulletCollision(Enemy* e, float radius);


void handleControls(){
	rotAngleX = spaceship.getRotAngleX();
	rotAngleZ = spaceship.getRotAngleZ();

	// Handle rotation and movement velocity. 
    handleInputsAngles(rotAngleX, rotAngleZ, spaceship.getPosition());
    handleInputs(velocityY, velocityZ, nextPosition, spaceship.getPosition());

	// Set next position by velocity
	nextPosition.x = GAME_SPEED;
	nextPosition.y += velocityY;
	nextPosition.z += velocityZ;
	

    // If Pressing space and can shoot, then spawn bullet at spaceship position. 
	if (glutKeyIsDown(32) && !spaceship.getIsShooting()){
		spaceship.shoot();
		vec3 bullPos = calculateBulletDirection(rotAngleZ, rotAngleX);
		bullets.push_back(new Bullet(bulletModel, 1, BULLET_SPEED, spaceship.getCrosshairPosition(), BULLET_DAMAGE, bullPos));
	}

	// Setting lookat(world-view) matrix here
	cameraPoint.x += GAME_SPEED;	// Game increase x-axis
	look = lookAtv(cameraPoint, cameraPoint + lookatPoint, vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(program, "lookat"), 1, GL_TRUE, look.m);

}



void loadModels(){
	world = LoadDataToModel(vertices, vertex_normals, tex_coords, colors, indices, 4, 6);	// Or 4*3

	spaceshipModel = LoadModel("../Models/teapot.obj");
	bulletModel = LoadModel("../Models/groundsphere.obj");
	enemyModel = LoadModel("../Models/teddy.obj");

	// From https://www.cgtrader.com/items/92541/download-page
	crosshairModel = LoadModel("../Models/crosshair.obj");

	
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


// Based on collision2-surfaces-multiobj-little-city.c by Ingemar Ragnemalm
void detectEnemySpaceshipCollision(Enemy* e, float radius){
	vec3 diff;
	diff = spaceship.getPosition() - e->getPosition(); // Position difference

	std::cout << Norm(diff) << std::endl;

	if (Norm(diff) < radius) // Close enough to collide? Using Euclidian distance. 
	{
		std::cout << "Enemy spaceship collision!\n";
	}
}

// Check if this enemy collide with bullet, if so then return true. 
bool checkEnemyBulletCollision(Enemy* e, float radius){

	for (int i = 0; i < bullets.size(); ++i) {
		Bullet* b = bullets[i];
		vec3 diff = b->getPosition() - e->getPosition(); // Position difference

		if (Norm(diff) < radius) {
			// Enemy takes damage. 
			e->takeDamage(b->getDamage());

			// Erase the bullet
			delete b;
			bullets.erase(bullets.begin() + i); 

			// Return death status
			return e->getIsDead();
		}
	}
	return false;
}



void init(void)
{
	loadModels();
	initTextures();
	GLInits();
    loadShaders();	

	// Projection
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, PROJECTION_NEAR, PROJECTION_FAR);	// far = 50.0, near = 0.2
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix.m);

	// Lookat matrix init
	cameraPoint = vec3(-20.0, 10.4, 0);	// x=-30.3, y, z=1.42
	lookatPoint = vec3(2.82, 0, 0);		// 0.19
	look = lookAtv(cameraPoint, cameraPoint + lookatPoint, vec3(0, 1, 0));

	// Frustum culling
	frustumCulling = FrustumCulling(cameraPoint, cameraPoint + lookatPoint, PROJECTION_NEAR, PROJECTION_FAR);

	// Model-world matrix
	worldMatrix = IdentityMatrix();

	// Spaceship
	spaceship = Spaceship(spaceshipModel, 100, MOVE_SPEED, vec3(0.0f, 0.0f, 0.0f), 0);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the screen
	glDepthFunc(GL_LESS);

	// Continous rotation
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	// Enemy spawner handler
	enemySpawner();

	// Input handler
	handleControls();
	
	// Set projection. 
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, worldMatrix.m);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix.m);

	// Frustum culling
	frustumCulling.updatePlanes(cameraPoint, cameraPoint + lookatPoint, PROJECTION_NEAR, PROJECTION_FAR);

	// Todo bind textures here. 

	
	// Using a seperate loop to avoid pointer/iterator invalidation. 
	for (auto it = enemies.begin(); it != enemies.end();) {
		 Enemy* e = *it;
		if (frustumCulling.IsInsidePlane(frustumCulling.getNearPlane(), e->getPosition(), 1.0f)){
			std::cout << "Enemy free check!\n";
			delete e;
			it = enemies.erase(it);
		}
		else{
			it++;
		}
	}
	// Bullet free check. 
	// Using a seperate loop to avoid pointer/iterator invalidation. 
	for (auto it = bullets.begin(); it != bullets.end();) {
		Bullet* bullet = *it;
		if (frustumCulling.IsInsidePlane(frustumCulling.getFarPlane(), bullet->getPosition(), 40.0f)){
			std::cout << "Bullet free!";
			delete bullet;
			it = bullets.erase(it);
		}
		else{
			it++;
		}
	}


	// Collision checks Enemy with spaceship and bullets
	for (auto it = enemies.begin(); it != enemies.end();) {
		Enemy* e = *it;
		detectEnemySpaceshipCollision(e, 8.0f);
		if(checkEnemyBulletCollision(e, 4.0f)){

			
			delete e;
			it = enemies.erase(it);
		}
		else{
			it++;
		}
	}
	

	// Draw the furthest objects first
	drawWorld();

	// Draw enemies
	for (Enemy* e : enemies){
		drawEnemy(e);
	}

	// Detect bullet collisions
	for (Bullet* b : bullets){
		b->draw(program, worldMatrix);
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

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);	
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
	// Set random position within 
	vec3 pos = spaceship.getPosition();
	pos.x += SPAWN_DISTANCE;
	int randZ = rand() % 5 - 2;
	int randY = rand() % 5 + 9;	// Todo might need fixing these random coordinates. 

	//std::cout << randZ << ", " << randY << std::endl;

	pos.y = randY;
	pos.z = randZ;

	enemies.push_back(new Enemy(enemyModel, ENEMY_HEALTH, 0.0f, pos, ENEMY_DAMAGE));
}


// Draw functions


// Todo fix using other code
void drawWorld(){
	glUseProgram(program);
	mat4 trans = T(0.0f, 0, 0.0f);
	mat4 scale = S(500.0f, 0.1f, 500.0f);
	mat4 rotation = Ry(0);
	mat4 modification = trans * scale * rotation;
	mat4 total = worldMatrix * modification;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, total.m);

	DrawModel(world, program, "in_Position", "in_Normal", "inTexCoord");

}

void drawSpaceship(){
	glUseProgram(program);

	// Draw crosshair first. 
	vec3 posCross = spaceship.getCrosshairPosition();
	mat4 modification2 = T(posCross.x, posCross.y, posCross.z);
	mat4 rotation2 = Rz(90.0f);
	mat4 scaling2 = S(0.2, 0.2, 0.2);
	mat4 total2 = worldMatrix * modification2 * rotation2 * scaling2;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, total2.m);
	DrawModel(crosshairModel, program, "in_Position", "in_Normal", "inTexCoord");


	// Draw spaceship here
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
	mat4 scaling = S(1.0, 1.0, 1.0);
	spaceshipModelMatrix = worldMatrix * modification * rotation * scaling;
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, spaceshipModelMatrix.m);

	// Draw
	DrawModel(spaceship.getModel(), program, "in_Position", "in_Normal", "inTexCoord");

}

void drawEnemy(Enemy* e){
	// Move
	e->move(vec3{-ENEMY_SPEED, 0, 0});	// Todo add lerping here (not moving x, moving yz)

	// Set model-view matrix
	vec3 pos = e->getPosition();
	mat4 modification = worldMatrix * T(pos.x, pos.y, pos.z) * S(10.0, 10.0, 10.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, modification.m);


	// Draw
	DrawModel(e->getModel(), program, "in_Position", "in_Normal", "inTexCoord");

}
