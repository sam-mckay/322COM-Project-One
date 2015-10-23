#pragma once
#include "Shape.h"
class Box :	public Shape
{
public:
	Box();
	Box(glm::vec3 *position, glm::vec3 colour, glm::vec3 *size);
	~Box();
	//properties
	glm::vec3 *size;
	//methods
	bool intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir);
	glm::vec3 getNormal(glm::vec3 intersectionPoint);
};

