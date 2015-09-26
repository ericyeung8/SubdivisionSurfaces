// SubdivisionSurfaces.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include "Loadobj.h"
#include "Subdivision.h"
#include <gl/glut.h>
#include <gl/gl.h>

std::vector <Vertex> v;
std::vector <Surface> f;
float xrange, yrange, zrange;
int flag = 0;
int clear = 0;
int mouseposition[2] = { 0, 0 };
int count[2] = { 0, 1 };
float move[2] = { 0, 0 };

//Calculate the approximate parameter for projection
void CalculateRange()
{
	int i;
	float xmax, ymax, zmax, xmin, ymin, zmin;
	xmax = ymax = zmax = -100;
	xmin = ymin = zmin = 100;

	for (i = 1; (unsigned int)i < v.size(); i++)
	{
		if (v[i].x > xmax)
			xmax = v[i].x;
		else if (v[i].x < xmin)
			xmin = v[i].x;

		if (v[i].y > ymax)
			ymax = v[i].y;
		else if (v[i].y < ymin)
			ymin = v[i].y;

		if (v[i].z > zmax)
			zmax = v[i].z;
		else if (v[i].z < zmin)
			zmin = v[i].z;
	}

	if (xmax > abs(xmin))
		xrange = ceil(xmax) + 20;
	else xrange = ceil(abs(xmin)) + 20;

	if (ymax > abs(ymin))
		yrange = ceil(ymax) + 20;
	else yrange = ceil(abs(ymin)) + 20;

	if (zmax > abs(zmin))
		zrange = ceil(zmax) + 20;
	else zrange = ceil(abs(zmin)) + 20;

}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Enable lighting
	float light_position1[] = { 10.0, 10.0, 30.0, 1.0 };
	float light_position2[] = { 10.0, 10.0, -30.0, 1.0 };
	float light_ambient[] = { .5, .5, .5, 1 };
	float light_diffuse[] = { .9, 0, .9, 1 };
	float light_specular[] = { 1, 1, 1, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHTING);

	//Enable z-buffering
	glEnable(GL_DEPTH_TEST);

	//Enable FLAT Shading
	glShadeModel(GL_FLAT);
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-xrange, xrange, -yrange, yrange, -zrange, zrange);
	glMatrixMode(GL_MODELVIEW);
}

void Translation()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef(pow(1.1, count[0]), pow(1.1, count[0]), pow(1.1, count[0]));
	glRotatef(move[0], 1, 0, 0);
	glRotatef(move[1], 0, 1, 0);
}

void display(void)
{
	int i;
	
	if (clear == 1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		clear = 0;
	}
	
	//Linear Subdivision and Averaging
	if (flag == 1)
	{
		LinearSub(v, f);
		flag = 0;
	}
	else if (flag == 2)
	{
		Averaging(v, f);
		flag = 0;
	}
	else if (flag == 3)
	{
		LinearSub(v, f);
		Averaging(v, f);
		flag = 0;
	}

	glLoadIdentity();

	//Ratate and zoom
	Translation();

	//Draw the object
	glBegin(GL_QUADS);
	for (i = 0; (unsigned int)i < f.size(); i++)
	{
		glVertex3f(v[f[i].a - 1].x, v[f[i].a - 1].y, v[f[i].a - 1].z);
		glVertex3f(v[f[i].b - 1].x, v[f[i].b - 1].y, v[f[i].b - 1].z);
		glVertex3f(v[f[i].c - 1].x, v[f[i].c - 1].y, v[f[i].c - 1].z);
		glVertex3f(v[f[i].d - 1].x, v[f[i].d - 1].y, v[f[i].d - 1].z);
	}
	glEnd();

	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'L':
		flag = 1;
		clear = 1;
		break;
	case 'A':
		flag = 2;
		clear = 1;
		break;
	case '+':
		flag = 3;
		clear = 1;
		break;
	}

	glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_RIGHT_BUTTON:
		//Zoom in
		if (state == GLUT_DOWN)
		{
			count[0] += 1;
			clear = 1;
		}
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP)
		{
			clear = 1;
		}
	}

	glutPostRedisplay();
}

void MouseMove(int x, int y)
{
	//Calculate the rotate angle
	move[0] += ((float)x - mouseposition[0]) / WINDOW_WIDTH * 360.0;
	move[1] += ((float)y - mouseposition[1]) / WINDOW_HEIGHT * 360.0;
	clear = 1;

	glutPostRedisplay();
}

int main(int argc, char*argv[])
{
	if (argc != 2)
	{
		printf("Please input the obj file name!\n");
		exit(1);
	}
	else printf("%s", argv[1]);

	Loadobj(v, f, argv[1]);

	//Loadobj(v, f, "E:\\monsterfrog.obj");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Jiajun Yang - Assignment 6");
	CalculateRange();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
	glutMotionFunc(MouseMove);
	glutMainLoop();
	return 0;
}