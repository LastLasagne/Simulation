#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "TriangleSurface.h"
#include "oktaederclass.h"
#include "Collisionobject.h"

class RollingBall : public oktaederClass
{
public:
    RollingBall();
    void Update(TriangleSurface* surface, float time);
	void UpdatePosition(float time) { setPosition(getPosition() + velocity * time); }
	void ResolveCollision(CollisionObject collisionObject);
	void scale(float s) {
		radius *= s;
		oktaederClass::scale(s);
	}
	const QVector3D GRAVITY = QVector3D(0, 0, -9.81);
	const float RESTITUTION = 0.4f; //Coefficient of restitution
	const float FRICTION = 0.3f; //Coefficient of friction
	const float DRAG = 0.05f; //Coefficient of air resistance
	QVector3D velocity = QVector3D(0, 0, 0);
    
	CollisionObject* contactObject = nullptr;

	float radius = 1.0f;
	float mass = 10.0f;

	bool isResting = false;
};

#endif // ROLLINGBALL_H
