#include <iostream>
#include <sstream>
#include <string>

#include <gl/glew.h>
#include <gl/glut.h>
#include "billboard.h"
#include "ObjModel.h"
#include "TrafficLight.h"
#include "utility.h"
#include "PPMImage.h"
#include <time.h>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

time_t rawtime;
struct tm * timeinfo;
char buffer[80];

// Counter for the traffic signal.  Only one is needed.

int counter = 0;
int j;
// 20 miliseconds
int updateInterval = 20;

// 3D models
ObjModel car, surveillanceCamera, building,human;
TrafficLight trafficLight;
Billboard billboard1, billboard2, billboard3, billboard4 ;
PPMImage ppm,violation;
GLubyte *OpenGLimage, *p;

// The display list IDs for car, surveillanceCamera, and terrain.
int carID, surveillanceCameraID, terrainID,humanID;

// Signals for North-South traffic and West-East traffic
Signal NS_Signal = Green;
Signal WE_Signal = Red;

// Car position and speed
Vector3 carPosition = {3, 0, 45},
		localCarSpeed = {0, 0, 0}, worldCarSpeed;

Vector3 location = { -10, 0, -45 };
float orientation = -45.00;
// Car direction and heading
float carDirection = 180;
string carHeading = "N";

// Third person camera offset in the car's local frame and world frame.
Vector3	localCameraOffset = { 0, 0, -4 }, worldCameraOffset;
char time1[10];
char date[50];
char name[50];

float acceleration = 0.05;
// Width and height for the window and three small viewports.
int winWidth, winHeight, sWidth, sHeight;

// Update the small viewports' size automatically.
void reshape(int w, int h)
{
	winWidth = w, winHeight = h;

	// Update sWidth and sHeight here.
	sWidth = w / 4, sHeight = h / 4;
}

void specialKey(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		// Update car direction
		carDirection += 2;
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		if (carDirection > 360) carDirection = 0;

		// Compute the car heading.
		//carDirection = carDirection % 360;
		if (carDirection > 180 && carDirection < 270) carHeading = "NW";
		else if (carDirection > 270 && carDirection < 360) carHeading = "SW";
		else if (carDirection > 0 && carDirection < 90) carHeading = "SE";
		else if (carDirection > 90 && carDirection < 180) carHeading = "NE";
		else if (carDirection == 180) carHeading = "N";
		else if (carDirection == 270) carHeading = "W";
		else if (carDirection == 360) carHeading = "S";
		else if (carDirection == 360) carHeading = "E";


	}
	if (key == GLUT_KEY_RIGHT)
	{
		// Handle the right turns.
			carDirection -= 2;
			if (carDirection == 0) carDirection = 360;
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		//worldCameraOffset = computeRotatedVector(localCameraOffset, carDirection);
		//worldCameraOffset.z = worldCameraOffset.z;
		if (carDirection == 90) carHeading = "E";
		else if (carDirection == 0) carHeading = "S";
		else if (carDirection == 180) carHeading = "N";
		else if (carDirection == 270) carHeading = "W";
		else if (carDirection > 90 && carDirection < 180) carHeading = "NE";
		else if (carDirection > 0 && carDirection < 90) carHeading = "SE";
		else if (carDirection > 180 && carDirection < 270) carHeading = "NW";
		else if (carDirection > 270 && carDirection < 360) carHeading = "SW";

	
	}
	if (key == GLUT_KEY_UP)
	{
		
		acceleration = 0.08;
		localCarSpeed.z += acceleration;
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		
	}

	if (key == GLUT_KEY_DOWN)
	{
		// deceleration
		
		acceleration = .05;
		localCarSpeed.z -= acceleration;
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
	}
}

void drawScene()
{
	// Draw terrain
	glCallList(terrainID);

	glEnable(GL_LIGHTING);

	// Traffic Lights
	// North-East (NS_Signal)

	glPushMatrix();
	glTranslatef(10, 0, -10.5);
	glScalef(1/3.28/12, 1/3.28/12, 1/3.28/12);
	trafficLight.setSignal(NS_Signal);
	trafficLight.Draw();
	glPopMatrix();

	// North-West (WE_Signal)
	glPushMatrix();
	glTranslatef(-10, 0, -10);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	glRotatef(70, 0, 1, 0);
	trafficLight.setSignal(WE_Signal);
	trafficLight.Draw();
	glPopMatrix();

	// South-West (NS_Signal)
	glPushMatrix();
	glTranslatef(-10, 0, 10);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
    glRotatef(180, 0, 1, 0);
	trafficLight.setSignal(NS_Signal);
	trafficLight.Draw();
	glPopMatrix();
	glCallList(terrainID);

	glEnable(GL_LIGHTING);
	// South-East (WE_Signal)
	glPushMatrix();
	glTranslatef(10, 0, 10);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	glRotatef(-90, 0, 1, 0);
	trafficLight.setSignal(WE_Signal);
	trafficLight.Draw();
	glPopMatrix();
	
	//surveillance  Camera
	// North-East camera
	glPushMatrix();
	glTranslatef(10, 0, -10);
	glRotatef(-360, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// North west camera
	glPushMatrix();
	glTranslatef(-10.5, 0, -10);
	glRotatef(-135, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// South west camera
	glPushMatrix();
	glTranslatef(-10.5, 0, 10);
	glRotatef(-45, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// South east camera
	glPushMatrix();
	glTranslatef(10, 0, 11);
	glRotatef(-70, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();
	
	// Draw the car.
	glPushMatrix();
	//glTranslatef(3, carPosition.y, 38);
	glTranslatef(carPosition.x, carPosition.y, carPosition.z-4);
	glRotatef(carDirection, 0, 1, 0);
	glScalef(1 / 3.2 / 0.3, 1 / 3.28 / 0.3, 1 / 3.28 / 0.3);
	glCallList(carID);

	//glPushMatrix();
	//glTranslatef(10,0,14);
	////glRotatef(carDirection, 0, 1, 0);
	//glCallList(humanID);
	//glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	glPopMatrix();
	glColor3f(1, 1, 1);

}

void init()
{
	

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
	std::string str(buffer);

	sWidth = winWidth / 4, sHeight = winHeight / 4;
	glClearColor(0.5, 0.5, 1.0, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Note that the light is defined in the eye or camera frame.
	GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};

	GLfloat ambient[] = {0.3, 0.3, 0.3, 1};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);	
	glEnable(GL_LIGHT0);

	
	// Generate display list for the surveillance camera.
	surveillanceCameraID = glGenLists(1);
	glNewList(surveillanceCameraID, GL_COMPILE);
	surveillanceCamera.Draw();
	glEndList();
	



	glEnable(GL_LIGHTING);
	// Generate display list for the car.
	carID = glGenLists(1);
	glNewList(carID, GL_COMPILE);
	car.Draw();
	glEndList();
	// Generate the display list for terrain, including road and grass.
	terrainID = glGenLists(1);
	glNewList(terrainID, GL_COMPILE);
	glDisable(GL_LIGHTING);

	// Generate the display list for human.
	/*humanID = glGenLists(1);
	glNewList(humanID, GL_COMPILE);
	human.Draw();
	glEndList();*/
	
	// Grass
	glColor3f(0, 0.7, 0);
	glBegin(GL_QUADS);
		glVertex3f(-1000, 0, 1000);
		glVertex3f(-10, 0, 1000);
		glVertex3f(-10, 0, 10);
		glVertex3f(-1000, 0, 10);

		glVertex3f(10, 0, 1000);
		glVertex3f(1000, 0, 1000);
		glVertex3f(1000, 0, 10);
		glVertex3f(10, 0, 10);

		glVertex3f(10, 0, -10);
		glVertex3f(1000, 0, -10);
		glVertex3f(1000, 0, -1000);
		glVertex3f(10, 0, -1000);

		glVertex3f(-1000, 0, -10);
		glVertex3f(-10, 0, -10);
		glVertex3f(-10, 0, -1000);
		glVertex3f(-1000, 0, -1000);
	glEnd();

	// Roads
	glBegin(GL_QUADS);
		glColor3f(0.2, 0.2, 0.2);

		glVertex3f(-10, 0, 1000);
		glVertex3f(10, 0, 1000);
		glVertex3f(10, 0, -1000);
		glVertex3f(-10, 0, -1000);

		glVertex3f(-1000, 0, 10);
		glVertex3f(1000, 0, 10);
		glVertex3f(1000, 0, -10);
		glVertex3f(-1000, 0, -10);
	glEnd();

	// Yellow line.
	//south line
	glBegin(GL_POLYGON);
		glColor3f(1, 1, 0);
		glVertex3f(-0.1, .050, 1000);
		glVertex3f(0.1, .050, 1000);
		glVertex3f(0.1, 0.05, 20);
		glVertex3f(-0.1, 0.05, 20);
	glEnd();
	//north line
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0);
	glVertex3f(0.1, 0.05, -20);
	glVertex3f(-0.1, 0.05, -20);
	glVertex3f(-0.1, 0.05, -1000);
	glVertex3f(0.1, 0.05, -1000);
	glEnd();
	/// east line 
	glBegin(GL_POLYGON);
	glVertex3f(10, 0.05, 1);
	glVertex3f(10, 0.05, -1);
	glVertex3f(1000, 0.05, -1);
	glVertex3f(1000, 0.05, 1);
	glEnd();
	/// west  line 
	glBegin(GL_POLYGON);
	glVertex3f(-10, 0.05, 1);
	glVertex3f(-10, 0.05, -1);
	glVertex3f(-1000, 0.05, -1);
	glVertex3f(-1000, 0.05, 1);
	glEnd();


	
	
	/// Dashed white lines
	//south line
	for (int i = 20; i < 1000; i = i + 8)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);

		glVertex3f(3.7, 0.05, i);
		glVertex3f(3.50, 0.05, i);
		glVertex3f(3.50, .050, i + 2);
		glVertex3f(3.7, .050, i + 2);
		glEnd();
	}
	for (int i = 20; i<1000; i = i + 8)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);

		glVertex3f(-3.7, 0.05, i);
		glVertex3f(-3.50, 0.05, i);
		glVertex3f(-3.50, .050, i + 2);
		glVertex3f(-3.7, .050, i + 2);
		glEnd();
	
	}
	/// North   lines
	for (int i = -20; i >-1000; i = i - 8)
	{
		int j = i - 2;
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(3.7, 0.05, i);
		glVertex3f(3.5, 0.05, i);
		glVertex3f(3.5, 0.05, j);
		glVertex3f(3.7, 0.05, j);
		glEnd();
	}

	for (int i = -20; i >-1000; i = i - 8)
	{
		int j = i - 2;
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(-3.7, 0.05, i);
		glVertex3f(-3.5, 0.05, i);
		glVertex3f(-3.5, 0.05, j);
		glVertex3f(-3.7, 0.05, j);
		glEnd();
	}
	/// east line

	for (int i = 10; i< 1000; i = i + 8)
	{
		int j = i + 2;
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(i, 0.05, 3.5);
		glVertex3f(i, 0.05, 3.7);
		glVertex3f(j, 0.05, 3.7);
		glVertex3f(j, 0.05, 3.5);
		glEnd();
	}

	for (int i = 10; i < 1000; i = i + 8)
	{
		int j = i + 2;
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(i, 0.05, -3.5);
		glVertex3f(i, 0.05, -3.7);
		glVertex3f(j, 0.05, -3.7);
		glVertex3f(j, 0.05, -3.5);
		glEnd();
	}
	
	/// west line

	for (int i = -10; i>- 1000; i = i - 8)
	{
		int j = i - 2;
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(i, 0.05, 3.5);
		glVertex3f(i, 0.05, 3.7);
		glVertex3f(j, 0.05, 3.7);
		glVertex3f(j, 0.05, 3.5);
		glEnd();
	}

	for (int i = -10; i >- 1000; i = i - 8)
	{
		int j = i - 2;
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex3f(i, 0.05, -3.5);
		glVertex3f(i, 0.05, -3.7);
		glVertex3f(j, 0.05, -3.7);
		glVertex3f(j, 0.05, -3.5);
		glEnd();
	}
	glPushMatrix();
	// draw four billboards
	billboard1.ReadFile("advertisements/7.ppm");//");
	billboard1.SetSize(20,20);
	billboard1.SetLocation(location);
	billboard1.SetOrientation(45);
	billboard1.Draw(1);
	billboard2.ReadFile("advertisements/mercedes-benz-2.ppm");
	billboard2.SetSize(20, 20);
	billboard2.SetLocation(location);
	billboard2.SetOrientation(45);
	billboard2.Draw(2);
	billboard3.ReadFile("advertisements/gettyimages-84709618.ppm");
	billboard3.SetSize(20, 20);
	billboard3.SetLocation(location);
	billboard3.SetOrientation(orientation);
	billboard3.Draw(3);
	billboard4.ReadFile("advertisements/ericsson_landscape3.ppm");
	billboard4.SetSize(20, 20);
	billboard4.SetLocation(location);
	billboard4.SetOrientation(-45);
	billboard4.Draw(4);
	glPopMatrix();
	
	glEndList();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Generate head-up display (HUD)
	stringstream ss;

	// Setup viewport, projection, and camera for the main view.
	glViewport(0, 0, winWidth, winHeight - sHeight - 50);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)winWidth / (winHeight - sHeight - 50), 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Update the code here for the correct third person camera that moves with the car.

	gluLookAt(carPosition.x + worldCameraOffset.x, carPosition.y + 2, carPosition.z + worldCameraOffset.z, carPosition.x, carPosition.y + 1.5,
		carPosition.z, 0, 1, 0);

	drawScene();

	// Setup viewport, projection, and camera for the South-East camera and draw the scene again.
	glViewport(35, winHeight - sHeight, ((sWidth / .85)+12), 200);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)sWidth / sHeight, 1, 15000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 4, 10, 0.0, 3.0, -5.0, 0.0, 1.0, 0.0);
	drawScene();

	// Setup the viewport, projection, camera for the top view and draw the scene again.

	glViewport(450, winHeight - sHeight, sWidth / .85, sHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)sWidth / sHeight, 1, 15000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 70, 0, 0.0, 0.50, 0, 0.0, 0.0, -1.0);
	drawScene();

	// Setup viewport, projection, camera for the South-West camera and draw the scene again.
	glViewport(850, winHeight - sHeight, sWidth / .85, 120);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)sWidth / sHeight, 1, 15000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-10, 4, 10, 15, 0, -10, 0, 1, 0);

	drawScene();

	float speed;
	speed = localCarSpeed.z/0.02;
	speed = speed * 2.24;
	ss << speed;
	glWindowPos2f(550, 550);
	glColor3f(1.0, 0.0, 0.0);
	printString("Heading: " + carHeading + "   Speed: " + ss.str() + "m/h");
	glutSwapBuffers();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	
	switch(key)
	{
	case 'r':
		// Add code for reset
		carPosition = { 3, 0, 45 },
			localCarSpeed = { 0, 0, 0 },
			worldCarSpeed= { 0, 0, 0 };
		 carDirection = 180;
		 carHeading = "N";
		break;
	case 'b':
		// Add code for breaking.

		localCarSpeed = { 0, 0, 0 },
		worldCarSpeed = { 0, 0, 0 };
		break;
	case 's':
		
		ppm.AllocateMemory(winWidth, winHeight);
		//glReadPixels(850, winHeight - sHeight, sWidth / .85, 120, GL_RGB, GL_UNSIGNED_BYTE, ppm.image);
		glReadPixels(0, 0, winWidth, winHeight, GL_RGB, GL_UNSIGNED_BYTE, ppm.image);
		ppm.VerticalFlip();
		ppm.WriteFile("snapshots/snap.ppm","P6");

		glFinish();
		break;
	case 27:
		exit(0);
		break;
	}

	glutPostRedisplay();
}
void trafficrule_Violation()
{

}

void update()
{
	// Update car position.
	
	carPosition.z += worldCarSpeed.z;
	carPosition.x += worldCarSpeed.x;
	worldCameraOffset = computeRotatedVector(localCameraOffset, carDirection);

	// State machine for the traffic signals using three variables: NS_Signal, WE_Signal, and counter. 

	counter++;
	if (counter == 5 * 1000 / updateInterval && NS_Signal == Green)
	{
		
		NS_Signal = Yellow;
		WE_Signal = Red;
		counter = 0;
		
	}
	else if (NS_Signal == Yellow && counter == 1000 / updateInterval)
	{
		NS_Signal = Red;
		WE_Signal = Green;
		counter = 0; 
		

	}

	else if (NS_Signal == Red && counter == 6*1000 / updateInterval)
	{
		NS_Signal = Green;
		WE_Signal = Yellow;
		counter = 0;
	}

	_strtime(time1);
	_strdate(date);



	
	if (NS_Signal == Red )
	{
		
		if ( carPosition.z < 3 && carPosition.z>-20)
		{
			
			auto t = std::time(nullptr);
			auto tm = *std::localtime(&t);

			std::ostringstream oss;
			oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
			auto str = oss.str();
			string filename;
			filename = "violation/"+str+".ppm";
			violation.AllocateMemory(winWidth-900 , winHeight-600 );
			glReadPixels(33, 600, winWidth-900, winHeight-600, GL_RGB, GL_UNSIGNED_BYTE, violation.image);
			glDrawPixels(winWidth - 900, winHeight - 600, GL_RGB, GL_UNSIGNED_BYTE, violation.image);
			violation.VerticalFlip();
			violation.WriteFile(filename.c_str(), "P6");
			glFinish();
		}
	}
}

void timer(int miliseconds)
{
	update();
	glutTimerFunc(updateInterval, timer, updateInterval);	
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	winWidth = 1300, winHeight = 800;
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Traffic Simulation");
	glewInit();

	// Load the 3D models.
	trafficLight.ReadFile("Models/TrafficLight.obj");
	car.ReadFile("Models1/taxi.obj");
	surveillanceCamera.ReadFile("Models/camera.obj");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKey);
	glutTimerFunc(0, timer, updateInterval);
	glutMainLoop();
	system("pause");
}