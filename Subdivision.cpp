#include "stdafx.h"
#include <vector>
#include <map>
#include "DataStruc.h"

typedef std::pair<int, int> TableKey;
std::map <TableKey, int> MyMap;

int getVert(int i1, int i2, std::vector <Vertex> &v)
{
	int temp;
	Vertex midv;
	std::map <TableKey, int>::iterator it;

	//orderless i1 and i2
	if (i1 > i2)
	{
		temp = i2;
		i2 = i1;
		i1 = temp;
	}

	it = MyMap.find(TableKey(i1, i2));

	//if the midpoint do not exist, add it
	if (it == MyMap.end())
	{
		midv.x = (v[i1].x + v[i2].x) / 2;
		midv.y = (v[i1].y + v[i2].y) / 2;
		midv.z = (v[i1].z + v[i2].z) / 2;
		v.push_back(midv);

		MyMap.insert(std::pair<TableKey, int>(TableKey(i1, i2), v.size() - 1));
	}

	it = MyMap.find(TableKey(i1, i2));

	return it->second;
}

void LinearSub(std::vector <Vertex> &newv, std::vector <Surface> &f)
{
	std::vector <Surface> newf;
	int i, j;
	int e[4] = { 0, 0, 0, 0 };
	float x, y, z;
	int c;

	for (i = 0; i < f.size(); i++)
	{
		//Calculate the midpoint
		e[0] = getVert(f[i].a - 1, f[i].b - 1, newv);
		e[1] = getVert(f[i].b - 1, f[i].c - 1, newv);
		e[2] = getVert(f[i].c - 1, f[i].d - 1, newv);
		e[3] = getVert(f[i].d - 1, f[i].a - 1, newv);

		//Calculate the centroid
		x = (newv[f[i].a - 1].x + newv[f[i].b - 1].x + newv[f[i].c - 1].x + newv[f[i].d - 1].x) / 4;
		y = (newv[f[i].a - 1].y + newv[f[i].b - 1].y + newv[f[i].c - 1].y + newv[f[i].d - 1].y) / 4;
		z = (newv[f[i].a - 1].z + newv[f[i].b - 1].z + newv[f[i].c - 1].z + newv[f[i].d - 1].z) / 4;

		newv.push_back(Vertex(x, y, z));
		c = newv.size();

		//Add four new surface
		newf.push_back(Surface(f[i].a, e[0] + 1, c, e[3] + 1));
		newf.push_back(Surface(f[i].b, e[1] + 1, c, e[0] + 1));
		newf.push_back(Surface(f[i].c, e[2] + 1, c, e[1] + 1));
		newf.push_back(Surface(f[i].d, e[3] + 1, c, e[2] + 1));
	}

	f = newf;
}

void Averaging(std::vector <Vertex> &v, std::vector <Surface> &f)
{
	std::vector <Vertex> newv;
	std::vector <int> val;
	float x, y, z;
	int i;

	//Initialize the newv and val
	for (i = 0; i < v.size(); i++)
	{
		newv.push_back(Vertex());
		val.push_back(0);
	}

	for (i = 0; i < f.size(); i++)
	{
		Vertex centroid;

		//Calculate the centroid
		x = (v[f[i].a - 1].x + v[f[i].b - 1].x + v[f[i].c - 1].x + v[f[i].d - 1].x) / 4;
		y = (v[f[i].a - 1].y + v[f[i].b - 1].y + v[f[i].c - 1].y + v[f[i].d - 1].y) / 4;
		z = (v[f[i].a - 1].z + v[f[i].b - 1].z + v[f[i].c - 1].z + v[f[i].d - 1].z) / 4;

		centroid = Vertex(x, y, z);
		newv[f[i].a - 1].x += centroid.x;
		newv[f[i].a - 1].y += centroid.y;
		newv[f[i].a - 1].z += centroid.z;
		val[f[i].a - 1] += 1;

		newv[f[i].b - 1].x += centroid.x;
		newv[f[i].b - 1].y += centroid.y;
		newv[f[i].b - 1].z += centroid.z;
		val[f[i].b - 1] += 1;

		newv[f[i].c - 1].x += centroid.x;
		newv[f[i].c - 1].y += centroid.y;
		newv[f[i].c - 1].z += centroid.z;
		val[f[i].c - 1] += 1;

		newv[f[i].d - 1].x += centroid.x;
		newv[f[i].d - 1].y += centroid.y;
		newv[f[i].d - 1].z += centroid.z;
		val[f[i].d - 1] += 1;
	}

	for (i = 0; i < newv.size(); i++)
	{
		newv[i].x /= val[i];
		newv[i].y /= val[i];
		newv[i].z /= val[i];
	}

	v = newv;
}