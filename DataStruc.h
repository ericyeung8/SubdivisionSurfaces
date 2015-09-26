#ifndef _DATASTRUC_H_
#define _DATASTRUC_H_

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

class Vertex
{
public:
	float x, y, z;
	Vertex(float newX, float newY, float newZ)
	{
		x = newX;
		y = newY;
		z = newZ;
	}
	Vertex(void)
	{
		x = y = z = 0;
	}
};

class Surface
{
public:
	int a, b, c, d;
	Surface(int newA, int newB, int newC, int newD)
	{
		a = newA;
		b = newB;
		c = newC;
		d = newD;
	}
	Surface(void)
	{
		a = b = c = d = 0;
	}
};

#endif