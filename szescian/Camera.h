#pragma once
#define GL_PI 3.14159265359

struct Camera
{
	double elevation = GL_PI / 12;
	float xCamPos = 0;
	float yCamPos = 0;
	float zCamPos = 0;
	double camDistance = 200;
	double const angleJump = GL_PI / 128;
	double const radiusJump = 10;
	int const renderDistance = 5000;
	double azimuth = GL_PI;
};