#include "Node.h"
using namespace std;

Node::Node(Shape *val)
{
	//cout << "Node Constructor" << endl;
	this->value = val;
	this->next = (Node*)0;
	this->prev = (Node*)0;
}

Node::Node(int edge0, int edge1)
{
	A = edge0;
	B = edge1;
	value = NULL;
}


Node::~Node()
{
	//cout << "Node Destrustor" << endl;
}

//setters
void::Node::setID(int x)
{
	ID = x;
}
//getters

int::Node::getID()
{
	return ID;
}
