#pragma once
#include"Node.h"

using namespace std;

class List
{
private:
	int length;
public:
	Node* head;
	Node* tail;
	List();
	~List();
	void insert(Node* prevNode, Node* newNode);
	void display();
	void displayEdges();
	Node* getNode(int x);
	Node* getNodeFromID(int ID);
	void deleteNode(int nodeNum);
	int getLength();
};

