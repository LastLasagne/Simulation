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

    Point min;
    Point max;

	int size = 0;
    int colCount = 0;
    int rowCount = 0;

	const float RESOLUTION = 0.05f;
	const float HEIGHT_SCALE = 2.0f;

public:
    TriangleSurface();
    TriangleSurface(const std::string& filename);
    //TriangleSurface(bool toggle, const std::string& filename);

    CollisionObject* SurfaceSphereCollision(QVector3D position, float radius);

    float SampleHeight(int x, int y);
    void CalculateHeights();
};

#endif // TRIANGLESURFACE_H
