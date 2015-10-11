#pragma once
#include "Shape.h"
class Triangle : public Shape
{
public:
	Triangle();
	Triangle(glm::vec3 *position, glm::vec3 colour);
	~Triangle();
};

