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

TriangleSurface::TriangleSurface(const std::string& filename, CollisionBox* goal)
{
	collisionBox = goal;
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
		QVector3D color = {0.494, 0.941, 0.4};
		int width = 5;
		if (x > width && y > width && y < rowCount - width && x < colCount - width)
		{
			friction = 10.0f;
			color = QVector3D(0.949f, 0.835f, 0.267f);
		}

		float distance = 0.25f;
		QVector3D goalPos = goal->getPosition();
		float xPos = RESOLUTION * (x);
		float yPos = RESOLUTION * (y);
		if (xPos < goalPos.x() + distance && yPos < goalPos.y() + distance && xPos > goalPos.x() - distance && yPos > goalPos.y() - distance)
		{
			friction = 5.0f;
			color = QVector3D(0.18, 0.369, 0.02);
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

CollisionObject* TriangleSurface::GetCollision(QVector3D position, float radius)
{
	QVector3D normal(0, 0, 0);
	float penetration = 0.0f;

	float minX = 0.0f;
	float minY = 0.0f;
	float maxX = RESOLUTION * (colCount - 1);
	float maxY = RESOLUTION * (rowCount - 1);

	// X penetration
	if (position.x() < minX) {
		penetration = position.x() - minX;
		normal = QVector3D(1, 0, 0);
	}
	else if (position.x() > maxX) {
		penetration = maxX - position.x();
		normal = QVector3D(-1, 0, 0);
	}

	// Y penetration
	else if (position.y() < minY) {
		penetration = position.y() - minY;
		normal = QVector3D(0, 1, 0);
	}
	else if (position.y() > maxY) {
		penetration = maxY - position.y();
		normal = QVector3D(0, -1, 0);
	}

	if (normal != QVector3D(0,0,0))
		return new CollisionObject(normal, penetration, 0.0f);

	return nullptr;
}

bool TriangleSurface::IsInGoal(QVector3D position, float radius)
{
	// --- collision box collision ---
	if (position.x() + radius > collisionBox->boundsMinX &&
		position.x() - radius < collisionBox->boundsMaxX &&
		position.y() + radius > collisionBox->boundsMinY &&
		position.y() - radius < collisionBox->boundsMaxY &&
		position.z() + radius > collisionBox->boundsMinZ &&
		position.z() - radius < collisionBox->boundsMaxZ)
	{
		return true;
	}
	return false;
}

CollisionObject* TriangleSurface::SurfaceSphereCollision(QVector3D position, float radius)
{
	int x = static_cast<int>((position.x() - min.x) / RESOLUTION);
	int y = static_cast<int>((position.y() - min.y) / RESOLUTION);

	if (x < 0 || x > colCount + 1 || y < 0 || y > rowCount + 1)
		return nullptr;

	x = std::min(colCount - 2, x);
	y = std::min(y, rowCount - 2);

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
