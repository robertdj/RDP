#ifndef RDP
#define RDP

namespace rdp {
struct Point2D
{
    double x;
    double y;
};

void RamerDouglasPeucker(const std::vector<rdp::Point2D> &pointList, double epsilonSquared, size_t startIndex, size_t endIndex, std::vector<size_t> &indicesToKeep);
}

#endif
