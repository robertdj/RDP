#include <cassert>
#include <stdexcept>
#include <tuple>
#include <vector>


namespace rdp {
struct Point2D
{
    double x;
    double y;
};


Point2D operator-(Point2D a, Point2D b)
{
    return {a.x - b.x, a.y - b.y};
}


double abs2(Point2D p)
{
    return p.x * p.x + p.y * p.y;
}


// Find the point with the maximum distance from line between start and end.
// Rearranging this formula to avoid recomputing constants:
// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
std::pair<double, size_t> findMostDivergentPoint(const std::vector<Point2D> &points,
                                                 size_t startIndex, size_t endIndex)
{
    assert(startIndex < endIndex && "Start index must be smaller than end index");
    assert(endIndex < points.size() && "End index is larger than the number of points");
    // The inequalities 0 <= startIndex < endIndex < points.size() imply that points.size() >= 2
    assert(points.size() >= 2 && "At least two points needed");

    double maxDistance = 0.0;
    size_t maxDistanceIndex = startIndex;

    Point2D startPoint = points[startIndex];
    Point2D endPoint = points[endIndex];

    Point2D lineDiff = endPoint - startPoint;

    double det1 = startPoint.y * (endPoint.x - startPoint.x) - startPoint.x * (endPoint.y - startPoint.y);

    for (size_t i = startIndex + 1; i != endIndex; ++i)
    {
        double numerator = det1 - points[i].y * lineDiff.x + points[i].x * lineDiff.y;
        double unscaledDistance = numerator * numerator;

        if (unscaledDistance > maxDistance)
        {
            maxDistanceIndex = i;
            maxDistance = unscaledDistance;
        }
    }

    maxDistance /= abs2(lineDiff);

    // Constructor is faster than initialization
    // return {maxDistance, maxDistanceIndex};
    return std::make_pair(maxDistance, maxDistanceIndex);
}


// `indicesToKeep` should be initialized with a 0 in the first entry.
void RamerDouglasPeucker(const std::vector<Point2D> &points, double epsilonSquared,
                         size_t startIndex, size_t endIndex, std::vector<size_t> &indicesToKeep)
{
    assert(startIndex < endIndex && "Start index must be smaller than end index");
    assert(endIndex < points.size() && "End index is larger than the number of points");
    // The inequalities 0 <= startIndex < endIndex < points.size() imply that points.size() >= 2
    assert(points.size() >= 2 && "At least two points needed");

    assert(epsilonSquared >= 0 && "epsilonSquared must be non-negative");

    assert(indicesToKeep.size() >= 1 && "indicesToKeep should be non-empty");
    assert(indicesToKeep[0] == 0 && "indicesToKeep should be initialized with a 0");

    auto [maxDistance, maxDistanceIndex] = findMostDivergentPoint(points, startIndex, endIndex);

    if (maxDistance > epsilonSquared)
    {
        // Recursive call
        RamerDouglasPeucker(points, epsilonSquared, startIndex, maxDistanceIndex, indicesToKeep);
        RamerDouglasPeucker(points, epsilonSquared, maxDistanceIndex, endIndex, indicesToKeep);
    }
    else
    {
        // startIndex is included from the previous run because we execute sequentially with the
        // lower parts of the list first
        indicesToKeep.push_back(endIndex);
    }
}
} // end namespace rdp
