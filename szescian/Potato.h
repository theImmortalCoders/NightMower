#pragma once
#include <loader.h>

struct Potato
{
	ObjectData object;
	int x;
	int z;
	float angle = 0;
	float heightArgument = 0;
	void draw() {
		drawObj(&object, 0, 0, 0, 2, 2, 2, 1, 1);
	}
};