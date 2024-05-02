

#include "Controls.h"



void handleInputsAngles(float& rotAngleX, float& rotAngleZ, vec3 spaceshipPos){

	// Handle rotations
	if (!isHittingBoundaryUp(spaceshipPos)){
		handleAcceleration(rotAngleZ, 'w', ANGULAR_ACCELERATION_Z, ANGULAR_FRICTION_ACC);
	}
	if (!isHittingBoundaryDown(spaceshipPos)){
		handleAcceleration(rotAngleZ, 's', -ANGULAR_ACCELERATION_Z, ANGULAR_FRICTION_ACC);
	}
	if (!isHittingBoundaryLeft(spaceshipPos)){
		handleAcceleration(rotAngleX, 'a', -ANGULAR_ACCELERATION_X, ANGULAR_FRICTION_ACC);
	}
	if (!isHittingBoundaryRight(spaceshipPos)){
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
void handleInputs(float& velY, float& velZ, vec3& nextPosition, vec3 spaceshipPos){
	// Next position of spaceship. 
	nextPosition = vec3{0,0,0};

	// Move Up W
	if (!isHittingBoundaryUp(spaceshipPos)){
		handleAcceleration(velY, 'w', ACCELERATION_VERTICAL, FRICTION_COEFFICIENT_ACC);
	}
	else{
		if (!glutKeyIsDown('s')){velY = 0.0f;}
	}


	// Move Down S
	if (!isHittingBoundaryDown(spaceshipPos)){
		handleAcceleration(velY, 's', -ACCELERATION_VERTICAL, FRICTION_COEFFICIENT_ACC);
	}
	else{
		if (!glutKeyIsDown('w')){velY = 0.0f;}
	}
	

	// Move Left A
	if (!isHittingBoundaryLeft(spaceshipPos)){
		handleAcceleration(velZ, 'a', -ACCELERATION_HORIZONTAL, FRICTION_COEFFICIENT_ACC);		
	}
	else{
		if (!glutKeyIsDown('d')){velZ = 0.0f;}
	}

	// Move Right D
	if (!isHittingBoundaryRight(spaceshipPos)){
		handleAcceleration(velZ, 'd', ACCELERATION_HORIZONTAL, FRICTION_COEFFICIENT_ACC);	
	}
	else{
		if (!glutKeyIsDown('a')){velZ = 0.0f;}
	}

	// Handle friction deacceleration
	handleDeacceleration(velZ, 'a', 'd', FRICTION_COEFFICIENT_DEC);
	handleDeacceleration(velY, 'w', 's', FRICTION_COEFFICIENT_DEC);

	// Max velocity checks. 
	velZ = velZ < -MAX_VELOCITY_HORIZONTAL ? -MAX_VELOCITY_HORIZONTAL : velZ;
	velZ = velZ > MAX_VELOCITY_HORIZONTAL ? MAX_VELOCITY_HORIZONTAL : velZ;

	velY = velY < -MAX_VELOCITY_VERTICAL ? -MAX_VELOCITY_VERTICAL : velY;
	velY = velY > MAX_VELOCITY_VERTICAL ? MAX_VELOCITY_VERTICAL : velY;
    
}




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


// Rotate in the y direction depending on the value of rotAngleZ
// And rotate in the opposite of rotAngleX in regards to the z-axis.  
vec3 calculateBulletDirection(float& rotAngleZ, float& rotAngleX){
	return vec3(0, rotAngleZ, -rotAngleX);
}


// Boundary checks
bool isHittingBoundaryUp(vec3 const pos){
	return pos.y >= OFFSET_SCREEN_Y.second;
}

bool isHittingBoundaryDown(vec3 const pos){
	return pos.y <= OFFSET_SCREEN_Y.first;
}

bool isHittingBoundaryRight(vec3 const pos){
	return pos.z >= OFFSET_SCREEN_Z.second;
}

bool isHittingBoundaryLeft(vec3 const pos){
	return pos.z <= OFFSET_SCREEN_Z.first;	// 
}