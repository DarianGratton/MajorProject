#pragma once

#include <sciplot/sciplot.hpp>

#include <string>
#include <vector>

#include "../Environment/Environment.h"

namespace GameAgent
{

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
			- labelname: Name of the label used for the legend.
	*/
	void PlotLine(std::vector<float> X, std::vector<float> Y,
		std::string filename, std::string labelname);

	/*
		Creates a bar plot then stores it in a file.
		params:
			- X: Values to be plotted on the X axis
			- Y: Values to be plotted on the Y axis
			- filename: Name of the file for the plot to be stored in.
	*/
	void PlotBar(std::vector<float> X, std::vector<float> Y, std::string filename);

};

} /* namespace GameAgent */
