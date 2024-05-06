


// This class represents frustum culling planes for the near and the far plane of the game. 
// This class is based on the current source (https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling)


#ifndef _FC
#define _FC

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"
#include "VectorUtils4.h"
#include "LoadTGA.h"
#include <iostream>


class FrustumCulling{
private:
    vec4 nearPlane;
    vec4 farPlane;

public:
    // Creating planes
    FrustumCulling();
    FrustumCulling(vec3 camPos, float near, float far);
    void updatePlanes(vec3 camPos, float near, float far);

    // Getters
    inline vec4 getFarPlane() const { return farPlane; }
    inline vec4 getNearPlane() const { return nearPlane; }

    // Checker
    bool IsInsidePlane(vec4 plane, vec3 center, float radius) const;

    // Helper to calculate the plane. 
    vec4 calculatePlane(vec3 a, vec3 b, vec3 p);


};

#endif