#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H
#include <vector>
#include "Vertex.h"

class Triangle;

class CollisionObject
{
public:
    CollisionObject(bool colliding, QVector3D n, float d);

	float distance;
    QVector3D normal;
	bool isColliding = false;
};

#endif // COLLISIONOBJECT_H
