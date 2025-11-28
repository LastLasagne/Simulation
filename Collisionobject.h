#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H
#include <vector>
#include "Vertex.h"
#include "Triangle.h"

struct CollisionObject
{
public:
	CollisionObject(bool c, QVector3D n, float d, float f) : isColliding(c), normal(n), distance(d), friction(f) {}

	float distance;
    QVector3D normal;
	bool isColliding = false;
	float friction;
};

#endif // COLLISIONOBJECT_H
