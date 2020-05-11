#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "bmp_io.h"

using namespace std;

float alpha = 1.0;
int textureInc = 0;
const int numImages = 6;
static GLuint texName[numImages];

int animation = 1;

char fileNames[numImages][80] = {"YShen_ID.bmp", "IMG_1702.bmp", "IMG_1691.bmp", "test.bmp",
"Water.bmp", "Winter.bmp"};
unsigned long int widths[numImages];
long int heights[numImages];
unsigned char *images[numImages];

#define bool int
#define false 0
#define true 1
#define M_PI 3.14159

/*
** Global settings.
*/

int 	winWidth, winHeight;

float 	angle = 0.0, axis[3], trans[3];
bool 	trackballMove = false;
bool 	redrawContinue = false;

GLfloat objectXform[4][4] = {
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0}
};

float lastPos[3] = {0.0F, 0.0F, 0.0F};
int startX, startY;

void trackball_ptov(int x, int y, int width, int height, float v[3])
{
	float d, a;

	/* project x, y onto a hemi-sphere centered within width, height */
	v[0] = (2.0F * x - width) / width;
	v[1] = (height - 2.0F * y) / height;
	d = (float) sqrt(v[0] * v[0] + v[1] * v[1]);
	//v[2] = (float) cos((M_PI/2.0F) * ((d < 1.0F) ? d : 1.0F));
	v[2] = (float) sqrt(1 - ((d < 1.0F) ? d : 1.0F));
	a = 1.0F / (float) sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] *= a;
	v[1] *= a;
	v[2] *= a;
}


void mouseMotion(int x, int y)
{
	float curPos[3], dx, dy, dz;

	trackball_ptov(x, y, winWidth, winHeight, curPos);

	dx = curPos[0] - lastPos[0];
	dy = curPos[1] - lastPos[1];
	dz = curPos[2] - lastPos[2];

	if (dx || dy || dz) {
		angle = 90.0F * sqrt(dx * dx + dy * dy + dz * dz) / 3;

		axis[0] = lastPos[1] * curPos[2] - lastPos[2] * curPos[1];
		axis[1] = lastPos[2] * curPos[0] - lastPos[0] * curPos[2];
		axis[2] = lastPos[0] * curPos[1] - lastPos[1] * curPos[0];

		lastPos[0] = curPos[0];
		lastPos[1] = curPos[1];
		lastPos[2] = curPos[2];
	}
	glutPostRedisplay();
}

void startMotion(int x, int y)
{
	redrawContinue = false;
	startX = x; startY = y;
	trackball_ptov(x, y, winWidth, winHeight, lastPos);
	trackballMove = true;
}

void stopMotion(int x, int y)
{
	if (startX != x || startY != y) 
		redrawContinue = true;
	else 
	{
		angle = 0.0F;
		redrawContinue = false;
		trackballMove = false;
	}
}

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


void timer(int num)
{
	if (animation == 1)
	{
		alpha -= 0.02;
		if (alpha < 0)
		{
			//alpha += 1;
			alpha = 1;
			textureInc++;
		}

		if (textureInc > 5)
			textureInc -= 6;
	}

	glutPostRedisplay();
	glutTimerFunc(num, timer, num);	
}

void mouseButton(int button, int state, int x, int y)
{
	switch(state) 
	{
	case GLUT_DOWN:
		startMotion(x, y);
		break;
	case GLUT_UP:
		stopMotion(x, y);
		break;
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
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widths[i], heights[i],
					0, GL_RGBA, GL_UNSIGNED_BYTE, images[i]);
	}
}

void display(void)
{
	float vertices[8][3] = {{0.5, -0.5, -0.5},{0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
		{-0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}};
   
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

	if (trackballMove) {
		glPushMatrix();
		glLoadIdentity();
		glRotatef(angle, axis[0], axis[1], axis[2]);
		glMultMatrixf((GLfloat *) objectXform);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) objectXform);
		glPopMatrix();
	}
	glMultMatrixf((GLfloat *) objectXform);

	glColor3f(0, 1, 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[textureInc]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 1) % 6]);
	GLfloat constColor[4] = {0.0, 0.0, 0.0, 0.5};
	constColor[3] = alpha;
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE0);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE1);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_CONSTANT);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);
	
	glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 0.0);
		glVertex3fv(vertices[0]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 0.0);
		glVertex3fv(vertices[1]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 1.0);
		glVertex3fv(vertices[5]);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 1.0);
		glVertex3fv(vertices[4]);
	glEnd();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 1)%6]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 2) % 6]);
	glBegin(GL_QUADS);
		glNormal3f(0, 1.0, 0);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 0.0);
		glVertex3fv(vertices[1]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 0.0);
		glVertex3fv(vertices[2]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 1.0);
		glVertex3fv(vertices[6]);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 1.0);
		glVertex3fv(vertices[5]);
	glEnd();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 2)%6]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 3) % 6]);
	glBegin(GL_QUADS);
		glNormal3f(-1, 0, 0);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 0.0);
		glVertex3fv(vertices[2]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 0.0);
		glVertex3fv(vertices[3]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 1.0);
		glVertex3fv(vertices[7]);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 1.0);
		glVertex3fv(vertices[6]);
	glEnd();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 3)%6]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 4) % 6]);
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 0.0);
		glVertex3fv(vertices[3]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 0.0);
		glVertex3fv(vertices[0]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 1.0);
		glVertex3fv(vertices[4]);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 1.0);
		glVertex3fv(vertices[7]);
	glEnd();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 4)%6]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 5) % 6]);
	glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 0.0);
		glVertex3fv(vertices[4]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 0.0);
		glVertex3fv(vertices[5]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 1.0);
		glVertex3fv(vertices[6]);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 1.0);
		glVertex3fv(vertices[7]);
	glEnd();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 5)%6]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texName[(textureInc + 6) % 6]);
	glBegin(GL_QUADS);
		glNormal3f(0, 0, -1);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 0.0);
		glVertex3fv(vertices[0]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 0.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 0.0);
		glVertex3fv(vertices[3]);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0, 1.0);
		glVertex3fv(vertices[2]);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0, 1.0);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0, 1.0);
		glVertex3fv(vertices[1]);
	glEnd();

	glutSwapBuffers();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	winWidth = w;
	winHeight = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
		glOrtho(-1 * (float) w / h , 1 * (float) w / h, -1, 1, 3, 7);
	else
		glOrtho(-1, 1, -1 * (float) h / w, 1 * (float) h / w, 3, 7);
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
		alpha = 0;
		textureInc = 0;
		break;
	case 2:
		animation = 0;
		break;
	case 3:
		animation = 1;
	}
	glutPostRedisplay();
}
void createMenu()
{
	int topMenu;
	topMenu = glutCreateMenu(topMenuFunc);
	glutAddMenuEntry("Reset", 1);
	glutAddMenuEntry("Stop", 2);
	glutAddMenuEntry("Resume", 3);
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
	glewInit();
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutTimerFunc(0, timer, 60);
	glutMainLoop();
	return 0;   /* ANSI C requires main to return int. */
}