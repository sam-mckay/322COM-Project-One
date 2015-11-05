#pragma once
#include "Shape.h"
class Sphere :public Shape
{
public:
	Sphere();
	Sphere(glm::vec3 *position, glm::vec3 colour, float radius);
	~Sphere();

	//properties
	float radius;
	float diameter;
	bool intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir, double *distance);
	glm::vec3 getNormal(glm::vec3 intersectionPoint);
	//void draw(glm::vec3 **view);
};

