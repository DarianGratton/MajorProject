#pragma once

#include <matplot/matplot.h>

#include <string>
#include <vector>

#include "../Environment/Environment.h"

using namespace std;

/* 
  Visualizer

  A class that defines various functions for creating visual graphs and 
  charts. Useful for creating various visuals of the variables obtained from
  the Agent and it's environment.

  Requires gnuplot 5.2.8.

  Author: Darian G.
*/
class Visualizer
{
public:

	/*
	  Default Constructor.
	*/
	Visualizer() = default;

	/*
	  Creates a line plot then stores it in a file.
	  params:
			- X: Values to be plotted on the X axis
			- Y: Values to be plotted on the Y axis
			- filename: Name of the file for the plot to be stored in.
	*/
	void PlotLine(vector<float> X, vector<float> Y, string filename);

	/*
	  Creates a bar plot then stores it in a file.
	  params:
			- X: Values to be plotted on the X axis
			- Y: Values to be plotted on the Y axis
			- filename: Name of the file for the plot to be stored in.
	*/
	void PlotBar(vector<float> X, vector<float> Y, string filename);

};