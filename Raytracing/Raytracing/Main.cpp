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
double  ASPECT_RATIO = SCREEN_WIDTH / SCREEN_WIDTH;//image aspect ratio
double FOV = 60.0 *ASPECT_RATIO; //field of view
//position
double posX = 0, posY = 0, posZ = 0;


//view array
glm::vec3 **view;
//camera array
glm::vec3 *cameraSpace;
//ray origin arry
glm::vec3 *rayOrigin;

//init funcs
void move();

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
//for single colour value
float scaleColour(float colour)
{
	return colour * 255;
}

void draw(SDL_Surface* screenSurface)
{
	
	move();
}


//saves the current draw view array to an image file
/*
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
}*/



float convertToRad(float deg)
{
	return (deg*M_PI) / 180;
}

float convertToDeg(float rad)
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
	bool done = false;
	if (!SDL_PollEvent(&event)) return false;
	// read keys
	// User requests quit
	if (event.type == SDL_QUIT)
	{
		done = true;
	}
	//User presses a key
	else if (event.type == SDL_KEYDOWN)
	{
		//Select surfaces based on key press
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			done = true;
			break;

		default:
			break;
		}
	}
	return done;
}

void move()
{
	if (event.type == SDL_KEYDOWN)
	{
		posX = 0;
		posZ = 0;
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			posZ += 0.01;
			break;
		case SDLK_DOWN:
			posZ -= 0.01;
			break;
		case SDLK_LEFT:
			posX += 0.01;
			break;
		case SDLK_RIGHT:
			posX -= 0.01;
			break;
		default:
			break;
		}
		*rayOrigin = glm::vec3(rayOrigin->x + posX, rayOrigin->y, rayOrigin->z + posZ);
	}

	
}

//update time and window
void updateTimer()
{
	prevTime = currentTime;
	currentTime = Clock::now();
	double frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - prevTime).count();
	fpsCount++;
	frameTimeThisSec += (frameTime / 1000000);
	if (frameTimeThisSec >= 1000)
	{
		std::cout << "Frames Per Second: " << fpsCount << " Average Frame Time: " << frameTimeThisSec / fpsCount << std::endl;
		fpsCount = 0;
		frameTimeThisSec = 0;
	}
	//std::cout << "Time to render frame in Seconds: " << frameTime / 1000000 << std::endl;
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
	//AreaLight *areaLight = new AreaLight(new glm::vec3(0, 20, 0), glm::vec3(1.0f, 1.0f, 1.0f), new glm::vec3(6, 6, 6), 0.5f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.7, 0.7f, 0.7f));
	AreaLight *areaLight = new AreaLight(new glm::vec3(20, 20, -5), glm::vec3(1.0f, 1.0f, 1.0f), new glm::vec3(8, 8, 8), 0.5f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.7, 0.7f, 0.7f));
	//set up ray class
	RayCasting *rayCaster = new RayCasting(SCREEN_WIDTH, SCREEN_HEIGHT, ASPECT_RATIO, FOV, areaLight);
	//set up shapes
	//spheres
	Shape *redSphere = new Sphere(new glm::vec3(0, 0, -20), glm::vec3(1.0f, 0.32f, 0.36f), 4);
	Shape *yellowSphere = new Sphere(new glm::vec3(5, -1, -15), glm::vec3(0.90f, 0.76f, 0.46f), 2);
	Shape *blueSphere = new Sphere(new glm::vec3(5, 0, -25), glm::vec3(0.65f, 0.77f, 0.97f), 3);
	Shape *greySphere = new Sphere(new glm::vec3(-5.5, 0, -15), glm::vec3(1.0f, 0.90f, 0.90f), 3);

	Shape *redSphere2 = new Sphere(new glm::vec3(-5, 7, -20), glm::vec3(1.0f, 0.32f, 0.36f), 3);
	Shape *yellowSphere2 = new Sphere(new glm::vec3(-3, -1, -5), glm::vec3(0.90f, 0.76f, 0.46f), 1);
	Shape *blueSphere2 = new Sphere(new glm::vec3(2, -2, -7), glm::vec3(0.65f, 0.77f, 0.97f), 0.5);
	Shape *greySphere2 = new Sphere(new glm::vec3(1, 1, -10), glm::vec3(1.0f, 0.90f, 0.90f), 1);

	//floor
	Shape *floorSphere = new Sphere(new glm::vec3(0, -10004, -20), glm::vec3(1.0f, 1.0f, 1.0f), 10000);
	//Shape *floorPlane = new Plane(new glm::vec3(0, -10004, -20), glm::vec3(1.0f, 1.0f, 1.0f), new glm::vec3(0, 1, 0), 400, 400);
	//random triangle...
	Shape *triangle = new Triangle(new glm::vec3(5, -2.0, -5), new glm::vec3(4.5, -2, -10), new glm::vec3(4, -2.0f, -5), glm::vec3(0.0f, 0.0f, 1.0f));
	//cube
	Shape *box = new Box(new glm::vec3(-0.5, -3, -10), glm::vec3(0.4f, 0.4f, 0.87f), new glm::vec3(1.0, 1.0, 1.5));
	//add all shapes to linked list
	List<Shape> *shapeList = new List<Shape>();
	shapeList->insert(shapeList->head, new Node<Shape>(redSphere));
	shapeList->insert(shapeList->tail, new Node<Shape>(yellowSphere));
	shapeList->insert(shapeList->tail, new Node<Shape>(blueSphere));
	shapeList->insert(shapeList->tail, new Node<Shape>(greySphere));

	
	shapeList->insert(shapeList->head, new Node<Shape>(redSphere2));
	shapeList->insert(shapeList->tail, new Node<Shape>(yellowSphere2));
	shapeList->insert(shapeList->tail, new Node<Shape>(blueSphere2));
	shapeList->insert(shapeList->tail, new Node<Shape>(greySphere2));
	
	shapeList->insert(shapeList->tail, new Node<Shape>(floorSphere));

	//shapeList->insert(shapeList->tail, new Node<Shape>(floorPlane));
	shapeList->insert(shapeList->tail, new Node<Shape>(triangle));
	shapeList->insert(shapeList->tail, new Node<Shape>(box));
	cameraSpace = new glm::vec3(0.0f, 0.0f, 0.0f);

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
				//SDL_FillRect(screenSurface, NULL, 0x000000);
				//calculate intersections
					//cast Ray
				rayCaster->castRay(rayOrigin, cameraSpace, view, shapeList, screenSurface);

				draw(screenSurface);
				
				//move();
				
				//update time and window
				updateTimer();

				SDL_UpdateWindowSurface(window);
			}
		}
	}

	delete cameraSpace;
	//destroy the window
	SDL_DestroyWindow(window);

	//quit SDL subsystems
	SDL_Quit();

	//save to an image file	
	//saveToFile();
	//printArray();




	system("pause");

	return 0;
}