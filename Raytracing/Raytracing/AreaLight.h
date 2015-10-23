#pragma once
#include "Box.h"
#include "List.h"
class AreaLight : public Box
{
public:
	AreaLight();
	~AreaLight();
	AreaLight(glm::vec3 *position, glm::vec3 colour, glm::vec3 *size, float lightIntesity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	//properties
	float lightIntensity;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	List<glm::vec3> *samplePointList = new List<glm::vec3>();
	void samplingPoints(int samples);
	void getPoint(glm::vec3 startPos, glm::vec3 endPos);
};

