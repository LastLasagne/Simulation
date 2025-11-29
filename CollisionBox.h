#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "VisualObject.h"
#include "Vertex.h"

class CollisionBox : public VisualObject {
public:

    CollisionBox(QVector3D pos, float scale);

    //bounds
	float boundsMinX = -1.0f;
	float boundsMinY = -1.0f;
	float boundsMinZ = -1.0f;
	float boundsMaxX = 1.0f;
	float boundsMaxY = 1.0f;
	float boundsMaxZ = 1.0f;
};

#endif // COLLISIONBOX_H