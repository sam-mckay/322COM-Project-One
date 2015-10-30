#include "RayCasting.h"
#include <ppl.h>
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

RayCasting::RayCasting(int width, int height, double aspect, double fov, AreaLight *light)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	ASPECT_RATIO = aspect;
	FOV = fov;
	areaLight = light;
}



//deconstructor
RayCasting::~RayCasting()
{
}



//ray casting
void::RayCasting::castRay(glm::vec3 *rayOrigin, glm::vec3 *cameraSpace, glm::vec3 **view, List<Shape> *shapeList, SDL_Surface* screenSurface)
{
	//SDL_FillRect(screenSurface, NULL, 0x000000);
	//std::cout << "RAY CAST" << std::endl;
	SDL_FillRect(screenSurface, NULL, 0x000000);

	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			view[i][j] = glm::vec3(1.0f, 1.0f, 1.0f);
		}
	}
	concurrency::parallel_for(int(0), SCREEN_WIDTH, [&](int i)
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
			glm::vec3 cameraTest;
			cameraTest.x = cameraX;
			cameraTest.y = cameraY;
			cameraTest.z = -1;

			//calculate ray
			glm::vec3 rayDir = cameraTest - *rayOrigin;
			rayDir = glm::normalize(rayDir);
			//represent rays as colour - debug
			/*
			view[i][j].x = rayDir.x;
			view[i][j].y = rayDir.y;
			view[i][j].z = rayDir.z;
			*/

			double prevShape = 1;
			double prevDist = -1;
			double currentDist = INFINITY;

			for (int k = 0; k < shapeList->getLength(); k++)
			{
				Shape *currentShape = shapeList->getNode(k)->value;
				//std::cout << "LOOP: " << k << std::endl;
				if (currentShape->intersection(rayOrigin, rayDir, &currentDist))
				{
					if (prevDist == -1 || (prevDist > 0 && currentDist < prevDist))
					{
						glm::vec3 colour = currentShape->colour;
						view[i][j].x = colour.r;
						view[i][j].y = colour.g;
						view[i][j].z = colour.b;
						prevDist = currentDist;
						//hardShadows(view, i, j, currentShape, rayOrigin, rayDir, shapeList, currentDist);
						softShadows(view, i, j, currentShape, rayOrigin, rayDir, shapeList, 4, currentDist);
						if (view[i][j].x != 0 && view[i][j].y != 0 && view[i][j].z != 0)
						{
							phongShading(view, i, j, currentShape, rayOrigin, rayDir, currentDist);
						}
					}
				}
			}
			setPixel(screenSurface, i, j, scaleColour(view[i][j].x), scaleColour(view[i][j].y), scaleColour(view[i][j].z));
		}
	//}
	});
}

//calculate phong shading for lighting
void::RayCasting::phongShading(glm::vec3 **view, int i, int j, Shape *currentShape, glm::vec3 *rayOrigin, glm::vec3 rayDir, double currentDist)
{
	
	//
	//
	//ambient lighting
	//
	//
	glm::vec3 ambient = areaLight->ambient * view[i][j] * areaLight->lightIntensity;

	//
	//
	//diffuse lighting
	//
	//
	glm::vec3 distanceVec(currentDist, currentDist, currentDist);
		//calculate intersection point
	glm::vec3 intersectionPoint = (distanceVec + *rayOrigin) * rayDir;
		//calculate lightRay and normals
	glm::vec3 lightRay = *areaLight->position - intersectionPoint;
	//glm::vec3 normal = intersectionPoint - *currentShape->position;
	glm::vec3 normal = currentShape->getNormal(intersectionPoint);
		//normalize
	lightRay = glm::normalize(lightRay);
	normal= glm::normalize(normal);
		//calculate diffuse
	glm::vec3 diffuse = view[i][j] * areaLight->diffuse * areaLight->lightIntensity * float(fmax(0, glm::dot(lightRay, normal)));

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
	specular = areaLight->specular * areaLight->lightIntensity * float(pow(fmax(0, glm::dot(reflectionLightRay, primaryRay)), 128));
	
	
	//all together now...  / add all lighting types together
	if (diffuse.x > 0)
	{
		view[i][j] = ambient + diffuse  + specular;
	}
	else
	{
		view[i][j] = ambient + diffuse;
	}
}


//could opptimise by calling in phong shader
//if 0 is returned exit phong shader? 
void::RayCasting::hardShadows(glm::vec3 **view, int i, int j, Shape *currentShape, glm::vec3 *rayOrigin, glm::vec3 rayDir, List<Shape> *shapeList, double distance)
{
	//create light ray from point of intersection to the light source
	glm::vec3 *shadowRayOrigin;
	glm::vec3 shadowRayDir;

	//calculate direction from intersection point to light source
		//calculate distance vector
	glm::vec3 distanceVec(distance, distance, distance);
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
		if (currentShape->intersection(shadowRayOrigin, shadowRayDir, &distance))
		{
			view[i][j] = glm::vec3(0, 0, 0);
		}
	}
}


void::RayCasting::softShadows(glm::vec3 **view, int i, int j, Shape *currentShape, glm::vec3 *rayOrigin, glm::vec3 rayDir, List<Shape> *shapeList, int samples, double distance)
{
	int hitCount = 0;
	//create light ray from point of intersection to the light source
	glm::vec3 *shadowRayOrigin;
	glm::vec3 shadowRayDir;

	//calculate direction from intersection point to light source
	//calculate distance vector
	glm::vec3 distanceVec(distance, distance, distance);
	for (int k = 0; k < samples*samples; k++)	
	{
		bool hit = false;
		
		//calculate intersection point
		glm::vec3 intersectionPoint = (distanceVec + *rayOrigin) * rayDir;
		//light source - intersection point = direction to point on light source
		List<glm::vec3> *currentPosOnLight = areaLight->samplePointList;
		shadowRayDir = glm::normalize(*currentPosOnLight->getNode(k)->value - intersectionPoint);

		//calculate shadow ray origin and add precision error correction
		//get normal from shape at intersection point
		glm::vec3 normal = glm::normalize(currentShape->getNormal(intersectionPoint));
		//glm::vec3 normal = currentShape->getNormal(intersectionPoint);
		shadowRayOrigin = &(intersectionPoint + (normal * float(0.1)));

		//check for intersections with spheres
		for (int k = 0; k < shapeList->getLength(); k++)
		{
			Shape *currentShape = shapeList->getNode(k)->value;
			if (currentShape->intersection(shadowRayOrigin, shadowRayDir, &distance))
			{
				hit = true;
				hitCount += 1;
				break;
			}
		}
	}
	//std::cout << "Hit count: " << hitCount << std::endl;
	view[i][j].x -= ((view[i][j].x / pow(samples, 2)) * hitCount);
	view[i][j].y -= ((view[i][j].y / pow(samples, 2)) * hitCount);
	view[i][j].z -= ((view[i][j].z / pow(samples, 2)) * hitCount);
}

//drawPixel
bool::RayCasting::setPixel(SDL_Surface* screenSurface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{

	Uint32 colour;
	colour = SDL_MapRGB(screenSurface->format, r, g, b);

	Uint32 *pixelMemory32;
	pixelMemory32 = (Uint32*)screenSurface->pixels + y * screenSurface->pitch / 4 + x;
	*pixelMemory32 = colour;

	return 1;
}

float::RayCasting::scaleColour(float colour)
{
	float c = colour * 255;
	if (c < 0)
	{
		c = 0;
	}
	return c;
}