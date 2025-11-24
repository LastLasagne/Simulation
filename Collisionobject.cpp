#include "collisionobject.h"
#include "Triangle.h"

CollisionObject::CollisionObject(bool colliding, QVector3D n, float d)
{
	isColliding = colliding;
	normal = n;
	distance = d;
}
