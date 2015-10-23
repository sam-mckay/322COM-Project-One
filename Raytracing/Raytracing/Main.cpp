// First Tutorial
// Sam McKay 1.10.14
//
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <math.h>
#include "glm.hpp"
#include "RayCasting.h"
#include "Shape.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Box.h"
#include "Light.h"
#include "AreaLight.h"
#include "List.h"
#include <SDL.h>
#include <stdio.h>
#include <chrono>
//clock for fps calc
typedef std::chrono::high_resolution_clock Clock;
Clock::time_point prevTime;
Clock::time_point currentTime;
int fpsCount = 0;
double frameTimeThisSec =0;
//SDL Set up
Uint8* inkeys;
SDL_Event event = { 0 };
//screen statics
int SCREEN_HEIGHT = 480;
int SCREEN_WIDTH = 640;
double ASPECT_RATIO = SCREEN_WIDTH / SCREEN_WIDTH;//image aspect ratio
double FOV = 60.0 *ASPECT_RATIO; //field of view
//position
double posX = 0, posY = 0;


//view array
glm::vec3 **view;
//camera array
glm::vec3 *cameraSpace;
//ray origin arry
glm::vec3 *rayOrigin;


//draws a single pixel to the screen
bool setPixel(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	
	Uint32 colour;
	colour = SDL_MapRGB(surface->format, r, g, b);

	Uint32 *pixelMemory32;
	pixelMemory32 = (Uint32*)surface->pixels + y * surface->pitch/4 + x;
	*pixelMemory32 = colour;

	return 1;
}

//scales colour value from index to rgb 0-255
//for all - old - no longer useful
/*
void scaleColour()
{
	for (unsigned y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for (unsigned x = 0; x < SCREEN_WIDTH; ++x)
		{
			view[x][y].r = (view[x][y].r+1)*255;
			view[x][y].g = (view[x][y].g + 1) * 255;
			view[x][y].b = (view[x][y].b + 1) * 255;
		}
	}
}
*/

//scales colour value from index to rgb 0-255
//for single colour value
float scaleColour(float colour)
{
	return colour * 255;
}

void draw(SDL_Surface* screenSurface)
{
	for (unsigned y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for (unsigned x = 0; x < SCREEN_WIDTH; ++x)
		{
			//setPixel(screenSurface, x, y, scaleColour(view[x][y].x), scaleColour(view[x][y].y), scaleColour(view[x][y].z));
			//std::cout << "loop: " << x << "," << y << "," << view[x][y].r << "," << view[x][y].g << "," << view[x][y].b << std::endl;
		}
	}
}


//saves the current draw view array to an image file
void saveToFile()
{
	std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << SCREEN_WIDTH << " " << SCREEN_HEIGHT << "\n255\n";
	for (unsigned y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for (unsigned x = 0; x < SCREEN_WIDTH; ++x)
		{
			ofs << (unsigned char)(std::fmin((float)1, (float)view[x][y].x) * 255) <<
				(unsigned char)(std::fmin((float)1, (float)view[x][y].y) * 255) <<
				(unsigned char)(std::fmin((float)1, (float)view[x][y].z) * 255);
		}
	}
	ofs.close();
}



double convertToRad(double deg)
{
	return (deg*M_PI) / 180;
}

double convertToDeg(double rad)
{
	return rad*(180 / M_PI);
}

//print array
void printArray()
{
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			std::cout << view[i][j].x << "," << view[i][j].y << "," << view[i][j].z << "  ,  ";
		}
		system("pause");
	}
}

//Returns 1 if you close the window or press the escape key. Also handles everything that's needed per frame.
//Never put key input code right before done() or SDL may see the key as SDL_QUIT
bool done(bool quit_if_esc, bool delay) //delay makes CPU have some free time, use once per frame to avoid 100% usage of a CPU core
{
	if (delay) SDL_Delay(5); //so it consumes less processing power
	int done = 0;
	if (!SDL_PollEvent(&event)) return 0;
	// read keys
	// User requests quit
	if (event.type == SDL_QUIT)
	{
		done = 1;
	}
	//User presses a key
	else if (event.type == SDL_KEYDOWN)
	{
		//Select surfaces based on key press
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			done = 1;
			break;

		default:
			break;
		}
	}
	return done;
}

//main function
int main(int argc, char* args[])
{
	//setup SDL window
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	//setup view plane
	view = new glm::vec3*[SCREEN_WIDTH];
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		view[i] = new glm::vec3[SCREEN_HEIGHT];
	}
	//convert FOV to radians
	FOV = convertToRad(FOV);
	// assign temp value to rayOrigin
	rayOrigin = new glm::vec3(0, 0, 0);
	//loop through and assign each a colour vector
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			view[i][j] = glm::vec3(1.0f, 1.0f, 1.0f);
		}
	}
	//set up light
	Light *mainLight = new Light(new glm::vec3(0, 20, 0), glm::vec3(1.0f, 1.0f, 1.0f), 4, 0.5f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.7, 0.7f, 0.7f));
	AreaLight *areaLight = new AreaLight(new glm::vec3(0, 20, 0), glm::vec3(1.0f, 1.0f, 1.0f), new glm::vec3(6, 6, 6), 0.5f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.7, 0.7f, 0.7f));
	//set up ray class
	RayCasting *rayCaster = new RayCasting(SCREEN_WIDTH, SCREEN_HEIGHT, ASPECT_RATIO, FOV, areaLight);
	//set up shapes
	//spheres
	Shape *redSphere = new Sphere(new glm::vec3(0, 0, -20), glm::vec3(1.0f, 0.32f, 0.36f), 4);
	Shape *yellowSphere = new Sphere(new glm::vec3(5, -1, -15), glm::vec3(0.90f, 0.76f, 0.46f), 2);
	Shape *blueSphere = new Sphere(new glm::vec3(5, 0, -25), glm::vec3(0.65f, 0.77f, 0.97f), 3);
	Shape *greySphere = new Sphere(new glm::vec3(-5.5, 0, -15), glm::vec3(1.0f, 0.90f, 0.90f), 3);
	//floor
	Shape *floorSphere = new Sphere(new glm::vec3(0, -10004, -20), glm::vec3(1.0f, 1.0f, 1.0f), 10000);
	//Shape *floorPlane = new Plane(new glm::vec3(0, -10004, -20), glm::vec3(1.0f, 1.0f, 1.0f), new glm::vec3(0, 1, 0), 400, 400);
	//random triangle...
	Shape *triangle = new Triangle(new glm::vec3(-0.5, 0, -10), new glm::vec3(0.5, 0.5, -10), new glm::vec3(0.51, 0, -10), glm::vec3(0.0f, 0.0f, 1.0f));
	//cube
	Shape *box = new Box(new glm::vec3(-0.5, -3, -10), glm::vec3(0.4f, 0.4f, 0.87f), new glm::vec3(1.0, 1.0, 1.5));
	//add all shapes to linked list
	List<Shape> *shapeList = new List<Shape>();
	shapeList->insert(shapeList->head, new Node<Shape>(redSphere));
	shapeList->insert(shapeList->tail, new Node<Shape>(yellowSphere));
	shapeList->insert(shapeList->tail, new Node<Shape>(blueSphere));
	shapeList->insert(shapeList->tail, new Node<Shape>(greySphere));
	shapeList->insert(shapeList->tail, new Node<Shape>(floorSphere));
	//shapeList->insert(shapeList->tail, new Node(floorPlane));
	//shapeList->insert(shapeList->tail, new Node(triangle));
	shapeList->insert(shapeList->tail, new Node<Shape>(box));

	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("Ray Casting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//get window surface
			screenSurface = SDL_GetWindowSurface(window);

			while (!done(true, false))
			{
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

				//calculate intersections
					//temp method
				//cast Ray
				rayCaster->castRay(rayOrigin, cameraSpace, view, shapeList, screenSurface);

				//draw(screenSurface);


				//update time and window
				prevTime = currentTime;
				currentTime = Clock::now();
				int frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - prevTime).count();
				fpsCount++;
				frameTimeThisSec += (frameTime * 0.000001);
				if (frameTimeThisSec >= 1000.0f)
				{
					std::cout << "Frames Per Second: " << fpsCount << " Average Frame Time: " << frameTimeThisSec/fpsCount*0.1 << std::endl;
					fpsCount = 0;
					frameTimeThisSec = 0;
				}
				std::cout << "Time to render frame in Seconds: " << frameTime * 0.000000001 << std::endl;
				SDL_UpdateWindowSurface(window);
			}
		}
	}

	//destroy the window
	SDL_DestroyWindow(window);

	//quit SDL subsystems
	SDL_Quit();

	//save to an image file	
	saveToFile();
	//printArray();




	system("pause");

	return 0;
}