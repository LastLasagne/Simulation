// Dag Nylund, Universitetet i Innlandet
// Matematikk III 2025

#include "Vertex.h"

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
    os << std::fixed;
    os << "(" << v.x << ", " << v.y << ", " << v.z << ") ";
    os << "(" << v.r << ", " << v.g << ", " << v.b << ") ";
    os << "(" << v.u << ", " << v.v << ") ";
    return os;
}
std::istream& operator>> (std::istream& is, Vertex& v) {
    is >> v.x >> v.y >> v.z;
    is >> v.r >> v.g >> v.b;
    is >> v.u >> v.v;
    return is;
}

Vertex::Vertex(QVector3D pos, QVector3D normal, QVector2D uv)
{
	x = pos.x();
	y = pos.y();
	z = pos.z();
	r = normal.x();
	g = normal.y();
	b = normal.z();
	u = uv.x();
	v = uv.y();
}
