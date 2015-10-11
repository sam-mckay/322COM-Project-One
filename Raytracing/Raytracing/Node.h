#pragma once
#include<iostream>
#include"Shape.h"

class Node
{
private:
	int ID;

public:
	//GraphItem* value;
	Shape *value;
	int A; //edge pair
	int B; //edge pair
	Node* next;
	Node* prev;
	Node(Shape *val);
	Node(int A, int B);
	~Node();
	//setters
	void setID(int x);
	//getters
	int getID();
};

