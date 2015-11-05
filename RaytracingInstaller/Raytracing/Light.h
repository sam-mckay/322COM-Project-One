#pragma once
#include "Sphere.h"
class Light : public Sphere
{
public:
	Light();
	Light(glm::vec3 *position, glm::vec3 colour, double r, float lightIntesity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~Light();

	//properties
	float lightIntensity;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

