#include "Visualizer.h"

void Visualizer::PlotLine(std::vector<float> X, std::vector<float> Y, 
						  std::string filename, std::string labelname)
{
	// Create Plot
	sciplot::Plot2D plot;
	plot.xlabel("X");
	plot.ylabel("Y");
	
	// Create Plot Legend
	plot.legend()
		.atOutsideBottom()
		.displayHorizontal()
		.displayExpandWidthBy(2);

	// Plot Graph
	plot.drawCurve(X, Y).label(labelname);
	sciplot::Figure fig = { { plot } };
	sciplot::Canvas canvas = { { fig } };
	canvas.size(750, 750);
	canvas.save(filename);
}

void Visualizer::PlotBar(std::vector<float> X, std::vector<float> Y, std::string filename)
{
	// Setup Plot
	sciplot::Plot2D plot;
	plot.legend().atOutsideLeft();
	plot.ylabel("Y");

	// Plot the boxes using y values.
	sciplot::Strings xStrings;
	std::transform(X.begin(), X.end(), std::back_inserter(xStrings), [](auto d) { return std::to_string(d); });
	std::vector<float> boxWidthes = { 0.5, 0.5, 0.5, 0.5 };
	plot.drawBoxes(xStrings, Y, boxWidthes)
		.fillSolid()
		.fillColor("red")
		.fillIntensity(1.0)
		.borderShow()
		.labelNone();

	// Adjust the relative width of the boxes
	plot.boxWidthRelative(0.75);
	plot.autoclean(false);

	// Save Graph
	sciplot::Figure fig = { { plot } };
	sciplot::Canvas canvas = { { fig } };
	canvas.size(750, 750);
	canvas.save(filename);
}
