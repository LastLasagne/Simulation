#ifndef SPLINETRACER_H
#define SPLINETRACER_H

#include "VisualObject.h"

class SplineTracer : public VisualObject
{
public:
    SplineTracer(VisualObject* object);
	VisualObject* tracedObject;
    ~SplineTracer(){};

	void Update()
	{
		UpdatePathSamples();
		UpdatePathVertexBuffer();
	}

	void UpdatePathSamples();
	void UpdatePathVertexBuffer();
	QVector3D EvaluateBSpline(int i, float t);

	std::vector<QVector3D> pathSamples; // member of RollingBall or a separate PathTracker
	float sampleDistanceThreshold = 0.05f; // sample if ball moved more than 5 cm
	QVector3D lastSampledPos;
};

#endif // SPLINETRACER_H
