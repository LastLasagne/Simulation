// Dag Nylund, Universitetet i Innlandet
// Matematikk III 2025
#include <QVector3D>
#include <QVector2D>

#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>

struct  Vertex {
    float x;    //Position
    float y;
    float z;
	float nx;	//Normal
    float ny;
    float nz;
	float r;	//Vertex color
    float g;
    float b;

    float friction;

	Vertex() = default;
    Vertex(float x, float y, float z, float nx, float ny, float nz, float oldU, float oldV)
        : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), r(1.0f), g(1.0f), b(1.0f), friction(0.0f) {}
    Vertex(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b)
        : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), r(r), g(g), b(b), friction(0.0f) {}
    Vertex(QVector3D pos, QVector3D normal, QVector3D color);
    Vertex(QVector3D pos, QVector3D normal, QVector2D oldUV);

    //! Overloaded ostream operator which writes all vertex data on an open textfile stream
    friend std::ostream& operator<< (std::ostream&, const Vertex&);

    //! Overloaded ostream operator which reads all vertex data from an open textfile stream
    friend std::istream& operator>> (std::istream&, Vertex&);
};

#endif // VERTEX_H
