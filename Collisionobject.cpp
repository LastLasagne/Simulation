#include "collisionobject.h"
#include "Triangle.h"

CollisionObject::CollisionObject(bool colliding, Triangle* t, float d)
{
	isColliding = colliding;
	triangle = t;
	distance = d;
}
