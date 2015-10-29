#pragma once
#include "Shape.h"
class Triangle : public Shape
{
public:
	Triangle();
	Triangle(glm::vec3 *point1, glm::vec3 *point2, glm::vec3 *point3, glm::vec3 colour);
	~Triangle();

	//properties
		//a
	glm::vec3 *point1;
		//b
	glm::vec3 *point2;
		//c
	glm::vec3 *point3;

	//methods
	bool intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir, double *distance);
	glm::vec3 getNormal(glm::vec3 intersectionPoint);
};

