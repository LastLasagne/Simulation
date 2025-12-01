#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "TriangleSurface.h"
#include "oktaederclass.h"
#include "Collisionobject.h"

class RollingBall : public oktaederClass
{
public:
	RollingBall();

	bool FixedUpdate();
    bool Update(float time);

	bool TryPlace(QVector3D pos);
	void UpdatePosition(float time) { setPosition(getPosition() + velocity * time); }
	void ResolveCollisions();
	void ResolveCollision(CollisionObject* collision);
	void scale(float s) {
		radius *= s;
		oktaederClass::scale(s);
	}

	const QVector3D GRAVITY = QVector3D(0, 0, -9.81);
	const float RESTITUTION = 0.25f; //Coefficient of restitution
	const float FRICTION = 0.5f; //Coefficient of friction
	float restitutionVariance = 0.0f;
	float frictionVariance = 0.0f;

	QVector3D velocity = QVector3D(0, 0, 0);
    
	TriangleSurface* surface;
	CollisionObject* contactObject = nullptr;
	float radius = 1.0f;

	bool isResting = false;

	float timeAccumulator = 0.0f;
	const float dt = 1.0 / 30.0; // fixed physics step
};

#endif // ROLLINGBALL_H
