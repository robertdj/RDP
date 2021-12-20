#ifndef RDP
#define RDP

namespace rdp {
struct Point2D
{
    double x;
    double y;
};

void RamerDouglasPeucker(const std::vector<Point2D> &pointList, size_t startIndex, size_t endIndex,
                         double epsilonSquared, std::vector<size_t> &indicesToKeep);
}

#endif
