#include "Triangle.h"


Triangle::Triangle()
{
}

Triangle::Triangle(glm::vec3 *p1, glm::vec3 *p2, glm::vec3 *p3, glm::vec3 colour) :Shape(p1, colour)
{
	point1 = p1;
	point2 = p2;
	point3 = p3;
}

Triangle::~Triangle()
{
}

bool::Triangle::intersection(glm::vec3 *rayOrigin, glm::vec3 rayDir, double *distance)
{
	//std::cout << "rayOrigin" << rayOrigin->x << "," << rayOrigin->y << "," << rayOrigin->z << std::endl;
	
	//normal ray direction
		//d
	//rayDir = glm::normalize(rayDir);
	//a = p1 
	//b = p2 
	//c = p3
	//std::cout << "RayDir" << rayDir.x << "," << rayDir.y << "," << rayDir.z << std::endl;
	//get vector between triangle vertices
		//e1 = b - a
	glm::vec3 aToB = *point2 - *point1;
		//e2 = c - a
	glm::vec3 aToC = *point3 - *point1;
	//std::cout << "AToB: " << aToB.x << ", " << aToB.y << ", " << aToB.z << std::endl;
	//std::cout << "AToC: " << aToC.x << ", " << aToC.y << ", " << aToC.z << std::endl;
	//calculate barycentric coords
		//u = ((o-a).(d*e2))/e1.(d*e2)
	
	float u = (glm::dot((*rayOrigin - *point1), (glm::cross(rayDir, aToC))) / glm::dot(aToB, (glm::cross(rayDir,aToC))));
		//v = d.((o-a)*e1)/e1.(d*e2)
	float v = (glm::dot(rayDir, (glm::cross((*rayOrigin - *point1), aToB)))) / glm::dot(aToB, (glm::cross(rayDir, aToC)));
	//w = 1 - u -v  // u + v + w = 1
	float w = 1 - u - v;
	//express any point in triangle with
		//p = ua + vb + wc

	glm::vec3 intersectionPoint = ( *point1*u + *point2*v + *point3*w);
	


	*distance = glm::distance(intersectionPoint, *rayOrigin);

	//check if point is in unit triangle
	// 0 <= u,v,w <= 1
	//std::cout << "U: " << float(u) << "," << "V: " << float(v) << "," << "W: " << w << std::endl;
	if (u<=0.0f || u>=1.0f)
	{
		//std::cout << "FAIL 1" << std::endl;
		return false;
	}
	else if (v<=0.0f || u+v>=1.0f)
	{
		//std::cout << "FAIL 2" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "SUCCESS" << std::endl;
		return true;
	}
}

glm::vec3 Triangle::getNormal(glm::vec3 intersectionPoint)
{
	//side1 = b - a
	glm::vec3 aToB = *point2 - *point1;
	//side2 = c - a
	glm::vec3 aToC = *point3 - *point1;

	return glm::cross(aToB, aToC);
}