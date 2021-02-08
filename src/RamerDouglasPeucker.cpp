#include <Rcpp.h>
using namespace Rcpp;

#include <cmath>
#include <vector>
using namespace std;

typedef std::pair<double, double> Point;

double PerpendicularDistance(const Point &pt, const Point &lineStart, const Point &lineEnd)
{
	double dx = lineEnd.first - lineStart.first;
	double dy = lineEnd.second - lineStart.second;

	//Normalise
	double mag = pow(pow(dx,2.0)+pow(dy,2.0),0.5);
	if(mag > 0.0)
	{
		dx /= mag; dy /= mag;
	}

	double pvx = pt.first - lineStart.first;
	double pvy = pt.second - lineStart.second;

	//Get dot product (project pv onto normalized direction)
	double pvdot = dx * pvx + dy * pvy;

	//Scale line direction vector
	double dsx = pvdot * dx;
	double dsy = pvdot * dy;

	//Subtract this from pv
	double ax = pvx - dsx;
	double ay = pvy - dsy;

	return pow(pow(ax,2.0)+pow(ay,2.0),0.5);
}

void RamerDouglasPeucker(const vector<Point> &pointList, double epsilon, vector<Point> &out)
{
	if(pointList.size()<2)
		throw invalid_argument("Not enough points to simplify");

	// Find the point with the maximum distance from line between start and end
	double dmax = 0.0;
	size_t index = 0;
	size_t end = pointList.size()-1;
	for(size_t i = 1; i < end; i++)
	{
		double d = PerpendicularDistance(pointList[i], pointList[0], pointList[end]);
		if (d > dmax)
		{
			index = i;
			dmax = d;
		}
	}

	// If max distance is greater than epsilon, recursively simplify
	if(dmax > epsilon)
	{
		// Recursive call
		vector<Point> recResults1;
		vector<Point> recResults2;
		vector<Point> firstLine(pointList.begin(), pointList.begin()+index+1);
		vector<Point> lastLine(pointList.begin()+index, pointList.end());
		RamerDouglasPeucker(firstLine, epsilon, recResults1);
		RamerDouglasPeucker(lastLine, epsilon, recResults2);

		// Build the result list
		out.assign(recResults1.begin(), recResults1.end()-1);
		out.insert(out.end(), recResults2.begin(), recResults2.end());
		if(out.size()<2)
			throw runtime_error("Problem assembling output");
	}
	else
	{
		//Just return start and end points
		out.clear();
		out.push_back(pointList[0]);
		out.push_back(pointList[end]);
	}
}

//' Ramer-Douglas-Peucker
//'
//' The Ramer-Douglas-Peucker algorithm for reducing the number of points on a curve.
//'
//' @param x The `x` values of the curve.
//' @param y The `y` values of the curve.
//' @param epsilon The threshold for filtering outliers from the simplified curve.
//'
//' @return The `x` and `y` values of the simplified curve.
//'
//' @export
//'
// [[Rcpp::export]]
Rcpp::DataFrame RDP(Rcpp::NumericVector x, Rcpp::NumericVector y, double epsilon) {
    size_t nx = x.length();

    if (nx != y.length())
        throw invalid_argument("x and y vectors must be of equal length");

    if (epsilon <= 0)
        throw invalid_argument("epsilon must be positive");

    vector<Point> points(nx);

    for (size_t i = 0; i < nx; i++) {
        points[i] = Point(x[i], y[i]);
    }

    vector<Point> pointsOut;
    RamerDouglasPeucker(points, epsilon, pointsOut);

    size_t n_out = pointsOut.size();
    vector<double> x_out(n_out);
    vector<double> y_out(n_out);

    for (size_t i = 0; i < n_out; i++) {
        x_out[i] = pointsOut[i].first;
        y_out[i] = pointsOut[i].second;
    }

    return Rcpp::DataFrame::create(Rcpp::Named("x") = x_out, Rcpp::Named("y") = y_out);
}
