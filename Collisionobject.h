#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H
#include <vector>
#include "Vertex.h"

class Triangle;

class CollisionObject
{
public:
    CollisionObject(bool colliding, Triangle* tri, float d);

	float distance;
    Triangle* triangle;
	bool isColliding = false;
};

#endif // COLLISIONOBJECT_H
