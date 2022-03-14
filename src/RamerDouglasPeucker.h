#ifndef RDP
#define RDP

namespace rdp {
struct Point2D
{
    double x;
    double y;
};

void RamerDouglasPeucker(const std::vector<Point2D> &pointList, std::size_t startIndex, std::size_t endIndex,
                         double epsilonSquared, std::vector<std::size_t> &indicesToKeep);
}

#endif
