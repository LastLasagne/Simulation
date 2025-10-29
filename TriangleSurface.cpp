#include "TriangleSurface.h"
#include <fstream>
#include <QDebug>
#include "point.h"
TriangleSurface::TriangleSurface() : VisualObject()
{
	Vertex v1{ 0.0f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f };  //  bottom-left corner
	Vertex v2{ 1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f }; //  bottom-right corner
	Vertex v3{ 0.0f,  1.0f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f }; //  top-left corner
	Vertex v4{ 1.0f,  1.0f,  0.0f,   1.0f, 1.0f, 0.0f,  1.0f, 1.0f }; //  top-right corner

	//Pushing 1st triangle,
	mVertices.push_back(v1);
	mVertices.push_back(v2);
	mVertices.push_back(v3);
	mVertices.push_back(v4);

	//Indexes for the two triangles to form a quad
	mIndices.push_back(0);
	mIndices.push_back(1);
	mIndices.push_back(2);
	mIndices.push_back(2);
	mIndices.push_back(1);
	mIndices.push_back(3);

	//Temporary scale and positioning
	mMatrix.scale(0.5f);
	mMatrix.translate(0.5f, 0.1f, 0.1f);
}

//TriangleSurface::TriangleSurface(const std::string &filename)
//{
//    std::ifstream inn(filename);
//    if (!inn.is_open())
//        return;
//    // read input from math part of compulsory
//    int n;  
//    Vertex v;
//    inn >> n;
//    for (auto i=0; i<n; i++)
//    {
//        inn >> v;
//		qDebug() << "Read vertex: " << v.x << v.y << v.z << v.r << v.g << v.b << v.u << v.v;
//        mVertices.push_back(v);
//        //qDebug() << v.x << v.y << v.z;
//    }
//    inn.close();
//}

TriangleSurface::TriangleSurface(const std::string& filename)
{
	std::ifstream inn(filename);
	if (!inn.is_open())
	{
		qDebug() << "Failed to open file: " << QString::fromStdString(filename);
		return;
	}
	int n;
	Point v;
	inn >> n;
	for (auto i = 0; i < n; i++)
	{
		inn >> v;
		//qDebug() << "Read vertex: " << v.x << v.y << v.z << v.r << v.g << v.b << v.u << v.v;
		points.push_back(v);
		if (v.x < xMin)
			xMin = v.x;
		if (v.x > xMax)
			xMax = v.x;
		if (v.y < yMin)
			yMin = v.y;
		if (v.y > yMax)
			yMax = v.y;
		if (v.z < zMin)
			zMin = v.z;
		if (v.z > zMax)
			zMax = v.z;
	}
	inn.close();

	rowColCount = static_cast<int>((std::max(xMax, yMax) - std::min(xMin, yMin)) / RESOLUTION) + 1;
	size = rowColCount * rowColCount;

	heights.resize(size, 0);
	counts.resize(size, 0);
	for (int i = 0; i < points.size(); i++)
	{
		Point p = points[i];
		int xIndex = static_cast<int>((p.x - xMin) / RESOLUTION);
		int yIndex = static_cast<int>((p.y - yMin) / RESOLUTION);
		int index = yIndex * rowColCount + xIndex;
		heights[index] += p.z;
		counts[index] += 1;
	}

	CalculateHeights();

	for (int i = 0; i < size; i++)
	{
		float height = (heights[i] - zMin) * HEIGHT_SCALE;
		mVertices.push_back(Vertex(
			(i % rowColCount) * RESOLUTION,
			(i / rowColCount) * RESOLUTION,
			height,
			height, height, height,
			0.0f, 0.0f));
	}

	for (int y = 0; y < rowColCount - 1; y++)
	{
		for (int x = 0; x < rowColCount - 1; x++)
		{
			int topLeft = y * rowColCount + x;
			int topRight = topLeft + 1;
			int bottomLeft = (y + 1) * rowColCount + x;
			int bottomRight = bottomLeft + 1;
			// First triangle
			mIndices.push_back(topLeft);
			mIndices.push_back(bottomLeft);
			mIndices.push_back(topRight);
			// Second triangle
			mIndices.push_back(topRight);
			mIndices.push_back(bottomLeft);
			mIndices.push_back(bottomRight);
		}
	}
}

CollisionObject* TriangleSurface::SurfaceSphereCollision(QVector3D position, float radius)
{
	for (int i = 0; i < mVertices.size(); i += 3)
	{
		Vertex* v1 = &mVertices[i];
		Vertex* v2 = &mVertices[i + 1];
		Vertex* v3 = &mVertices[i + 2];
		// Convert Vertex to QVector3D
		QVector3D p1(v1->x, v1->y, v1->z);
		QVector3D p2(v2->x, v2->y, v2->z);
		QVector3D p3(v3->x, v3->y, v3->z);
		Triangle* tri = new Triangle(p1, p2, p3);
		CollisionObject* collision = tri->TriangleSphereCollision(position, radius);
		if (collision != nullptr)
			return collision;
	}
	return nullptr;
}

void TriangleSurface::CalculateHeights()
{
	for (int i = 0; i < size; i++)
	{
		if (counts[i] > 0)
		{
			heights[i] /= counts[i];
			counts[i] = 1;
		}
	}

	bool changed = true;
	std::vector<float> newHeights = heights;
	std::vector<int> newCounts = counts;

	while (changed)
	{
		changed = false;

		for (int i = 0; i < size; i++)
		{
			if (counts[i] == 0)
			{
				float sum = 0.0f;
				int n = 0;

				if (i - 1 >= 0 && counts[i - 1] > 0) { sum += heights[i - 1]; n++; }
				if (i + 1 < size && counts[i + 1] > 0) { sum += heights[i + 1]; n++; }
				if (i - rowColCount >= 0 && counts[i - rowColCount] > 0) { sum += heights[i - rowColCount]; n++; }
				if (i + rowColCount < size && counts[i + rowColCount] > 0) { sum += heights[i + rowColCount]; n++; }

				if (n > 0)
				{
					newHeights[i] = sum / n;
					newCounts[i] = 1;
					changed = true;
				}
			}
		}

		heights = newHeights;
		counts = newCounts;
		//std::copy(newHeights.begin(), newHeights.begin() + size, heights.begin());
		//std::copy(newCounts.begin(), newCounts.begin() + size, counts.begin());
	}
}
