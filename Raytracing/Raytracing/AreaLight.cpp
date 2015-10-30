#include "AreaLight.h"


AreaLight::AreaLight()
{
}

AreaLight::AreaLight(glm::vec3 *position, glm::vec3 colour, glm::vec3 *size, float lI, glm::vec3 a, glm::vec3 d, glm::vec3 s) :Box(position, colour, size)
{
	lightIntensity = lI;
	ambient = a;
	diffuse = d;
	specular = s;
	samplingPoints(1);
}


AreaLight::~AreaLight()
{
}


void::AreaLight::samplingPoints(int samples)
{
	//std::cout << "SHAPE Start POS: " << position->x << "," << position->y << "," << position->z << std::endl;
	//std::cout << "SHAPE End POS: " << size->x << "," << size->y << "," << size->z << std::endl;
	for (int i = 0; i < samples; i++)
	{
		for (int j = 0; j < samples; j++)
		{
			glm::vec3 *midPoint = new glm::vec3(position->x + ((size->z / samples)*i), position->y, position->z + ((size->x / samples)*j));
			samplePointList->insert(samplePointList->tail, new Node<glm::vec3>(midPoint));
			//std::cout << "ADDED VEC: i: " << i << " j: " << j << std::endl;
			//std::cout << "SHAPE Sample POS: " << midPoint->x << "," << midPoint->y << "," << midPoint->z << std::endl;
		}
	}
}

void::AreaLight::getPoint(glm::vec3 startPos, glm::vec3 endPos)
{
	glm::vec3 *midPoint;
	
	samplePointList->insert(samplePointList->tail, new Node<glm::vec3>(midPoint));
}