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
	float t1;
	float t2;
	glm::vec3 intersectionDist;
	float epsilon = 0.001f;
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

	//*distance = double(glm::distance(*rayOrigin, intersectionDist));
	*distance = double(glm::distance(*rayOrigin, *position));

	//check intersection point lies on cube face
	if (p1.x > (position->x - epsilon) && p1.x < (farPosPoint.x + epsilon) &&
		p1.y > (position->y - epsilon) && p1.y < (farPosPoint.y + epsilon) &&
		p1.z > (position->z - epsilon)&& p1.z < (farPosPoint.z + epsilon))
	{
		//std::cout << "SUCCESS 0" << std::endl;
		return true;
	}
	if (p2.x > position->x - epsilon && p2.x < farPosPoint.x + epsilon &&
		p2.y > position->y - epsilon && p2.y < farPosPoint.y + epsilon &&
		p2.z > position->z - epsilon && p2.z < farPosPoint.z + epsilon)
	{
		//std::cout << "SUCCESS 1" << std::endl;
		return true;
	}
	if (p3.x > position->x - epsilon && p3.x < farPosPoint.x + epsilon &&
		p3.y > position->y - epsilon && p3.y < farPosPoint.y + epsilon &&
		p3.z > position->z - epsilon && p3.z < farPosPoint.z + epsilon)
	{
		//std::cout << "SUCCESS 2" << std::endl;
		return true;
	}
	//std::cout << "FAIL 2" << std::endl;

	return false;
}

glm::vec3 Box::getNormal(glm::vec3 intersectionPoint)
{
	float epsilon = 0.001f;
	//v2
	glm::vec3 farPosPoint = *position + *size;
	/*
	std::cout << "INTERSECTION " << intersectionPoint.x << "," << intersectionPoint.y << "," << intersectionPoint.z << std::endl;

	std::cout << "POSITION " << position->x << "," << position->y << "," << position->z << std::endl;
	std::cout << "FAR POSITION " << farPosPoint.x << "," << farPosPoint.y << "," << farPosPoint.z << std::endl;
	*/
	if (intersectionPoint.x > (position->x - epsilon) && intersectionPoint.x < (farPosPoint.x + epsilon) &&
		intersectionPoint.y >(position->y - epsilon) && intersectionPoint.y < (farPosPoint.y + epsilon) &&
		intersectionPoint.z >(position->z - epsilon) && intersectionPoint.z < (farPosPoint.z + epsilon))
	{
		std::cout << "SUCCESS 0" << std::endl;
		return glm::vec3(1, 0, 0);
	}
	if (intersectionPoint.x > position->x - epsilon && intersectionPoint.x < farPosPoint.x + epsilon &&
		intersectionPoint.y > position->y - epsilon && intersectionPoint.y < farPosPoint.y + epsilon &&
		intersectionPoint.z > position->z - epsilon && intersectionPoint.z < farPosPoint.z + epsilon)
	{
		std::cout << "SUCCESS 1" << std::endl;
		return glm::vec3(0, 1, 0);
	}
	if (intersectionPoint.x > position->x - epsilon && intersectionPoint.x < farPosPoint.x + epsilon &&
		intersectionPoint.y > position->y - epsilon && intersectionPoint.y < farPosPoint.y + epsilon &&
		intersectionPoint.z > position->z - epsilon && intersectionPoint.z < farPosPoint.z + epsilon)
	{
		std::cout << "SUCCESS 2" << std::endl;
		return glm::vec3(0, 0, 1);
	}
	return glm::vec3(1,0,0);
}

