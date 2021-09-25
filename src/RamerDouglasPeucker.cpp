#include <cassert>
#include <stdexcept>
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


// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
double PerpendicularDistanceSquared(Point2D pt, Point2D lineStart, Point2D lineEnd)
{
    Point2D lineDiff = lineEnd - lineStart;
    Point2D pointToLineStart = pt - lineStart;

    double lineLengthSquared = abs2(lineDiff);
    if (lineLengthSquared == 0.0)
    {
        // The line is just a point
        return abs2(pointToLineStart);
    }

    double determinant = lineDiff.y * pointToLineStart.x - lineDiff.x * pointToLineStart.y;
    double determinantSquared = determinant * determinant;

    return determinantSquared / lineLengthSquared;
}


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

    // Find the point with the maximum distance from line between start and end
    double maxDistance = 0.0;
    size_t maxDistanceIndex = startIndex;

    for (size_t i = startIndex + 1; i != endIndex; ++i)
    {
        double thisDistance = PerpendicularDistanceSquared(points[i], points[startIndex], points[endIndex]);
        if (thisDistance > maxDistance)
        {
            maxDistanceIndex = i;
            maxDistance = thisDistance;
        }
    }

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
