#include "point.h"

std::istream& operator>> (std::istream& is, Point& p) {
    is >> p.x >> p.y >> p.z;
    return is;
}