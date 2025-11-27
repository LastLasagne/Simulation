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

//TriangleSurface::TriangleSurface(bool toggle, const std::string &filename)
//{
//    std::ifstream inn(filename);
//    if (!inn.is_open())
//	{
//		qDebug() << "Failed to open file: " << QString::fromStdString(filename);
//		return;
//	}
//	// read input from math part of compulsory
//    int n;  
//    Vertex v;
//    inn >> n;
//
//	float minX = FLT_MAX;
//	float minY = FLT_MAX;
//	float maxX = -FLT_MAX;
//	float maxY = -FLT_MAX;
//
//    for (auto i=0; i<n; i++)
//    {
//        inn >> v;
//		qDebug() << "Read vertex: " << v.x << v.y << v.z << v.r << v.g << v.b << v.u << v.v;
//        mVertices.push_back(v);
//		if (v.x < minX)
//		{
//			min.x = v.x;
//			minX = v.x;
//		}
//		if (v.y < minY)
//		{
//			min.y = v.y;
//			minY = v.y;
//		}
//		if (v.x > maxX)
//		{
//			max.x = v.x;
//			maxX = v.x;
//		}
//		if (v.y > maxY)
//		{
//			max.y = v.y;
//			maxY = v.y;
//		}
//        //qDebug() << v.x << v.y << v.z;
//    }
//    inn.close();
//	mIndices = std::vector<uint32_t>();
//}

TriangleSurface::TriangleSurface(const std::string& filename)
{
	drawType = 1;
	std::ifstream inn(filename);
	if (!inn.is_open())
	{
		qDebug() << "Failed to open file: " << QString::fromStdString(filename);
		return;
	}
	Point v;

	int n;
	inn >> n;

	inn >> min;
	inn >> max;

	for (auto i = 0; i < n; i++)
	{
		inn >> v;
		points.push_back(v);
	}
	inn.close();

	colCount = static_cast<int>((max.x - min.x) / RESOLUTION);
	rowCount = static_cast<int>((max.y - min.y) / RESOLUTION);
	size = rowCount * colCount;

	heights.resize(size, 0);
	counts.resize(size, 0);
	for (int i = 0; i < points.size(); i++)
	{
		Point p = points[i];

		float xIndex = static_cast<int>((p.x - min.x) / RESOLUTION);
		float yIndex = static_cast<int>((p.y - min.y) / RESOLUTION);

		//fix so no stretching occurs on the outsides

		if (xIndex >= colCount || yIndex >= rowCount)
			continue;
		int index = yIndex * colCount + xIndex;
		heights[index] += p.z;
		counts[index] += 1;
	}

	CalculateHeights();

	for (int i = 0; i < size; i++)
	{
		int x = i % colCount;
		int y = i / colCount;

		float center = SampleHeight(x, y);
		float left = SampleHeight(x - 1, y);
		float right = SampleHeight(x + 1, y);
		float down = SampleHeight(x, y - 1);
		float up = SampleHeight(x, y + 1);

		QVector3D dx(2.0f * RESOLUTION, 0, right - left);
		QVector3D dy(0, 2.0f * RESOLUTION, up - down);

		// normal = cross(dy, dx) (order matters)
		
		QVector3D normal = QVector3D::crossProduct(dy, dx).normalized();

		mVertices.push_back(Vertex(
			x * RESOLUTION,
			y * RESOLUTION,
			center,
			normal.x(), normal.y(), normal.z(),
			0.0f, 0.0f));
	}

	for (int y = 0; y < rowCount - 1; y++)
	{
		for (int x = 0; x < colCount - 1; x++)
		{
			int topLeft = y * colCount + x;
			int topRight = topLeft + 1;
			int bottomLeft = (y + 1) * colCount + x;
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
	if (mIndices.size() == 0)
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
	}

	for (int i = 0; i < mIndices.size(); i += 3)
	{
		Vertex* v1 = &mVertices[mIndices[i]];
		Vertex* v2 = &mVertices[mIndices[i + 1]];
		Vertex* v3 = &mVertices[mIndices[i + 2]];
		// Convert Vertex to QVector3D
		QVector3D p1(v1->x, v1->y, v1->z);
		QVector3D p2(v2->x, v2->y, v2->z);
		QVector3D p3(v3->x, v3->y, v3->z);
		Triangle* tri = new Triangle(p1, p2, p3);
		CollisionObject* collision = tri->TriangleSphereCollision(position, radius);
		if (collision != nullptr)
			return collision;
	}

	float distance = 0.0f;
	QVector3D normal = QVector3D(0,0,0);

	if (position.x() < min.x)
	{
		distance += (min.x - position.x()) * (min.x - position.x());
		normal += QVector3D(1, 0, 0);
	}
	else if (position.x() > max.x)
	{
		distance += (position.x() - max.x) * (position.x() - max.x);
		normal += QVector3D(-1, 0, 0);
	}

	if (position.y() < min.y)
	{
		distance += (min.y - position.y()) * (min.y - position.y());
		normal += QVector3D(0, 1, 0);
	}
	else if (position.y() > max.y)
	{
		distance += (position.y() - max.y) * (position.y() - max.y);
		normal += QVector3D(0, -1, 0);
	}

	CollisionObject* boundsCollision = new CollisionObject(true, normal, distance);

	return boundsCollision;
}

float TriangleSurface::SampleHeight(int x, int y)
{
	x = std::max(0, std::min(colCount - 1, x));
	y = std::max(0, std::min(rowCount - 1, y));

	int idx = x + y * colCount;
	return (heights[idx] - min.z) * HEIGHT_SCALE;
};

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
				if (i - colCount >= 0 && counts[i - colCount] > 0) { sum += heights[i - colCount]; n++; }
				if (i + colCount < size && counts[i + colCount] > 0) { sum += heights[i + colCount]; n++; }

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
