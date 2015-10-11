#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <math.h>
#include "glm.hpp"
class Shape
{
public:
	Shape();
	Shape(glm::vec3 *position, glm::vec3 colour);
	~Shape();
	//properties
	glm::vec3 *position;
	glm::vec3 colour;
	void drawLine(glm::vec3 **view, int x, int y1, int y2, int z, glm::vec3 colour);
	//methods
	virtual bool intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir) = 0; //virtual function - not used in this class 
	virtual void draw(glm::vec3 **view) = 0; //virtual function - not used in this class 
};

