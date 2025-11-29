#include "rollingball.h"

RollingBall::RollingBall() { }

void RollingBall::FixedUpdate()
{
	if (isResting)
		return;

	//handle movement
	QVector3D acceleration = QVector3D(0, 0, 0);

	if (contactObject != nullptr)
	{
		//roll down
		QVector3D normal = contactObject->normal;
		//QVector3D NORMAL = (normal * normal.z());

		acceleration = GRAVITY - ((QVector3D::dotProduct(GRAVITY, normal)) * normal);
		//QVector3D accelerating = mass * gravityLength * ((QVector3D(normal.x() * normal.z(), normal.y() * normal.z(), normal.z() * normal.z() - 1)) );
		//QVector3D accelerationForce = NORMAL + GRAVITY.normalized();

		////todo fix friction
		//float frictionValue = normal.length() * FRICTION;
		//QVector3D frictionForce = QVector3D(0, 0, 0); //velocity.normalized()* frictionValue;

		//acceleration = (accelerationForce - frictionForce) / mass;
		//acceleration = accelerationDirection * (GRAVITY.length() * (accelerationValue - frictionValue));
		
		QVector3D tangentVel = velocity - QVector3D::dotProduct(velocity, normal) * normal;
		acceleration -= tangentVel * contactObject->friction;
	}
	else
	{
		//free fall
		acceleration = GRAVITY;//-(DRAG * velocity);
	}

	float accelerationLength = acceleration.length();
	float velocityLength = velocity.length();
	if (accelerationLength < 1.0f && velocityLength < 0.1)
	{
		velocity = QVector3D(0, 0, 0);
		isResting = true;
		return;
	}

	velocity += acceleration * dt;
	UpdatePosition(dt);

	//handle rotation
	//rotate(velocity.length() * time * 180 / 3.14f / 0.1f, QVector3D::crossProduct(normal, velocity));

	ResolveCollision();
}

void RollingBall::Update(float time)
{
	timeAccumulator += time;
	while (timeAccumulator >= dt)
	{
		FixedUpdate();
		timeAccumulator -= dt;
	}
}

bool RollingBall::TryPlace(QVector3D pos)
{
	CollisionObject* collisionObject = surface->GetCollision(pos, radius);
	if (collisionObject != nullptr)
	{
		setPosition(pos);
		return true;
	}
	return false;
}

void RollingBall::ResolveCollision()
{
	CollisionObject* collisionObject = surface->GetCollision(getPosition(), radius);

	if (collisionObject == nullptr)
		return;

	QVector3D normal = -collisionObject->normal.normalized();
	QVector3D position = getPosition() - normal * (radius - collisionObject->distance);
	setPosition(position);

	QVector3D reflectVelocity = ((RESTITUTION + 1) * (QVector3D::dotProduct(velocity, collisionObject->normal)) * collisionObject->normal);
	velocity = velocity - reflectVelocity;

	if (reflectVelocity.length() > velocity.length() || QVector3D::dotProduct(velocity, collisionObject->normal) < 0.01)
	{
		collisionObject->distance = radius;
		contactObject = collisionObject;
	}
	else
		contactObject = nullptr;
}