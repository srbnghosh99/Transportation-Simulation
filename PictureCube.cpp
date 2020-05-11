#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "bmp_io.h"

using namespace std;

const int numImages = 6;
static GLuint texName[numImages];

char fileNames[numImages][80] = {"YShen_ID.bmp", "IMG_1702.bmp", "IMG_1691.bmp", "test.bmp",
"winter.bmp", "water.bmp"};
unsigned long int widths[numImages];
long int heights[numImages];
unsigned char *images[numImages];

int s0 = 0;

void readFiles()
{
	bmp_byte_swap_set ( true );

	for (int i = 0; i < numImages; i++)
	{
		unsigned char *rarray = NULL, *garray = NULL, *barray = NULL;
		unsigned long int width;
		long int height;

		if (bmp_read(fileNames[i], &width, &height, &rarray, &garray, &barray))
			cout << "Failed to read file " << fileNames[i] << ".\n";
		rarray = new unsigned char[width * height];
		garray = new unsigned char[width * height];
		barray = new unsigned char[width * height];
		images[i] = new unsigned char[width * height * 4];
		widths[i] = width;
		heights[i] = height;

		bmp_read(fileNames[i], &width, &height, &rarray, &garray, &barray);

		for (int j = 0; j < width * height; j++)
		{
			images[i][j * 4] = rarray[j];
			images[i][j * 4 + 1] = garray[j];
			images[i][j * 4 + 2] = barray[j];
			images[i][j * 4 + 3] = 255;
		}

		delete [] rarray;
		delete [] garray;
		delete [] barray;
	}
}

void init (void) 
{
	glClearColor (0.0, 0.0, 0.2, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL);

	GLfloat light_pos[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glShadeModel(GL_FLAT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);	
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	readFiles();   
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(numImages, texName);

	for (int i = 0; i < numImages; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widths[i], heights[i],
					0, GL_RGBA, GL_UNSIGNED_BYTE, images[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
	glEnable(GL_TEXTURE_2D);
}

void display(void)
{
	float vertices[8][3] = {{0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
		{-0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}};
   
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

	glEnable(GL_LIGHTING);
		glColor3f(0, 1, 0);

		glBindTexture(GL_TEXTURE_2D, texName[s0]);
		glBegin(GL_QUADS);
			glNormal3f(1, 0, 0);
			glTexCoord2f(0.0, 0.0);
			glVertex3fv(vertices[0]);
			glTexCoord2f(1.0, 0.0);
			glVertex3fv(vertices[1]);
			glTexCoord2f(1.0, 1.0);
			glVertex3fv(vertices[5]);
			glTexCoord2f(0.0, 1.0);
			glVertex3fv(vertices[4]);
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0, 1.0, 0);
			glVertex3fv(vertices[1]);
			glVertex3fv(vertices[2]);
			glVertex3fv(vertices[6]);
			glVertex3fv(vertices[5]);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texName[2]);
		glBegin(GL_QUADS);
			glNormal3f(-1, 0, 0);
			glTexCoord2f(0.0, 0.0);
			glVertex3fv(vertices[2]);
			glTexCoord2f(1.0, 0.0);
			glVertex3fv(vertices[3]);
			glTexCoord2f(1.0, 1.0);
			glVertex3fv(vertices[7]);
			glTexCoord2f(0.0, 1.0);
			glVertex3fv(vertices[6]);
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0, -1, 0);
			glVertex3fv(vertices[3]);
			glVertex3fv(vertices[0]);
			glVertex3fv(vertices[4]);
			glVertex3fv(vertices[7]);
		glEnd();

	glutSwapBuffers();
   glFlush ();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float) w / h, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q' || key == 27)
		exit(0);
}

void topMenuFunc(int id)
{
	switch (id)
	{
	case 1:
		break;
	case 2:
		break;
	case 5:
		break;
	case 6:
		break;
	}
	glutPostRedisplay();
}

void createMenu()
{
	int topMenu;
	topMenu = glutCreateMenu(topMenuFunc);
	glutAddMenuEntry("Reset", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv)
{
	//int topMenu;
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Picture Cube");
   createMenu();
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}