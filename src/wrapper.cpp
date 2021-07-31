#include <Rcpp.h>
#include "RamerDouglasPeucker.h"

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
    if (epsilon < 0 || Rcpp::NumericVector::is_na(epsilon))
        throw std::domain_error("epsilon must be a non-negative number");

    R_xlen_t nx = x.length();
    if (nx != y.length())
        throw std::invalid_argument("x and y vectors must be of equal length");

    std::vector<rdp::Point2D> points;
    points.reserve(nx);
    for (R_xlen_t i = 0; i < nx; i++)
    {
        if (Rcpp::NumericVector::is_na(x[i]) || Rcpp::NumericVector::is_na(y[i]))
            throw std::invalid_argument("NA values are not allowed in coordinates");

        points.push_back({x[i], y[i]});
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
