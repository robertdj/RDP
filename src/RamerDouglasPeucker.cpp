#include <Rcpp.h>

typedef std::pair<double, double> Point;

double PerpendicularDistanceSquared(const Point &pt, const Point &lineStart, const Point &lineEnd)
{
    double dx = lineEnd.first - lineStart.first;
    double dy = lineEnd.second - lineStart.second;

    // Normalise
    double mag = sqrt(dx*dx + dy*dy);
    if (mag > 0.0)
    {
        dx /= mag;
        dy /= mag;
    }

    double pvx = pt.first - lineStart.first;
    double pvy = pt.second - lineStart.second;

    // Get dot product (project pv onto normalized direction)
    double pvdot = dx * pvx + dy * pvy;

    // Scale line direction vector
    double dsx = pvdot * dx;
    double dsy = pvdot * dy;

    // Subtract this from pv
    double ax = pvx - dsx;
    double ay = pvy - dsy;

    return ax*ax + ay*ay;
}


void RamerDouglasPeuckerCpp(const std::vector<Point> &pointList, double epsilonSquared, std::vector<Point> &out)
{
    if (pointList.size() < 2)
        throw std::invalid_argument("Not enough points to simplify");

    // Find the point with the maximum distance from line between start and end
    double dmax = 0.0;
    size_t index = 0;

    size_t end = pointList.size() - 1;
    for (size_t i = 1; i < end; i++)
    {
        double d = PerpendicularDistanceSquared(pointList[i], pointList[0], pointList[end]);
        if (d > dmax)
        {
            index = i;
            dmax = d;
        }
    }

    // If max distance is greater than epsilon, recursively simplify
    if (dmax > epsilonSquared)
    {
        // Recursive call
        std::vector<Point> recResults1;
        std::vector<Point> recResults2;
        std::vector<Point> firstLine(pointList.begin(), pointList.begin() + index + 1);
        std::vector<Point> lastLine(pointList.begin() + index, pointList.end());
        RamerDouglasPeuckerCpp(firstLine, epsilonSquared, recResults1);
        RamerDouglasPeuckerCpp(lastLine, epsilonSquared, recResults2);

        // Build the result list
        out.assign(recResults1.begin(), recResults1.end() - 1);
        out.insert(out.end(), recResults2.begin(), recResults2.end());
        if (out.size() < 2)
            throw std::runtime_error("Problem assembling output");
    }
    else
    {
        // Just return start and end points
        out.clear();
        out.push_back(pointList[0]);
        out.push_back(pointList[end]);
    }
}


//' Ramer-Douglas-Peucker
//'
//' The Ramer-Douglas-Peucker algorithm for reducing the number of points on a curve.
//'
//' @param x The `x` values of the curve as a vector.
//' @param y The `y` values of the curve as a vector.
//' @param epsilon The threshold for filtering outliers from the simplified curve.
//'
//' @return A `data.frame` with `x` and `y` values of the simplified curve.
//'
//' @examples
//' RDP::RamerDouglasPeucker(x = c(0, 1, 3, 5), y = c(2, 1, 0, 1), epsilon = 0.5)
//'
//' @export
//'
// [[Rcpp::export]]
Rcpp::DataFrame RamerDouglasPeucker(Rcpp::NumericVector x, Rcpp::NumericVector y, double epsilon)
{
    auto nx = x.length();

    if (nx != y.length())
        throw std::invalid_argument("x and y vectors must be of equal length");

    if (epsilon <= 0)
        throw std::invalid_argument("epsilon must be positive");

    std::vector<Point> points(nx);

    for (R_xlen_t i = 0; i < nx; i++)
    {
        points[i] = Point(x[i], y[i]);
    }

    double epsilonSquared = epsilon * epsilon;
    std::vector<Point> pointsOut;
    RamerDouglasPeuckerCpp(points, epsilonSquared, pointsOut);

    auto nOut = pointsOut.size();
    std::vector<double> xOut(nOut);
    std::vector<double> yOut(nOut);

    for (size_t i = 0; i < nOut; i++)
    {
        xOut[i] = pointsOut[i].first;
        yOut[i] = pointsOut[i].second;
    }

    return Rcpp::DataFrame::create(Rcpp::Named("x") = xOut, Rcpp::Named("y") = yOut);
}
