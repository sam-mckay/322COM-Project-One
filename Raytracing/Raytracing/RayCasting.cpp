#include "RayCasting.h"

//constructor
RayCasting::RayCasting()
{
}

//new constructor
RayCasting::RayCasting(int width, int height, double aspect, double fov, Light *light)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	ASPECT_RATIO = aspect;
	FOV = fov;
	mainLight = light;
}


//deconstructor
RayCasting::~RayCasting()
{
}



//ray casting
void::RayCasting::castRay(glm::vec3 *rayOrigin, glm::vec3 *cameraSpace, glm::vec3 **view, List *shapeList)
{
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			//normalise pixel position to range 0:1
			double pixelNormalisedX = (i + 0.5) / SCREEN_WIDTH;
			double pixelNormalisedY = (j + 0.5) / SCREEN_HEIGHT;
			//std::cout << "view: " << view[i][j].x << ", " << view[i][j].y << std::endl;
			//remap the coordinates from 0:1 to -1:1
			double pixelRemappedX = (2 * pixelNormalisedX - 1) * ASPECT_RATIO;
			double pixelRemappedY = 1 - 2 * pixelNormalisedY;

			//incorporate field of view (FOV)
			double cameraX = pixelRemappedX * tan(FOV / 2);
			double cameraY = pixelRemappedY * tan(FOV / 2);
			//std::cout << "camera : " << cameraX << ", " << cameraY << std::endl;

			//represent camera position as vector
			cameraSpace = new glm::vec3(cameraX, cameraY, -1);

			//calculate ray
			glm::vec3 rayDir = *cameraSpace - *rayOrigin;
			rayDir = glm::normalize(rayDir);
			//represent rays as colour - debug
			/*
			view[i][j].x = rayDir.x;
			view[i][j].y = rayDir.y;
			view[i][j].z = rayDir.z;
			*/

			double prevShape = 1;
			double prevDist = -1;

			for (int k = 0; k < shapeList->getLength(); k++)
			{
				Shape *currentShape = shapeList->getNode(k)->value;
				//std::cout << "LOOP: " << k << std::endl;
				if (currentShape->intersection(rayOrigin, rayDir))
				{
					if (prevDist == -1 || (prevDist > 0 && currentShape->t < prevDist))
					{
						glm::vec3 colour = currentShape->colour;
						view[i][j].x = colour.r;
						view[i][j].y = colour.g;
						view[i][j].z = colour.b;
						prevDist = currentShape->t;
						hardShadows(view, i, j, currentShape, rayOrigin, rayDir, shapeList);
						if (view[i][j].x != 0 && view[i][j].y != 0 && view[i][j].z != 0)
							phongShading(view, i ,j, currentShape, rayOrigin, rayDir);
						
					}
				}
			}
		}
	}
}

//calculate phong shading for lighting
void::RayCasting::phongShading(glm::vec3 **view, int i, int j, Shape *currentShape, glm::vec3 *rayOrigin, glm::vec3 rayDir)
{
	//
	//
	//ambient lighting
	//
	//
	glm::vec3 ambient = mainLight->ambient * view[i][j] * mainLight->lightIntensity;

	//
	//
	//diffuse lighting
	//
	//
	glm::vec3 distanceVec(currentShape->t, currentShape->t, currentShape->t);
		//calculate intersection point
	glm::vec3 intersectionPoint = (distanceVec + *rayOrigin) * rayDir;
		//calculate lightRay and normals
	glm::vec3 lightRay = *mainLight->position - intersectionPoint;
	//glm::vec3 normal = intersectionPoint - *currentShape->position;
	glm::vec3 normal = currentShape->getNormal(intersectionPoint);
		//normalize
	lightRay = glm::normalize(lightRay);
	normal= glm::normalize(normal);
		//calculate diffuse
	glm::vec3 diffuse = view[i][j] * mainLight->diffuse * mainLight->lightIntensity * float(fmax(0, glm::dot(lightRay, normal)));

	//
	//
	//Specular lighting
	//
	//
	glm::vec3 specular;
		//calculate reflection of L
		//r = 2 * (L.N) * N - L
	glm::vec3 reflectionLightRay;
	reflectionLightRay = 2.0f * glm::dot(lightRay, normal) * normal - lightRay;
		//calculate primary ray direction
	glm::vec3 primaryRay(0, 0, 0);
	primaryRay = *rayOrigin - intersectionPoint;
		//normalise above vectors
	reflectionLightRay = glm::normalize(reflectionLightRay);
	primaryRay = glm::normalize(primaryRay);
		//calculate specular
	specular = mainLight->specular * mainLight->lightIntensity * float(pow(fmax(0, glm::dot(reflectionLightRay, primaryRay)), 128));
	
	
	//all together now...  / add all lighting types together
	if(diffuse.x > 0)
		view[i][j] = ambient + diffuse  + specular;
	else
	{
		view[i][j] = ambient + diffuse;
	}
}


//could opptimise by calling in phong shader
//if 0 is returned exit phong shader? 
void::RayCasting::hardShadows(glm::vec3 **view, int i, int j, Shape *currentShape, glm::vec3 *rayOrigin, glm::vec3 rayDir, List *shapeList)
{
	//create light ray from point of intersection to the light source
	glm::vec3 *shadowRayOrigin;
	glm::vec3 shadowRayDir;

	//calculate direction from intersection point to light source
		//calculate distance vector
	glm::vec3 distanceVec(currentShape->t, currentShape->t, currentShape->t);
		//calculate intersection point
	glm::vec3 intersectionPoint = (distanceVec + *rayOrigin) * rayDir;
		//light source - intersection point = direction to light source
	shadowRayDir = glm::normalize(*mainLight->position - intersectionPoint);

	//calculate shadow ray origin and add precision error correction
		//get normal from shape at intersection point
	glm::vec3 normal = glm::normalize(currentShape->getNormal(intersectionPoint));
	shadowRayOrigin = &(intersectionPoint + (normal * float(1e-4)));

	//check for intersections with spheres
	for (int k = 0; k < shapeList->getLength(); k++)
	{
		Shape *currentShape = shapeList->getNode(k)->value;
		if (currentShape->intersection(shadowRayOrigin, shadowRayDir))
		{
			view[i][j] = glm::vec3(0, 0, 0);
		}
	}
}