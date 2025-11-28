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

		int xIndex = static_cast<int>((p.x - min.x) / RESOLUTION);
		int yIndex = static_cast<int>((p.y - min.y) / RESOLUTION);

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

		float left = SampleHeight(x - 1, y);
		float right = SampleHeight(x + 1, y);
		float down = SampleHeight(x, y - 1);
		float up = SampleHeight(x, y + 1);

		QVector3D dx(2.0f * RESOLUTION, 0, right - left);
		QVector3D dy(0, 2.0f * RESOLUTION, up - down);

		// normal = cross(dy, dx) (order matters)
		
		QVector3D normal = QVector3D::crossProduct(dy, dx).normalized();

		float center = SampleHeight(x, y);

		float friction = 0.1f;
		QVector3D color = { 0.5f, 0.25f, 0.0f };
		float width = 5.0f;
		if (x > width && y > width && y < rowCount - width && x < colCount - width)
		{
			friction = 3.0f;
			color = { 0.0f, 1.0f, 0.0f };
		}

		Vertex vertex = Vertex(
			x * RESOLUTION,
			y * RESOLUTION,
			center,
			normal.x(), normal.y(), normal.z(),
			color.x(), color.y(), color.z());
		vertex.friction = friction;
		mVertices.push_back(vertex);
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
	int x = static_cast<int>((position.x() - min.x) / RESOLUTION);
	int y = static_cast<int>((position.y() - min.y) / RESOLUTION);

	if (x < 0 || x > colCount + 1 || y < 0 || y > rowCount + 1)
	{
		return nullptr;
		//float distance = 0.0f;
		//QVector3D normal = QVector3D(0, 0, 0);

		//if (position.x() < min.x)
		//{
		//	distance += (min.x - position.x()) * (min.x - position.x());
		//	normal += QVector3D(1, 0, 0);
		//}
		//else if (position.x() > max.x)
		//{
		//	distance += (position.x() - max.x) * (position.x() - max.x);
		//	normal += QVector3D(-1, 0, 0);
		//}

		//if (position.y() < min.y)
		//{
		//	distance += (min.y - position.y()) * (min.y - position.y());
		//	normal += QVector3D(0, 1, 0);
		//}
		//else if (position.y() > max.y)
		//{
		//	distance += (position.y() - max.y) * (position.y() - max.y);
		//	normal += QVector3D(0, -1, 0);
		//}

		//CollisionObject* boundsCollision = new CollisionObject(true, normal, distance, 0.0f);

		//return boundsCollision;
	}

	x = std::min(colCount - 2, x);
	y = std::min(y, rowCount - 2);

	//if (mIndices.size() == 0)
	//{
	//	for (int i = 0; i < mVertices.size(); i += 3)
	//	{
	//		Vertex* v1 = &mVertices[i];
	//		Vertex* v2 = &mVertices[i + 1];
	//		Vertex* v3 = &mVertices[i + 2];
	//		Triangle* tri = new Triangle(v1, v2, v3);
	//		CollisionObject* collision = tri->TriangleSphereCollision(position, radius);
	//		if (collision != nullptr)
	//			return collision;
	//	}
	//}

	int index = (y * (colCount - 1) + x) * 6;

	Vertex* tri1v1 = &mVertices[mIndices[index]];
	Vertex* tri1v2 = &mVertices[mIndices[index + 1]];
	Vertex* tri1v3 = &mVertices[mIndices[index + 2]];
	Triangle* tri1 = new Triangle(tri1v1, tri1v2, tri1v3);
	CollisionObject* collision1 = tri1->TriangleSphereCollision(position, radius);
	if (collision1 != nullptr)
		return collision1;

	Vertex* tri2v1 = &mVertices[mIndices[index + 3]];
	Vertex* tri2v2 = &mVertices[mIndices[index + 4]];
	Vertex* tri2v3 = &mVertices[mIndices[index + 5]];
	Triangle* tri2 = new Triangle(tri2v1, tri2v2, tri2v3);
	CollisionObject* collision2 = tri2->TriangleSphereCollision(position, radius);
	if (collision2 != nullptr)
		return collision2;
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
