#include "RayCasting.h"

//constructor
RayCasting::RayCasting()
{
}

//new constructor
RayCasting::RayCasting(int width, int height, double aspect, double fov)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	ASPECT_RATIO = aspect;
	FOV = fov;
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

			for (int k = 0; k < shapeList->getLength(); k++)
			{
				Shape *currentShape = shapeList->getNode(k)->value;
				//std::cout << "LOOP: " << k << std::endl;
				if (currentShape->intersection(rayOrigin, rayDir))
				{
					if (prevShape == 1 || prevShape < 0 && currentShape->position->z > prevShape)
					{
						glm::vec3 colour = currentShape->colour;
						view[i][j].x = colour.r;
						view[i][j].y = colour.g;
						view[i][j].z = colour.b;
						prevShape = currentShape->position->z;
						//std::cout << "SUCCESS: COLOUR: " << colour.r << "," << colour.g << "," << colour.b << std::endl;
					}
				}
			}


		}
	}
}
