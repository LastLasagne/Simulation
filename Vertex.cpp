// Dag Nylund, Universitetet i Innlandet
// Matematikk III 2025

#include "Vertex.h"

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
    os << std::fixed;
    os << "(" << v.x << ", " << v.y << ", " << v.z << ") ";
    os << "(" << v.nx << ", " << v.ny << ", " << v.nz << ") ";
    os << "(" << v.r << ", " << v.g << "," << v.b << ") ";
    return os;
}
std::istream& operator>> (std::istream& is, Vertex& v) {
    is >> v.x >> v.y >> v.z;
    is >> v.nx >> v.ny >> v.nz;
	is >> v.r >> v.g >> v.b;
    return is;
}

Vertex::Vertex(QVector3D pos, QVector3D normal, QVector3D color)
{
	x = pos.x();
	y = pos.y();
	z = pos.z();
	nx = normal.x();
	ny = normal.y();
	nz = normal.z();
	r = color.x();
	g = color.y();
	b = color.z();
	friction = 0.0f;
}

Vertex::Vertex(QVector3D pos, QVector3D normal, QVector2D oldUV)
{
	x = pos.x();
	y = pos.y();
	z = pos.z();
	nx = normal.x();
	ny = normal.y();
	nz = normal.z();
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
	friction = 0.0f;
}
