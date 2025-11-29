#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H
#include <vector>
#include "Vertex.h"
#include "Triangle.h"

struct CollisionObject
{
public:
	CollisionObject(QVector3D n, float d, float f) : normal(n), distance(d), friction(f) {}

	float distance;
    QVector3D normal;
	float friction;
};

#endif // COLLISIONOBJECT_H
