#include <Rcpp.h>
#include "RamerDouglasPeucker.h"

//' Simplify a curve using the Ramer-Douglas-Peucker algorithm.
//'
//' Implements the [Ramer-Douglas-Peucker algorithm](https://en.wikipedia.org/wiki/Ramer-Douglas-Peucker_algorithm) for reducing the number of points on a curve.
//'
//' @details If there are no more than two points it does not make sense to simplify.
//' In this case the input is returned without further checks of `x` and `y`.
//' In particular, the input is not checked for `NA` values.
//'
//' @param x `[numeric]` The `x` values of the curve as a vector without `NA` values.
//' @param y `[numeric]` The `y` values of the curve as a vector without `NA` values.
//' @param epsilon `[positive numeric(1)]` The threshold for filtering outliers from the simplified curve.
//' @param keep_index `[logical]` If `TRUE`, returns a column called `index` with the index locations of points that are kept.
//'
//' @return A `data.frame` with `x` and `y` values of the simplified curve.
//'
//' @examples
//' RDP::RamerDouglasPeucker(x = c(0, 1, 3, 5), y = c(2, 1, 0, 1), epsilon = 0.5)
//' RDP::RamerDouglasPeucker(x = c(0, 1, 3, 5), y = c(2, 1, 0, 1), epsilon = 0.5, keep_index = TRUE)
//'
//' @export
//'
// [[Rcpp::export]]
Rcpp::DataFrame RamerDouglasPeucker(Rcpp::NumericVector x, Rcpp::NumericVector y, double epsilon,
                                    bool keep_index = false)
{
    R_xlen_t nPoints = x.length();
    if (nPoints != y.length())
        throw std::invalid_argument("x and y vectors must be of equal length");

    if (nPoints <= 2)
        return Rcpp::DataFrame::create(Rcpp::Named("x") = x, Rcpp::Named("y") = y);

    if (epsilon < 0 || Rcpp::NumericVector::is_na(epsilon))
        throw std::domain_error("epsilon must be a non-negative number");

    std::vector<rdp::Point2D> points;
    points.reserve(nPoints);
    for (R_xlen_t i = 0; i != nPoints; ++i)
    {
        if (Rcpp::NumericVector::is_na(x[i]) || Rcpp::NumericVector::is_na(y[i]))
            throw std::invalid_argument("NA values are not allowed in coordinates");

        points.push_back({x[i], y[i]});
    }

    // We don't know the number of indices to keep a priori, but the first point is always kept
    std::vector<std::size_t> indicesToKeep;
    indicesToKeep.reserve(nPoints);
    indicesToKeep.push_back(0);

    rdp::RamerDouglasPeucker(points, 0, nPoints - 1, epsilon * epsilon, indicesToKeep);

    // Here initialization is faster than reserve + push_back
    std::size_t nIndices = indicesToKeep.size();
    std::vector<double> xOut(nIndices);
    std::vector<double> yOut(nIndices);

    for (std::size_t i = 0; i != nIndices; ++i)
    {
        std::size_t index = indicesToKeep[i];
        xOut[i] = x[index];
        yOut[i] = y[index];
        // Add 1 to index before returning to R:
        indicesToKeep[i] += 1;
    }

    if (keep_index) {
        return Rcpp::DataFrame::create(Rcpp::Named("x") = xOut, Rcpp::Named("y") = yOut,
                                       Rcpp::Named("index") = indicesToKeep);
    } else {
        return Rcpp::DataFrame::create(Rcpp::Named("x") = xOut, Rcpp::Named("y") = yOut);
    }

}
