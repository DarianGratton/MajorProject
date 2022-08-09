#include "Visualizer.h"

void Visualizer::PlotLine(std::vector<float> X, std::vector<float> Y, std::string filename)
{
	matplot::plot(X, Y)->line_width(2).color("red");
	matplot::save(filename, "png");
}

void Visualizer::PlotBar(std::vector<float> X, std::vector<float> Y, std::string filename)
{
	matplot::bar(X, Y);
	matplot::save(filename, "png");
}
