#pragma once
#include "Shape.h"
class Plane : public Shape
{
public:
	Plane();
	Plane(glm::vec3 *position, glm::vec3 colour, glm::vec3 *normal, double width, double height);
	~Plane();

	//properties
	double width, height;
	glm::vec3 *normal;
	bool intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir);
};

