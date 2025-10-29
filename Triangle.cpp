#include "Triangle.h"

Triangle::Triangle() : VisualObject()
{
    mVertices.push_back(Vertex{-0.5f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f});
    mVertices.push_back(Vertex{-0.5f,  -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f});
    mVertices.push_back(Vertex{ 0.0f,   0.0f,  0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f});

	normal = QVector3D(0, 0, 1);
}

Triangle::Triangle(QVector3D p1, QVector3D p2, QVector3D p3)
{
	mVertices.push_back(Vertex{ p1, QVector3D(0,0,1), QVector2D(0,0) });
	mVertices.push_back(Vertex{ p2, QVector3D(0,0,1), QVector2D(0,1) });
	mVertices.push_back(Vertex{ p3, QVector3D(0,0,1), QVector2D(1,0) });

	QVector3D v1 = p2 - p1;
	QVector3D v2 = p3 - p1;
	normal = QVector3D::crossProduct(v1, v2);
	normal.normalize();
}

CollisionObject* Triangle::TriangleSphereCollision(QVector3D p, float radius)
{
	Vertex v0 = mVertices[0];
	Vertex v1 = mVertices[1];
	Vertex v2 = mVertices[2];

	float w1 = ((p.x() - v2.x) * (v0.y - v2.y) - (v0.x - v2.x) * (p.y() - v2.y));
	float w2 = ((p.x() - v0.x) * (v1.y - v0.y) - (v1.x - v0.x) * (p.y() - v0.y));
	float w0 = ((p.x() - v1.x) * (v2.y - v1.y) - (v2.x - v1.x) * (p.y() - v1.y));

	float w = sqrt(pow(w0, 2) + pow(w1, 2) + pow(w2, 2));
	w0 /= w;
	w1 /= w;
	w2 /= w;

	bool hasNegative = (w0 < 0) || (w1 < 0) || (w2 < 0);
	bool hasPositive = (w0 > 0) || (w1 > 0) || (w2 > 0);

	bool inTri = !(hasNegative && hasPositive);

	if (inTri)
	{
		QVector3D weights = QVector3D(w0 * w0, w1 * w1, w2 * w2);
		QVector3D zVector = QVector3D(v0.z, v1.z, v2.z);
		float z = QVector3D::dotProduct(weights, zVector);
		float distance = p.z() - z;
		bool colliding = distance < radius;
		return new CollisionObject(colliding, this, distance);
	}
	return nullptr;
}
