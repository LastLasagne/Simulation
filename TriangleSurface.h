#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include <string>
#include "VisualObject.h"
#include "Triangle.h"
#include "Collisionobject.h"
#include "point.h"

//Defaults to a quad, but can read a mesh from file
class TriangleSurface : public VisualObject
{
private:
	std::vector<Point> points;

    std::vector<float> heights;
    std::vector<int> counts;

    float xMin = FLT_MAX;
	float xMax = -FLT_MAX;
    float yMin = FLT_MAX;
    float yMax = -FLT_MAX;
	float zMin = FLT_MAX;
	float zMax = -FLT_MAX;

	int rowColCount = 0;
	int size = 0;

	const float RESOLUTION = 0.001f;
	const float HEIGHT_SCALE = 1.0f;

public:
    TriangleSurface();
    TriangleSurface(const std::string& filename);

    CollisionObject* SurfaceSphereCollision(QVector3D position, float radius);

    float GetHeight(int index);
	void CalculateHeights();
};

#endif // TRIANGLESURFACE_H
