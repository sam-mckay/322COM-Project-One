#pragma once

#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <math.h>
#include "List.h"
#include "glm.hpp"
#include "Sphere.h"
#include "Light.h"
#include "Shape.h"
class RayCasting
{
public:
	RayCasting();
	RayCasting(int width, int height, double aspect, double FOV, Light *mainLight);
	~RayCasting();
	void castRay(glm::vec3 *rayOrigin, glm::vec3 *cameraSpace, glm::vec3 **view, List *shapeList);
	void phongShading(glm::vec3 **view, int i, int j, Shape *currentShape, glm::vec3 *rayOrigin, glm::vec3 rayDir);
	void hardShadows(glm::vec3 **view, int i, int j, Shape *currentShape, glm::vec3 *rayOrigin, glm::vec3 rayDir, List *shapeList);
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	double ASPECT_RATIO;
	double FOV;
	Light *mainLight;
	
};

