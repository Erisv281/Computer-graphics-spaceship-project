

#include "Controls.h"



void handleInputsAngles(float& rotAngleX, float& rotAngleZ, vec3 spaceshipPos){

	// Handle rotations
	if (spaceshipPos.y < OFFSET_SCREEN_Y.second){   // todo fix these borders
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
void handleInputs(float& velY, float& velZ, vec3& nextPosition, vec3 spaceshipPos){
	// Next position of spaceship. 
	nextPosition = vec3{0,0,0};

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


// rotAngleZ for rotating the direction y and rotAngleX for rotating the direction Z. 
vec3 calculateBulletDirection(float& rotAngleZ, float& rotAngleX){
	return vec3(0, rotAngleZ, rotAngleX);
}