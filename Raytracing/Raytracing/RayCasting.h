#pragma once

#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <math.h>
#include "List.h"
#include "glm.hpp"
class RayCasting
{
public:
	RayCasting();
	RayCasting(int width, int height, double aspect, double FOV);
	~RayCasting();
	void castRay(glm::vec3 *rayOrigin, glm::vec3 *cameraSpace, glm::vec3 **view, List *shapeList);
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	double ASPECT_RATIO;
	double FOV;
	
};

