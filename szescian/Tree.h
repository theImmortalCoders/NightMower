#pragma once
#include "loader.h"

class Tree
{
	ObjectData wood;
	ObjectData leaves;

	int randomSelect(int min, int max);
public:
	int x;
	int z;
	int sphereID;
	Tree();
	void load();
	void draw();
};

