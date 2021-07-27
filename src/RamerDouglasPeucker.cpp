#include <Rcpp.h>

typedef std::pair<double, double> Point;

// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
double PerpendicularDistanceSquared(Point pt, Point lineStart, Point lineEnd)
{
    double xLineDiff = lineEnd.first - lineStart.first;
    double yLineDiff = lineEnd.second - lineStart.second;

    double xPointToLineStart = pt.first - lineStart.first;
    double yPointToLineStart = pt.second - lineStart.second;

    double lineLengthSquared = xLineDiff * xLineDiff + yLineDiff * yLineDiff;
    if (lineLengthSquared == 0)
    {
        // The line is just a point
        return xPointToLineStart * xPointToLineStart + yPointToLineStart * yPointToLineStart;
    }

    double doubleTriangleArea = yLineDiff * xPointToLineStart - xLineDiff * yPointToLineStart;
    double doubleTriangleAreaSquared = doubleTriangleArea * doubleTriangleArea;

    return doubleTriangleAreaSquared / lineLengthSquared;
}


void RamerDouglasPeuckerCpp(const std::vector<Point> &pointList, double epsilonSquared, size_t startIndex, size_t endIndex, std::vector<size_t> &indicesToKeep)
{
    if (pointList.size() < 2)
        throw std::invalid_argument("Not enough points to simplify");

    // Find the point with the maximum distance from line between start and end
    double dmax = 0.0;
    size_t index = startIndex;

    size_t end = pointList.size() - 1;
    for (size_t i = startIndex + 1; i < endIndex; i++)
    {
        double d = PerpendicularDistanceSquared(pointList[i], pointList[startIndex], pointList[endIndex]);
        if (d > dmax)
        {
            index = i;
            dmax = d;
        }
    }

    if (dmax > epsilonSquared)
    {
        // Recursive call
        RamerDouglasPeuckerCpp(pointList, epsilonSquared, startIndex, index, indicesToKeep);
        RamerDouglasPeuckerCpp(pointList, epsilonSquared, index, endIndex, indicesToKeep);
    }
    else
    {
        // startIndex is included from the previous run
        indicesToKeep.push_back(endIndex);
    }
}


//' Ramer-Douglas-Peucker
//'
//' The [Ramer-Douglas-Peucker algorithm](https://en.wikipedia.org/wiki/Ramer-Douglas-Peucker_algorithm) for reducing the number of points on a curve.
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
    if (epsilon <= 0 || Rcpp::NumericVector::is_na(epsilon))
        throw std::domain_error("epsilon must be a positive number");

    R_xlen_t nx = x.length();
    if (nx != y.length())
        throw std::invalid_argument("x and y vectors must be of equal length");

    std::vector<Point> points(nx);
    for (R_xlen_t i = 0; i < nx; i++)
    {
        if (Rcpp::NumericVector::is_na(x[i]) || Rcpp::NumericVector::is_na(y[i]))
            throw std::invalid_argument("NA values are not allowed in coordinates");

        points[i] = Point(x[i], y[i]);
    }

    std::vector<size_t> indicesToKeep;
    indicesToKeep.reserve(nx);
    // The first point is always kept
    indicesToKeep.push_back(0);

    RamerDouglasPeuckerCpp(points, epsilon * epsilon, 0, nx - 1, indicesToKeep);

    size_t nOut = indicesToKeep.size();
    std::vector<double> xOut(nOut);
    std::vector<double> yOut(nOut);

    for (size_t i = 0; i < nOut; i++)
    {
        size_t index = indicesToKeep[i];
        xOut[i] = x[index];
        yOut[i] = y[index];
    }

    return Rcpp::DataFrame::create(Rcpp::Named("x") = xOut, Rcpp::Named("y") = yOut);
}
