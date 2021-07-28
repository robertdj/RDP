#ifndef RDP
#define RDP

struct Point2D
{
    double x;
    double y;
};

void RamerDouglasPeuckerCpp(const std::vector<Point2D> &pointList, double epsilonSquared, size_t startIndex, size_t endIndex, std::vector<size_t> &indicesToKeep);

#endif
