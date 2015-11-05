#include "Light.h"


Light::Light()
{
}

Light::Light(glm::vec3 *position, glm::vec3 colour, double r, float lI, glm::vec3 a, glm::vec3 d, glm::vec3 s) :Sphere(position, colour, r)
{
	lightIntensity = lI;
	ambient = a;
	diffuse = d;
	specular = s;
}

Light::~Light()
{
}
