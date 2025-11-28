#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "TriangleSurface.h"
#include "oktaederclass.h"
#include "Collisionobject.h"

class RollingBall : public oktaederClass
{
public:
	RollingBall();

	void FixedUpdate();
    void Update(float time);

	bool TryPlace(QVector3D pos);
	void UpdatePosition(float time) { setPosition(getPosition() + velocity * time); }
	void ResolveCollision(CollisionObject collisionObject);
	void scale(float s) {
		radius *= s;
		oktaederClass::scale(s);
	}
	const QVector3D GRAVITY = QVector3D(0, 0, -9.81);
	const float RESTITUTION = 0.0f; //Coefficient of restitution
	const float FRICTION = 0.0f; //Coefficient of friction
	const float DRAG = 0.05f; //Coefficient of air resistance
	QVector3D velocity = QVector3D(0, 0, 0);
    
	CollisionObject* contactObject = nullptr;
	TriangleSurface* surface;
	float radius = 1.0f;
	float mass = 10.0f;

	bool isResting = false;

	float timeAccumulator = 0.0f;
	const float dt = 1.0 / 30.0; // fixed physics step
};

#endif // ROLLINGBALL_H
