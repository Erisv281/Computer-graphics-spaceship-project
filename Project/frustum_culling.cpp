

#include "frustum_culling.h"


FrustumCulling::FrustumCulling(){
    this->farPlane = vec4(0,0,0,0);
    this->nearPlane = vec4(0,0,0,0);
}

FrustumCulling::FrustumCulling(vec3 camPos, vec3 lookatPoint, float near, float far){
    updatePlanes(camPos, lookatPoint, near, far);
}

void FrustumCulling::updatePlanes(vec3 camPos, vec3 lookatPoint, float near, float far){
    // Calculate the directions
    vec3 forwardDir = vec3(1,0,0);  //Normalize(lookatPoint - camPos);
    vec3 rightDir = vec3(0,0,1); //Normalize(CrossProduct(vec3(0,1,0), forwardDir));
    vec3 upDir = vec3(0,1,0); //Normalize(CrossProduct(rightDir, forwardDir));

    // Calculate the points of the planes
    vec3 nearPoint = camPos + forwardDir * near;
    vec3 farPoint = camPos + forwardDir * far;

    // Creating the planes by using the plane equation
    this->farPlane = calculatePlane(upDir, rightDir, farPoint);
    this->nearPlane = calculatePlane(upDir, rightDir, nearPoint);

}


// Return true if inside the plane, otherwise returns false. 
bool FrustumCulling::IsInsidePlane(vec4 plane, vec3 center, float radius) const{

    // Calc distance from object to plane. 
    float distanceToCenter = dot(vec3(plane), center) + plane.w; 

    // return whether the object is inside plane
    return fabs(distanceToCenter) < radius;

}



vec4 FrustumCulling::calculatePlane(vec3 a, vec3 b, vec3 p){
    // Use the plane equiation to calculate the plane spanned by a,b,p
    vec3 normal = Normalize(CrossProduct(a, b));	// normal = (A,B,C) Todo crossproduct instead of
	float D = -dot(normal, p);					// D

    return vec4(normal.x, normal.y, normal.z, D);
}
