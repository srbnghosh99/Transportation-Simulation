#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
#include "PPMImage.h";

#include "Vector3D.h";

#include "bmp_io.h"

using namespace std;

class Billboard
{
public:
	Billboard(void);
	~Billboard(void);
	void ReadFile (string fileName1);
	void SetSize(float width, float height);
	void SetLocation(Vector3 location);
	void SetOrientation(float orientation);
	void Draw(int billboard);

private:
	PPMImage textureImage;
	float width, height;
	// The variable location contains 3 variables: x, y, z, where y is really the elevation of the board.
	Vector3 location;
	float orientation;
	GLuint textureNumber;
};

