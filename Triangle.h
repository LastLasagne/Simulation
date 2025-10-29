#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "VisualObject.h"
#include "Collisionobject.h"
#include "Vertex.h"

//Simple 90 45 45 degree triangle
class Triangle : public VisualObject {
public:

    Triangle();
	Triangle(QVector3D p1, QVector3D p2, QVector3D p3);

	CollisionObject* TriangleSphereCollision(QVector3D p, float radius);

	QVector3D normal;
};

#endif // TRIANGLE_H
