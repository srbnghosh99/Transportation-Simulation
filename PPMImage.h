#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class PPMImage
{
public:
	PPMImage(void);
	~PPMImage(void);
	int width, height;
	unsigned char* image;
	void AllocateMemory(int width, int height);
	void ReadFile(string fileName);
	void VerticalFlip();
	void WriteFile(string fileName1, string fileType);

private:
	char c, buf[128];
	ifstream file;
	ofstream outFile;
	void CheckComment();
};

