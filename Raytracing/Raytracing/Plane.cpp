#include "Plane.h"


Plane::Plane()
{
}

Plane::Plane(glm::vec3 *position, glm::vec3 colour, glm::vec3 *n, double w, double h) :Shape(position, colour)
{
	width = w;
	height = h;
	normal = n;
}


Plane::~Plane()
{
}

bool::Plane::intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir, double *distance)
{
	//rayOrigin
	//L0

	//planeOrigin (position)
	//P0

	//normal 
	//n

	//normalise ray direction
	//L
	rayDir = glm::normalize(rayDir);

	//calculate distance to plane  
	//where t = ((L0 - P0).n) / (L.n)
		
	//origin to plane direction
	glm::vec3 planeDir = *rayOrigin - *position;
	
	//dot product planeDir with the normal
	double planeDotNorm = glm::dot(planeDir, *normal);
	
	//dot product rayDir with normal
	double rayDotNorm = glm::dot(rayDir, *normal);

	//t
	double tPlaneDistance = 0-(planeDotNorm/rayDotNorm);

	*distance = tPlaneDistance;

	if (tPlaneDistance >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

glm::vec3 Plane::getNormal(glm::vec3 intersectionPoint)
{
	return *normal;
}