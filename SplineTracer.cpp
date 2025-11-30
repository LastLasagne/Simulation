#include "SplineTracer.h"

SplineTracer::SplineTracer(VisualObject* object)
{
	tracedObject = object;
	drawType = 1; // 0 = fill, 1 = line
	// Initial samples
	pathSamples.push_back(tracedObject->getPosition());
	pathSamples.push_back(tracedObject->getPosition());
	pathSamples.push_back(tracedObject->getPosition());
	pathSamples.push_back(tracedObject->getPosition());
}

void SplineTracer::UpdatePathSamples()
{
	QVector3D currentPos = tracedObject->getPosition();

	if (pathSamples.empty() || (currentPos - lastSampledPos).length() > sampleDistanceThreshold)
	{
		pathSamples.push_back(currentPos);
		lastSampledPos = currentPos;
	}
}

void SplineTracer::UpdatePathVertexBuffer()
{
	if (pathSamples.size() <= 4) return;

	std::vector<Vertex> vertices;
	const int segments = 50; // points along spline
	int n = pathSamples.size();

	for (int s = 0; s < segments; ++s)
	{
		float u = float(s) / (segments - 1);
		float segF = u * (n - 3);
		int i = int(segF);
		float t = segF - i;
		QVector3D p = EvaluateBSpline(i, t);
		vertices.push_back(Vertex(p, QVector3D(0, 0, 0), QVector3D(0, 0, 0)));
	}

	mVertices = vertices;
}

QVector3D SplineTracer::EvaluateBSpline(int i, float t)
{
	int n = pathSamples.size();
	if (n < 4) return QVector3D();

	if (i < 0) i = 0;
	if (i > n - 4) i = n - 4;

	const QVector3D& P0 = pathSamples[i];
	const QVector3D& P1 = pathSamples[i + 1];
	const QVector3D& P2 = pathSamples[i + 2];
	const QVector3D& P3 = pathSamples[i + 3];

	float B0 = (1 - t) * (1 - t) * (1 - t) / 6.0f;
	float B1 = (3 * t * t * t - 6 * t * t + 4) / 6.0f;
	float B2 = (-3 * t * t * t + 3 * t * t + 3 * t + 1) / 6.0f;
	float B3 = t * t * t / 6.0f;

	return P0 * B0 + P1 * B1 + P2 * B2 + P3 * B3;
}