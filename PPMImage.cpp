#include "PPMImage.h"


PPMImage::PPMImage(void)
{
	image = NULL;
}

PPMImage::~PPMImage(void)
{
	if (image != NULL)
		delete[] image;
}

void PPMImage::AllocateMemory(int width, int height)
{
	this->width = width;
	this->height = height;
	image = new unsigned char[width * height * 3];
}

void PPMImage::ReadFile(string fileName)
{
	file.open(fileName, ios_base::binary);

	if (!file.is_open())
	{
		cout << fileName << " couldn't be opened.\n";
		system("pause");
		exit(1);
	}

	string fileType;
	file >> fileType;

	CheckComment();
	file >> width >> height;
	image = new unsigned char[width * height * 3];

	int maxValue;

	file >> maxValue;

	if (fileType == "P3")
	{
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				for (int k = 0; k < 3; k++)
				{
					string temp;
					file >> temp;
					image[(i * width + j) * 3 + k] = (unsigned char)atoi(temp.c_str());
				}
	}

	else if (fileType == "P6")
	{
		file.getline(buf, 127);
		file.read((char *)image, height * width * 3);
	}

	file.close();
}

void PPMImage::WriteFile(string fileName1, string fileType)
{
	string fileName = fileName1;
	// Read and understand the code.
	if (fileType == "P6")
	{
		outFile.open(fileName, ios_base::binary);

		if (!outFile.is_open())
		{
			cout << fileName << " couldn't be opened.\n";
			system("pause");
			exit(1);
		}

		outFile << fileType << endl;
		outFile << width << " " << height << endl;
		outFile << 255 << endl;

		outFile.write((char *)image, height * width * 3);
	}
	else if (fileType == "P3")
	{
		outFile.open(fileName);

		if (!outFile.is_open())
		{
			cout << fileName << " couldn't be opened.\n";
			system("pause");
			exit(1);
		}

		outFile << fileType << endl;
		outFile << width << " " << height << endl;
		outFile << 255 << endl;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				for (int k = 0; k < 3; k++)
				{
					outFile << (int)image[(i * width + j) * 3 + k];
					//if (!(k == 2 && j == (width - 1)))
					outFile << " ";
				}
			outFile << endl;
		}
	}
	outFile.close();
}

void PPMImage::CheckComment()
{
	c = file.peek();

	while (c == '#' || c == '\n' || c == ' ')
	{
		file.getline(buf, 127);
		c = file.peek();
	}
}

void PPMImage::VerticalFlip()
{
	unsigned char* flippedImage;

	flippedImage = new unsigned char[width * height * 3];
	int i, j,k;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width ; j++)
		{
			for (k = 0; k < 3; k++)
			{
				
				flippedImage[((i )* width + j) * 3 + k] = image[((height - 1 - i) * width + j) * 3 + k];
			}
		}
	}
	// Do a vertical flip.  You need to use the help variable declared above.
	// Also do dynamic memory allocation for the variable based on the image size.
	image = NULL;
	image = flippedImage;
	
}

