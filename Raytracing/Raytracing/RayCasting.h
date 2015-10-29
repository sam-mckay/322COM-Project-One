#pragma once

#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <math.h>
#include "List.h"
#include "glm.hpp"
#include "Sphere.h"
#include "Light.h"
#include "AreaLight.h"
#include "Shape.h"
#include "SDL.h"
class RayCasting
{
public:
	RayCasting();
	RayCasting(int width, int height, double aspect, double FOV, Light *mainLight);
	RayCasting(int width, int height, double aspect, double FOV, AreaLight *mainLight);
	~RayCasting();
	void castRay(glm::vec3 *rayOrigin, glm::vec3 *cameraSpace, glm::vec3 **view, List<Shape> *shapeList, SDL_Surface* screenSurface);
	void phongShading(glm::vec3 **view, int i, int j, Shape *currentShape, glm::vec3 *rayOrigin, glm::vec3 rayDir, double distance);
	void hardShadows(glm::vec3 **view, int i, int j, Shape *currentShape, glm::vec3 *rayOrigin, glm::vec3 rayDir, List<Shape> *shapeList, double distance);
	void softShadows(glm::vec3 **view, int i, int j, Shape *currentShape, glm::vec3 *rayOrigin, glm::vec3 rayDir, List<Shape> *shapeList, int samples, double distance);
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	double ASPECT_RATIO;
	double FOV;
	Light *mainLight;
	AreaLight *areaLight;
	List<glm::vec3> *samplePointList;
	bool setPixel(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);
	float scaleColour(float colour);
};

