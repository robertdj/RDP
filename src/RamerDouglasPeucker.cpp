#include <vector>
#include <limits>
#include <stdexcept>

struct Point2D
{
    double x;
    double y;

    double abs2()
    {
        return x * x + y * y;
    }
};


Point2D operator-(Point2D a, Point2D b)
{
    return {a.x - b.x, a.y - b.y};
}


// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
double PerpendicularDistanceSquared(Point2D pt, Point2D lineStart, Point2D lineEnd)
{
    Point2D lineDiff = lineEnd - lineStart;
    Point2D pointToLineStart = pt - lineStart;

    double lineLengthSquared = lineDiff.abs2();
    if (lineLengthSquared < std::numeric_limits<double>::epsilon())
    {
        // The line is just a point
        return pointToLineStart.abs2();
    }

    double doubleTriangleArea = lineDiff.y * pointToLineStart.x - lineDiff.x * pointToLineStart.y;
    double doubleTriangleAreaSquared = doubleTriangleArea * doubleTriangleArea;

    return doubleTriangleAreaSquared / lineLengthSquared;
}


void RamerDouglasPeuckerCpp(const std::vector<Point2D> &pointList, double epsilonSquared, size_t startIndex, size_t endIndex, std::vector<size_t> &indicesToKeep)
{
    if (pointList.size() < 2)
        throw std::invalid_argument("Not enough points to simplify");

    // Find the point with the maximum distance from line between start and end
    double maxDistance = 0.0;
    size_t maxDistanceIndex = startIndex;

    size_t end = pointList.size() - 1;
    for (size_t i = startIndex + 1; i < endIndex; i++)
    {
        double thisDistance = PerpendicularDistanceSquared(pointList[i], pointList[startIndex], pointList[endIndex]);
        if (thisDistance > maxDistance)
        {
            maxDistanceIndex = i;
            maxDistance = thisDistance;
        }
    }

    if (maxDistance > epsilonSquared)
    {
        // Recursive call
        RamerDouglasPeuckerCpp(pointList, epsilonSquared, startIndex, maxDistanceIndex, indicesToKeep);
        RamerDouglasPeuckerCpp(pointList, epsilonSquared, maxDistanceIndex, endIndex, indicesToKeep);
    }
    else
    {
        // startIndex is included from the previous run because we execute sequentially with the
        // lower parts first
        indicesToKeep.push_back(endIndex);
    }
}
