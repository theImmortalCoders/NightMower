#pragma once
#include <loader.h>
#include <random.h>

struct Potato
{
	ObjectData object;
	int x;
	int z;
	float angle = 0;
	float heightArgument = 0;
	int sphereID;
	Potato(int maxX, int maxZ, int minX, int minZ) {
		object = loadFile("potato.obj", "potato.jpg");
		x = random(minX + 100, maxX - 100);
		z = random(minZ + 100, maxZ - 100);
		if (z < 0) {
			if (x > 0) {
				sphereID = 1;
			}
			else {
				sphereID = 2;
			}
		}
		else {
			if (x > 0) {
				sphereID = 3;
			}
			else {
				sphereID = 4;
			}
		}
		angle = random(0, 360);
		heightArgument = random(0, 200) / 100.0;
	}
	void draw() {
		drawObj(&object, 0, 0, 0, 2, 2, 2, 1, 1);
	}
};