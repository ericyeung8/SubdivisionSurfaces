#ifndef SUBDIVISION_H
#define SUBDIVISION_H

#include "stdafx.h"
#include <vector>
#include <map>
#include "DataStruc.h"

int getVert(int i1, int i2, std::vector <Vertex> &v);
void LinearSub(std::vector <Vertex> &newv, std::vector <Surface> &f);
void Averaging(std::vector <Vertex> &v, std::vector <Surface> &f);

#endif