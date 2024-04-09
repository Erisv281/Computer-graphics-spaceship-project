// Project main file



// Includes
#include "GL_utilities.h"
#include "MicroGlut.h"
#define MAIN
#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include "LoadTGA.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <csignal>
#include <chrono>	// For timers

// My own includes
// Todo add to makefile aswell
#include "./GameData/Spaceship.h"
#include "./GameData/Bullet.h"



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

// GameData
Spaceship spaceship;
std::vector<Bullet*> bullets;

// Constants
const float GAME_SPEED = 0.25f;
const float MOVE_SPEED = 2.0f;
const float ROT_SPEED = 0.2f;

const float BULLET_SPEED = 0.5f;

// Shooting
const double SHOOT_INTERVAL = 2.0;
std::chrono::duration<double> shootingTime;
std::chrono::steady_clock::time_point endTime;	// Todo something with this

// Screen offset boundaries
const std::pair<float, float> OFFSET_SCREEN_Z {-9.0f, 17.0f};
const std::pair<float, float> OFFSET_SCREEN_Y {-6.0f, 20.0f};

const std::pair<float, float> OFFSET_ROT_X {-1.0f, 1.0f};
const std::pair<float, float> OFFSET_ROT_Z {-0.5f, 0.5f};

// Misc. 
vec3 nextPosition{0,0,0};
float rotAngleX{0.0f};
float rotAngleZ{0.0f};


// World
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
void handleInputs();
void loadModels();
void initTextures();

void drawWorld();
void drawSpaceship();
void drawBullet(Bullet* b);

// Todo fix later
void handleInputs(){
	// WS = Move up and down
	// AD = Move left and right

	// Next position of spaceship. 
	nextPosition = vec3{GAME_SPEED,0,0};
	float moveSpeed = spaceship.getSpeed();
	vec3 spaceshipPos = spaceship.getPosition();
	rotAngleX = spaceship.getRotAngleX();
	rotAngleZ = spaceship.getRotAngleZ();

	if (glutKeyIsDown('w') && spaceshipPos.y < OFFSET_SCREEN_Y.second){
		nextPosition.y += moveSpeed;

		if (rotAngleZ < OFFSET_ROT_Z.second){
			rotAngleZ += ROT_SPEED;
		}
		
	}
	if (glutKeyIsDown('s') && spaceshipPos.y > OFFSET_SCREEN_Y.first){
		nextPosition.y -= moveSpeed;

		if (rotAngleZ > OFFSET_ROT_Z.first){
			rotAngleZ -= ROT_SPEED;
		}
	}
	if (glutKeyIsDown('a') && spaceshipPos.z > OFFSET_SCREEN_Z.first){
		nextPosition.z -= moveSpeed;

		if (rotAngleX > OFFSET_ROT_X.first){
			rotAngleX -= ROT_SPEED;
		}
		
	}
	if (glutKeyIsDown('d') && spaceshipPos.z < OFFSET_SCREEN_Z.second){
		nextPosition.z += moveSpeed;
		if (rotAngleX < OFFSET_ROT_X.second){
			rotAngleX += ROT_SPEED;
		}
	}

	
	// If Pressing space and can shoot
	if (glutKeyIsDown(32) && !spaceship.getIsShooting()){
		// Spawn bullet as my position
		std::cout << "Shoot!";
		spaceship.shoot();
		// Todo start cooldown timer. 
		bullets.push_back(new Bullet(bulletModel, 1, BULLET_SPEED, spaceship.getPosition()));
	}



	// send camera position to shader, camera pos already in world cooridnates. 
	//glUniform3f(glGetUniformLocation(program, well within the lab tim"camera_pos"), cameraPoint.x, cameraPoint.y, cameraPoint.z);

	// Lookat here
	cameraPoint.x += GAME_SPEED;	// Game increase x-axis
	look = lookAtv(cameraPoint, cameraPoint + lookatPoint, vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(program, "lookat"), 1, GL_TRUE, look.m);
}

void loadModels(){
	world = LoadDataToModel(vertices, vertex_normals, tex_coords, colors, indices, 4, 6);	// Or 4*3

	spaceshipModel = LoadModel("../Models/teapot.obj");
	bulletModel = LoadModel("../Models/groundsphere.obj");

	
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
	spaceship = Spaceship(spaceshipModel, 100, MOVE_SPEED, vec3(5.0f, 0.0f, 3.0f), SHOOT_INTERVAL);


	
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
	glUniformMatrix4fv(glGetUniformLocation(program, "model_view"), 1, GL_TRUE, modelView.m);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix.m);

	// Todo bind textures here. 

	// Draw the furthest objects first
	drawWorld();

	

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
	std::cout << "here free data\n";
	for (Bullet* b : bullets){
		delete b;
	}
	bullets.clear();	// Pointer invalidation clear
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

	// Move and rotate
	spaceship.move(nextPosition);
	spaceship.setRotAngleX(rotAngleX);
	spaceship.setRotAngleZ(rotAngleZ);

	// Todo check timer heer. 

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
