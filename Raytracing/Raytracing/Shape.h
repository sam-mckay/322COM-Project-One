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
	//double t;
	void drawLine(glm::vec3 **view, int x, int y1, int y2, int z, glm::vec3 colour);
	//methods
	virtual bool intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir, double *distance) = 0; //virtual function - not used in this class 
	virtual glm::vec3 getNormal(glm::vec3 intersectionPoint) =0; //virtual function - not used in this class 
	//virtual void draw(glm::vec3 **view) = 0; //virtual function - not used in this class 
};

