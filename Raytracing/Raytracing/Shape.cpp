#include "Shape.h"

Shape::Shape()
{
}

Shape::Shape(glm::vec3 *p, glm::vec3 c)
{
	position = p;
	colour = c;
}


Shape::~Shape()
{
}

//draws a vertical line

void Shape::drawLine(glm::vec3 **view, int x, int y1, int y2, int z, glm::vec3 colour)
{
	if (x < 0)
	{
		return;
	}
	for (int y = y1; y < y2; y++)
	{
		//for calculating nearest pixel - later
		view[x][y].z = z;

		//set pixel colour
		view[x][y].r = colour.r;
		view[x][y].g = colour.g;
		view[x][y].b = colour.b;		
	}
}