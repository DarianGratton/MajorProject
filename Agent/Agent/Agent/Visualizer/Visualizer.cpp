#include "Visualizer.h"

void Visualizer::PlotLine(vector<float> X, vector<float> Y, string filename)
{
	matplot::plot(X, Y)->line_width(2).color("red");
	matplot::save(filename, "png");
}

void Visualizer::PlotBar(vector<float> X, vector<float> Y, string filename)
{
	matplot::bar(X, Y);
	matplot::save(filename, "png");
}
