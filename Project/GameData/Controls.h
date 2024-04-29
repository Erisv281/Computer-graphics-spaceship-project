
// This file handle controls for velocity and rotation change. 

#ifndef _CONTROLS
#define _CONTROLS

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include "LoadTGA.h"
#include <iostream>

// Velocity constants
const float ACCELERATION_VERTICAL = 0.2f;
const float ACCELERATION_HORIZONTAL = 0.1f;
const float MAX_VELOCITY_HORIZONTAL = 3.0f;
const float MAX_VELOCITY_VERTICAL = 3.0f;
const float FRICTION_COEFFICIENT_ACC = 3.0f;
const float FRICTION_COEFFICIENT_DEC = 0.5f;

// Rotation constants
const float ANGULAR_ACCELERATION_X = 0.3f; 
const float ANGULAR_ACCELERATION_Z = 0.15f;
const float MAX_ROT_X = 1.0f; 	
const float MAX_ROT_Z = 0.5f;	
const float ANGULAR_FRICTION_ACC = 0.95;
const float ANGULAR_FRICTION_DEC = 0.95;

// Screen offset boundaries
const std::pair<int, int> OFFSET_SCREEN_Z {-36, 36};
const std::pair<int, int> OFFSET_SCREEN_Y {-32, 42};

void handleInputs(float& velY, float& velZ, vec3& nextPosition, vec3 spaceshipPos);

void handleInputsAngles(float& rotAngleX, float& rotAngleZ, vec3 spaceshipPos);

void handleDeacceleration(float& movement, char key1, char key2, float friction);

void handleAcceleration(float& movement, char moveKey, float acceleration, float friction);


vec3 calculateBulletDirection(float& rotAngleZ, float& rotAngleX);

#endif