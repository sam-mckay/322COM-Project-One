#include "Sphere.h"


Sphere::Sphere()
{	
}

//Parameter = double (Radius)
Sphere::Sphere(glm::vec3 *position, glm::vec3 colour, float r) :Shape(position, colour)
{
	radius = r;
	diameter = r * r;
}

Sphere::~Sphere()
{
}

bool::Sphere::intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir, double *distance)
{
	//std::cout << "RAY SPHERE INTERSECTION: POS: " << position->x << "," << position->y << "," << position->y <<
	//	" RAY: " << rayOrigin->x << "," << rayOrigin->y << "," << rayOrigin->y << std::endl;

	//get distance between camera and Sphere position
	//store rayOrigin in temp vector
	//L 
	glm::vec3 objectDistance = *position - *rayOrigin;

	//get unit vector for object distance
	//d
	glm::vec3 rayDirUnit = glm::normalize(rayDir);

	//get distance between camera and current point (in line with radius)
	//Tca
	double rayDistance = glm::dot(objectDistance, rayDirUnit);
	//std::cout << "ray distance: " << rayDistance << std::endl;

	//check if less than 0 
	//return false if < 0 as object is behind camera
	if (rayDistance < 0)
	{
		//std::cout << "FAILED - CHECK 1 - NO INTERSECTION" << std::endl;
		return false;
	}

	//distance beteween current point and sphere position
	//s2
	double centreToCurrentPoint = glm::dot(objectDistance, objectDistance) - (rayDistance *rayDistance);

	//std::cout << "dot product result: " << glm::dot(objectDistance, objectDistance) << std::endl;
	//std::cout << "origin to current point: " << centreToCurrentPoint << std::endl;
	
	//check if current point to origin is further than radius
	//return false if so as no intersection
	if (centreToCurrentPoint > diameter)
	{
		//std::cout << "FAILED - CHECK 2 - NO INTERSECTION" << std::endl;
		return false;
	}

	//calculate distance to edge of sphere from current point
		//thc
	double distanceToSphereEdge = sqrt(pow(radius, 2) - centreToCurrentPoint);

	//std::cout << "Distance to edge of sphere: " << distanceToSphereEdge << std::endl;

	if (rayDistance - distanceToSphereEdge < rayDistance + distanceToSphereEdge)
	{
		*distance = rayDistance - distanceToSphereEdge;
		//std::cout << "Sphere T: " << t << std::endl;
	}
	else
	{
		*distance = rayDistance + distanceToSphereEdge;
		//std::cout << "Sphere T: " << t << std::endl;
	}

	//
	//std::cout << "SUCCESS - INTERSECTION" << std::endl;
	return true;
}

glm::vec3 Sphere::getNormal(glm::vec3 intersectionPoint)
{
	glm::vec3 normal = intersectionPoint - *position;
	return normal;
}

/*

void Sphere::draw(glm::vec3 **view)
{
	//calculate width of circle at specific point in circle
	//iterate through circle pixel by pixel
	//debug
	//radius = 100;
	for (int i = 0; i < radius*2; i++)
	{
		//sqrt 8xrxh - 4 h^2
		double height = i;
		double width = sqrt((8 * radius*height) - (4 * pow(height, 2)));
		drawLine(view, position->x +i - radius, position->y-width, position->y + width, position->z, colour);
		//std::cout << "DRAW LINE: " << i << " width: " << width << std::endl;
	}

	//drawLine(view, position->x, position->y, position->y + 200, position->z, colour);


}*/