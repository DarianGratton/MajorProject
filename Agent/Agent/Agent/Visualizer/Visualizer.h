#pragma once

#include <matplot/matplot.h>

#include <string>
#include <vector>

#include "../Environment/Environment.h"

using namespace std;

class Visualizer
{
public:

	/*
	
	*/
	Visualizer() = default;

	void PlotLine(vector<float> X, vector<float> Y, string filename);
	void PlotBar(vector<float> X, vector<float> Y, string filename);
};