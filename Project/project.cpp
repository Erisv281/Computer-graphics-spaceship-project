// Project main file


// Includes
#define MAIN
#include <cmath>
#include <cstdlib> // For random
#include <algorithm>
#include <vector>
#include <csignal>
#include <string>

// My own includes
#include "./GameData/Spaceship.h"
#include "./GameData/Bullet.h"
#include "./GameData/Enemy.h"
#include "./GameData/frustum_culling.h"
#include "./GameData/Controls.h"
#include "./GameData/simplefont.h"



// Matrices
vec3 cameraPoint, lookatPoint;
mat4 look, worldMatrix, projectionMatrix;

// Timer
GLfloat t;

// Planes
FrustumCulling frustumCulling;


// Models
Model* spaceshipModel;
Model* bulletModel;
Model* enemyModel;
Model* crosshairModel;
Model* skybox;

// Tex references
GLuint skyBoxTex, texEnemy, texSpaceship, texBullet;	// 0 1 2 3

// GameData
Spaceship spaceship;
std::vector<Bullet*> bullets;
std::vector<Enemy*> enemies;

// Game states
bool isPlaying = true;
bool enableEnemySpawn = true;
bool enableInfiniteHealth = false;
bool enableShowCollisions = false;

// World constants
const double PROJECTION_FAR = 200.0;
const double PROJECTION_NEAR = 0.1;
const double SPAWN_DISTANCE = 80.0;

// Speed constants
const float GAME_SPEED = 0.25f;
const float MOVE_SPEED = 2.0f;
const float BULLET_SPEED = 10.0f;
const float ENEMY_SPEED = 0.7f;

// Bullets constants
const int BULLET_DAMAGE = 1;

// Enemies data
const std::pair<int, int> SPAWNER_COOLDOWN {2, 5};	
std::chrono::time_point<std::chrono::system_clock> enemySpawnTime;
float enemySpawnCooldown = 2.0f;
const int ENEMY_HEALTH = 1;
const int ENEMY_DAMAGE = 1;


// For movement controls
float velocityY{0.0f};
float velocityZ{0.0f};
float rotAngleX{0.0f};
float rotAngleZ{0.0f};
vec3 nextPosition{0,0,0};

// Programs
GLuint program;
GLuint programSky;

// Score system
int score = 0;
int highScore = 0;


// Switch the spawn status and removes all enemies
void switchEnemySpawn(bool status){
	enableEnemySpawn = status;
	for (Enemy* e : enemies){
		delete e;
	}
	enemies.clear();
}


// Pause game by disable enemy spawn and player. 
void loseGame(){
	isPlaying = false;

	// Set high score
	if (score > highScore){
		highScore = score;
	}

}


void resetGame(){
	isPlaying = true;
	switchEnemySpawn(true);

	// Respawn spaceship
	spaceship.setHealth(10);
	score = 0;
}



// Used for handling different game states
void keyboard(unsigned char c, int x, int y){
	switch (c)
	{
		// Enable/disable enemies by pressing k
		case 'k':
			if (isPlaying){
				switchEnemySpawn(!enableEnemySpawn);
			}
			break;

		// Infinite health by pressing l
		case 'l':
			enableInfiniteHealth = !enableInfiniteHealth;
			break;

		// Visualize collision borders
		case 'j':
			enableShowCollisions = !enableShowCollisions;
			break;
		// Play again
		case 'p':
			if (!isPlaying){
				resetGame();
			}
			break;

			
	}
}


// Used for handling movement controls
void handleSpaceshipControls(){
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
		bullets.push_back(new Bullet(bulletModel, 1, BULLET_SPEED, spaceship.getCrosshairPosition(), BULLET_DAMAGE, 2.0f, bullPos));
	}


	cameraPoint.x += GAME_SPEED;	// Game increase x-axis
	
}



void loadModels(){
	spaceshipModel = LoadModel("../Models/teapot.obj");
	bulletModel = LoadModel("../Models/groundsphere.obj");
	enemyModel = LoadModel("../Models/teddy.obj");
	skybox = LoadModelPlus("../Models/labskybox2.obj");	

	// From https://www.cgtrader.com/items/92541/download-page
	crosshairModel = LoadModel("../Models/crosshair.obj");


}

void initTextures(){
	// Load textures
	LoadTGATextureSimple("../Models/cloud-landscape.tga", &skyBoxTex);
	LoadTGATextureSimple("../Models/Textures/stone4_b.tga", &texEnemy);
	LoadTGATextureSimple("../Models/Textures/kt_rot_2.tga", &texSpaceship);
	LoadTGATextureSimple("../Models/Textures/kt_stone03.tga", &texBullet);

	// Texture 0 for skybox
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyBoxTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	// Tex 1 for enemies
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texEnemy);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 1);

	// Tex 2 for spaceship
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texSpaceship);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 2);

	// Tex 3 for bullet
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texBullet);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 3);

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
	programSky = loadShaders("skybox.vert", "skybox.frag");	
	printError("init shader");
}



// Based on collision2-surfaces-multiobj-little-city.c by Ingemar Ragnemalm
void detectEnemySpaceshipCollision(Enemy* e){
	vec3 diff = spaceship.getCenterPosition() - e->getCenterPosition(); // Center position difference
	float radius = spaceship.getRadius() + e->getRadius();	

	if (Norm(diff) < radius) // Close enough to collide? Using Euclidian distance. 
	{
		if (!enableInfiniteHealth){
			// Spaceship takes damage and maybe destroyed. 
			spaceship.takeDamage(e->getDamage());
			if (spaceship.getIsDead()){
				std::cout << "Spaceship down!\n";
				loseGame();
			}
		}

		
	}
}

// Check if this enemy collide with bullet, if so then return true. 
bool checkEnemyBulletCollision(Enemy* e){

	for (int i = 0; i < bullets.size(); ++i) {
		Bullet* b = bullets[i];
		vec3 diff = b->getCenterPosition() - e->getCenterPosition(); // Position center difference
		float radius = b->getRadius() + e->getRadius();	

		if (Norm(diff) < radius) {
			// Enemy takes damage. 
			e->takeDamage(b->getDamage());

			// Erase the bullet
			delete b;
			bullets.erase(bullets.begin() + i); 

			// Increase game score
			score += 10;

			// Return death status
			return e->getIsDead();
		}
	}
	return false;
}


// Draw the crosshair for the spaceship
void drawCrossHair(){
	// Set model-world matrix
	vec3 posCross = spaceship.getCrosshairPosition();
	mat4 totalCross = worldMatrix * T(posCross.x, posCross.y, posCross.z) * Rz(90.0f) * Rx(rotAngleX) * Rz(rotAngleZ) * S(0.75, 0.75, 0.75);

	// Draw model
	glUniformMatrix4fv(glGetUniformLocation(program, "model_world"), 1, GL_TRUE, totalCross.m);
	DrawModel(crosshairModel, program, "in_Position", "in_Normal", "inTexCoord");
}

void spawnEnemy(){
	// Set random position within 
	vec3 pos = spaceship.getPosition();
	pos.x += SPAWN_DISTANCE;
	int randZ = rand() % 33 - 16;	// Todo when doing rotations, use the boundary methods. 
	int randY = rand() % 35 - 9;	

	std::cout << randZ << ", " << randY << std::endl;

	pos.y = randY;
	pos.z = randZ;

	enemies.push_back(new Enemy(enemyModel, ENEMY_HEALTH, ENEMY_SPEED, pos, ENEMY_DAMAGE, 9.0f));
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


void drawSkybox(){
	// Using a seperate shader
	glUseProgram(programSky);	

	// Disable Z-buffer and backface culling
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Nullify the translation part of the lookat matrix
	mat4 newCamera = look;
	newCamera.m[3] = 0;
	newCamera.m[7] = 0;
	newCamera.m[11] = 0;
	mat4 total = newCamera * T(0, -0.3f, -0.5f);
	
	// Put matrix data to shaders and draw skybox
	glUniformMatrix4fv(glGetUniformLocation(programSky, "model_world"), 1, GL_TRUE, worldMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(programSky, "lookat"), 1, GL_TRUE, total.m);
	glUniformMatrix4fv(glGetUniformLocation(programSky, "projection"), 1, GL_TRUE, projectionMatrix.m);
	DrawModel(skybox, programSky, "in_Position", NULL, "inTexCoord");

	// Re-enable back face culling and z-test
	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_CULL_FACE);

	// Reset to origin shader
	glUseProgram(program);
}


// This code is based on Ingemar Ragnemalm's simplefont.c
void setFont(std::string s, int width, int height){
	sfSetFont(-1);	// Default font
	sfSetFontColor(1, 1, 1);

	// https://stackoverflow.com/questions/10847237/how-to-convert-from-int-to-char
	char const *pchar = s.c_str();  //use char const* as target type
	char* text = const_cast<char*>(pchar);
	sfDrawString(width, height, text);
}


// Draw bounding sphere around Entity
void displayCollisionBorders(Entity* e){
	
	float radius = e->getRadius();
	vec3 center = e->getCenterPosition();
	mat4 matrix = worldMatrix * T(center.x, center.y, center.z) * S(radius, radius, radius);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_world"), 1, GL_TRUE, matrix.m);

	// Draw
	DrawModel(bulletModel, program, "in_Position", "in_Normal", "inTexCoord");
}



void init(void)
{
	// Init models, textures, shaders
	loadModels();
	GLInits();
    loadShaders();	

	// Projection
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, PROJECTION_NEAR, PROJECTION_FAR);
	glUseProgram(programSky);
	glUniformMatrix4fv(glGetUniformLocation(programSky, "projection"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(programSky, "texUnit"), 0);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix.m);

	initTextures();

	// Lookat matrix init
	cameraPoint = vec3(-20.0, 10.4, 0);
	lookatPoint = vec3(2.82, 0, 0);	
	look = lookAtv(cameraPoint, cameraPoint + lookatPoint, vec3(0, 1, 0));

	// Frustum culling
	frustumCulling = FrustumCulling(cameraPoint, cameraPoint + lookatPoint, PROJECTION_NEAR, PROJECTION_FAR);

	// Model-world matrix
	worldMatrix = IdentityMatrix();

	// Spaceship
	spaceship = Spaceship(spaceshipModel, 10, MOVE_SPEED, vec3(0.0f, 0.0f, 0.0f), 0, 5.0f);

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

	// Continous time t
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000;

	if (isPlaying){
		// Control spaceship
		handleSpaceshipControls();
	}
	
	// Set lookat matrix
	glUseProgram(program);
	look = lookAtv(cameraPoint, cameraPoint + lookatPoint, vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(program, "lookat"), 1, GL_TRUE, look.m);

	// Set projection and model-to-world matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "model_world"), 1, GL_TRUE, worldMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix.m);

	// Draw skybox
	drawSkybox();
	

	// Use this font to show controls for resetting the game. 
	if (!isPlaying){
		setFont("Restart game: P", 200, 200);
	}
	else{
		// Enemy spawner handler
		if (enableEnemySpawn){
			enemySpawner();
		}

		// Frustum culling
		frustumCulling.updatePlanes(cameraPoint, cameraPoint + lookatPoint, PROJECTION_NEAR, PROJECTION_FAR);

		// Set spaceship movement and rotation.
		spaceship.move(nextPosition);
		spaceship.setRotAngleX(rotAngleX);
		spaceship.setRotAngleZ(rotAngleZ);

		
		// Check collision for each enemy between near frustum plane, spaceship and bullets. 
		for (auto it = enemies.begin(); it != enemies.end();) {
			Enemy* e = *it;

			// Check spaceship-Enemy collision
			detectEnemySpaceshipCollision(e);

			// Delete enemy if inside near plane or collide with bullets. 
			if (frustumCulling.IsInsidePlane(frustumCulling.getNearPlane(), e->getPosition(), 1.0f) || checkEnemyBulletCollision(e)){
				delete e;
				it = enemies.erase(it);
			}
			else{
				it++;
			}
		}
		// Check collision between each bullet and the far plane
		for (auto it = bullets.begin(); it != bullets.end();) {
			Bullet* bullet = *it;
			if (frustumCulling.IsInsidePlane(frustumCulling.getFarPlane(), bullet->getPosition(), 40.0f)){
				delete bullet;
				it = bullets.erase(it);
			}
			else{
				it++;
			}
		}


		// Draw enemies, rotating around their arbitrary axis
		glActiveTexture(GL_TEXTURE1);	
		glUniform1i(glGetUniformLocation(program, "texUnit"), 1);
		for (Enemy* e : enemies){
			mat4 rotation = ArbRotate(e->getPosition(), t*0.5f);
			e->draw(program, worldMatrix * rotation);

			if (enableShowCollisions){
				displayCollisionBorders(e);
			}

			
		}

		// Draw bullets
		glActiveTexture(GL_TEXTURE3);
		glUniform1i(glGetUniformLocation(program, "texUnit"), 3);
		for (Bullet* b : bullets){
			b->draw(program, worldMatrix);
		}


		// Draw spaceship and crosshair
		drawCrossHair();
		glActiveTexture(GL_TEXTURE2);
		glUniform1i(glGetUniformLocation(program, "texUnit"), 2);
		spaceship.draw(program, worldMatrix);

		if (enableShowCollisions){
				displayCollisionBorders(&spaceship);
		}

	
		// UI Fonts for score
		setFont("Score: " + std::to_string(score), 100, 100);
		setFont("High score: " + std::to_string(highScore), 100, 120);
		
		// UI for health
		std::string healthString = enableInfiniteHealth ? "oo" : std::to_string(spaceship.getHealth());
		setFont("Health: " + healthString, 100, 140);


		// UI for controls
		setFont("Move: WASD", 400, 100);
		setFont("Collisions: J", 400, 120);
		setFont("Enemy spawn: K", 400, 140);
		setFont("Inf health: L", 400, 160);


		// Extra check to remove all enemies if player death
		if (!isPlaying){
			switchEnemySpawn(false);
		}
	
	}


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
	// Free dynamic memory
	signal(SIGINT, signalHandler);

	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);	
	glutInitWindowSize(600, 600);
	glutCreateWindow ("Project");

	// Animation
	glutRepeatingTimer(150);

	// Init
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);	// Using keyboard
	init ();
	glutMainLoop();
	return 0;
}






