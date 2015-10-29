#include "Box.h"


Box::Box()
{
}

Box::Box(glm::vec3 *position, glm::vec3 colour, glm::vec3 *s) :Shape(position, colour)
{
	size = s;
}

Box::~Box()
{
}

bool::Box::intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir, double *distance)
{
	double t1;
	double t2;
	glm::vec3 intersectionDist;
	double epsilon = 0.001;
		//v2
	glm::vec3 farPosPoint = *position+*size;
	//find closest distances
		//x
	if (rayDir.x)
	{
		t1 = (position->x - rayOrigin->x)/ rayDir.x;
		t2 = (farPosPoint.x - rayOrigin->x) / rayDir.x;
		//set x to closest dist
		intersectionDist.x = fmin(t1, t2);
	}
		//y
	if (rayDir.y)
	{
		t1 = (position->y - rayOrigin->y) / rayDir.y;
		t2 = (farPosPoint.y-rayOrigin->y) / rayDir.y;
		//set y to closest dist
		intersectionDist.y = fmin(t1, t2);
	}
		//z
	if (rayDir.z)
	{
		t1 = (position->z - rayOrigin->z) / rayDir.z;
		t2 = (farPosPoint.z - rayOrigin->z) / rayDir.z;
		//set z to closest point
		intersectionDist.z = fmin(t1, t2);
	}
		

	//get intersection point
	glm::vec3 p1 = *rayOrigin + rayDir * intersectionDist.x;
	glm::vec3 p2 = *rayOrigin + rayDir * intersectionDist.y;
	glm::vec3 p3 = *rayOrigin + rayDir * intersectionDist.z;

	//check intersection point lies on cube face
	if (p1.x > (position->x - epsilon) && p1.x < (farPosPoint.x + epsilon) &&
		p1.y > (position->y - epsilon) && p1.y < (farPosPoint.y + epsilon) &&
		p1.z > (position->z - epsilon)&& p1.z < (farPosPoint.z + epsilon))
	{
		//std::cout << "SUCCES 0" << std::endl;
		return true;
	}
	if (p2.x > position->x - epsilon && p2.x < farPosPoint.x + epsilon &&
		p2.y > position->y - epsilon && p2.y < farPosPoint.y + epsilon &&
		p2.z > position->z - epsilon && p2.z < farPosPoint.z + epsilon)
	{
		//std::cout << "SUCCES 1" << std::endl;
		return true;
	}
	if (p3.x > position->x - epsilon && p3.x < farPosPoint.x + epsilon &&
		p3.y > position->y - epsilon && p3.y < farPosPoint.y + epsilon &&
		p3.z > position->z - epsilon && p3.z < farPosPoint.z + epsilon)
	{
		//std::cout << "SUCCES 2" << std::endl;
		return true;
	}
	//std::cout << "FAIL 2" << std::endl;
	return false;
}

glm::vec3 Box::getNormal(glm::vec3 intersectionPoint)
{
	return intersectionPoint;
}

