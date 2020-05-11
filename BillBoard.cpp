#include "Billboard.h"
#include "bmp_io.h"
#include <GL/glew.h>


Billboard::Billboard(void)
{
}

Billboard::~Billboard(void)
{
}

void Billboard::ReadFile(string fileName)
{
	this->textureImage.ReadFile(fileName);

	glGenTextures(1, &textureNumber);

	//Bind a texture
	glBindTexture(GL_TEXTURE_2D, textureNumber);

	//Unload image data into the current texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.width, textureImage.height,
		0, GL_RGB, GL_UNSIGNED_BYTE, textureImage.image);

	//Specify Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

}

void Billboard::SetSize(float width, float height)
{
	// Your code
	width = width;
	height = height;
}

void Billboard::SetLocation(Vector3 location)
{
	// Your code
	this->location = location;

}

void Billboard::SetOrientation(float orientation)
{
	// Your code
	this->orientation = orientation;
}

void Billboard::Draw(int billboard)
{
	int flag = billboard;
	// Draw the board and pillar.  Use texture mapping for the board only.
	// The pillar is drawn just using simple geometry, i.e., a rectangle.

	// Use glEnable() and glDisable() to change OpenGL states.

	// bill board on the left first one
	if (flag == 1) {
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(location.x - 10, 0, location.z);
		//glRotatef(3, 0, 1, 0);
		//glScaled(8, location.y, 1);
		glBegin(GL_QUADS);
		glColor3f(0.192, 0.192, 0.192);
		glVertex3f(-0.5, 0, 0);
		glVertex3f(0.1, 0, 0);
		glVertex3f(0.1, 5, 0);
		glVertex3f(-0.5, 5, 0);
		glEnd();
		glPopMatrix();

		// signboard
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureNumber);
		glPushMatrix();
		glTranslatef(location.x - 10, 0, location.z);
		glBegin(GL_QUADS);
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-5, 5, 0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(5, 5, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(5, 10, 0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-5, 10, 0);
		glEnd();
		glPopMatrix();

	}

	if (flag == 2) {
		// bill board on the left  second one
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureNumber);
		glPushMatrix();
		glTranslatef(location.x - 6, 0, location.z - 10);
		glRotatef(orientation, 0, 1, 0);
		//glRotatef(3, 0, 1, 0);
		//glScaled(8, location.y, 1);
		glBegin(GL_QUADS);
		glColor3f(0.192, 0.192, 0.192);
		glVertex3f(-0.5, 0, 0);
		glVertex3f(0.1, 0, 0);
		glVertex3f(0.1, 5, 0);
		glVertex3f(-0.5, 5, 0);
		glEnd();
		glPopMatrix();
		// signboard
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(location.x - 6, 0, location.z - 10);
		glRotatef(orientation, 0, 1, 0);
		glBegin(GL_QUADS);
		glColor3f(0.192, 0.192, 0.192);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-5, 5, 0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(5, 5, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(5, 10, 0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-5, 10, 0);
		glEnd();
		glPopMatrix();
	}
	//////////////////////////////////////////////////////////////////
	// bill board on the right  first  one
	if (flag == 3) {
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(location.x + 26, 0, location.z);
		glRotatef(-75, 0, 1, 0);
		glBegin(GL_QUADS);
		glColor3f(0.192, 0.192, 0.192);
		glVertex3f(-0.5, 0, 0);
		glVertex3f(0.1, 0, 0);
		glVertex3f(0.1, 6, 0);
		glVertex3f(-0.5, 6, 0);
		glEnd();
		glPopMatrix();
		// signboard
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureNumber);
		glPushMatrix();
		glTranslatef(location.x + 26, 0, location.z);
		glRotatef(orientation, 0, 1, 0);
		glBegin(GL_QUADS);
		glColor3f(0.192, 0.192, 0.192);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-5, 5, 0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(5, 5, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(5, 10, 0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-5, 10, 0);
		glEnd();
		glPopMatrix();
	}
	// bill board on the right  second one
	if (flag == 4) {
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureNumber);
		glPushMatrix();
		glTranslatef(location.x + 26, 0, location.z - 50);
		//glRotatef(3, 0, 1, 0);
		//glScaled(8, location.y, 1);
		glBegin(GL_QUADS);
		glColor3f(0.192, 0.192, 0.192);
		glVertex3f(-0.5, 0, 0);
		glVertex3f(0.1, 0, 0);
		glVertex3f(0.1, 5, 0);
		glVertex3f(-0.5, 5, 0);
		glEnd();
		glPopMatrix();
		// signboard
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(location.x + 26, 0, location.z - 50);
		glRotatef(orientation, 0, 1, 0);
		//glScaled(8, location.y, 1);
		glBegin(GL_QUADS);
		glColor3f(0.192, 0.192, 0.192);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-5, 5, 0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(5, 5, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(5, 10, 0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-5, 10, 0);
		glEnd();
		glPopMatrix();
	}
}
