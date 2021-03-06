#include <Rcpp.h>

typedef std::pair<double, double> Point;

// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
double PerpendicularDistanceSquared(const Point &pt, const Point &lineStart, const Point &lineEnd)
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
    double numerator = doubleTriangleArea * doubleTriangleArea;

    return numerator / lineLengthSquared;
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

    std::vector<Point> pointsOut;
    RamerDouglasPeuckerCpp(points, epsilon * epsilon, pointsOut);

    size_t nOut = pointsOut.size();
    std::vector<double> xOut(nOut);
    std::vector<double> yOut(nOut);

    for (size_t i = 0; i < nOut; i++)
    {
        xOut[i] = pointsOut[i].first;
        yOut[i] = pointsOut[i].second;
    }

    return Rcpp::DataFrame::create(Rcpp::Named("x") = xOut, Rcpp::Named("y") = yOut);
}
