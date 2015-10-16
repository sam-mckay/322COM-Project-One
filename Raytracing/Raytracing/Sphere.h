#pragma once
#include "Shape.h"
class Sphere :public Shape
{
public:
	Sphere();
	Sphere(glm::vec3 *position, glm::vec3 colour, double radius);
	~Sphere();

	//properties
	double radius;
	bool intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir);
	glm::vec3 getNormal(glm::vec3 intersectionPoint);
	//void draw(glm::vec3 **view);
};

