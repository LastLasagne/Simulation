#include "CollisionBox.h"

CollisionBox::CollisionBox(QVector3D pos, float s) : VisualObject()
{
    drawType = 0;

	setPosition(pos.x(), pos.y(), pos.z());
	scale(s);

	mVertices.push_back(Vertex{ boundsMinX, boundsMinY, boundsMinZ, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
	mVertices.push_back(Vertex{ boundsMaxX, boundsMinY, boundsMinZ, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
	mVertices.push_back(Vertex{ boundsMaxX, boundsMaxY, boundsMinZ, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
	mVertices.push_back(Vertex{ boundsMinX, boundsMaxY, boundsMinZ, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
	mVertices.push_back(Vertex{ boundsMinX, boundsMinY, boundsMaxZ, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
	mVertices.push_back(Vertex{ boundsMaxX, boundsMinY, boundsMaxZ, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
	mVertices.push_back(Vertex{ boundsMaxX, boundsMaxY, boundsMaxZ, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
	mVertices.push_back(Vertex{ boundsMinX, boundsMaxY, boundsMaxZ, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });

	boundsMinX = boundsMinX * s + pos.x();
	boundsMinY = boundsMinY * s + pos.y();
	boundsMinZ = boundsMinZ * s + pos.z();
	boundsMaxX = boundsMaxX * s + pos.x();
	boundsMaxY = boundsMaxY * s + pos.y();
	boundsMaxZ = boundsMaxZ * s + pos.z();

	unsigned int idx[] = {
		0, 1, 2,   0, 2, 3,
		1, 5, 6,   1, 6, 2,
		5, 4, 7,   5, 7, 6,
		4, 0, 3,   4, 3, 7,
		4, 1, 5,   4, 0, 1,
		3, 2, 6,   3, 6, 7
	};

    mIndices.assign(idx, idx + 36);
}