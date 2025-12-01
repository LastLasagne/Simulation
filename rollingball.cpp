#include "rollingball.h"

RollingBall::RollingBall()
{
	float variance = 0.2f;
	restitutionVariance = -variance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (variance - (-variance))));
	variance = 0.5f;
	frictionVariance = -variance + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (variance - (-variance))));
}

bool RollingBall::FixedUpdate()
{
	if (isResting)
		return false;

	//handle movement
	QVector3D acceleration = QVector3D(0, 0, 0);

	if (contactObject != nullptr)
	{
		//roll down
		QVector3D normal = contactObject->normal;

		acceleration = GRAVITY.length() * ((QVector3D(normal.x() * normal.z(), normal.y() * normal.z(), normal.z() * normal.z() - 1)) );

		QVector3D tangentVel = velocity - QVector3D::dotProduct(velocity, normal) * normal;
		acceleration -= tangentVel * std::max(0.0f, (contactObject->friction + FRICTION + frictionVariance));
	}
	else
	{
		//free fall
		acceleration = GRAVITY;
	}

	float accelerationLength = acceleration.length();
	float velocityLength = velocity.length();
	if (accelerationLength < 1.0f && velocityLength < 0.1)
	{
		velocity = QVector3D(0, 0, 0);
		isResting = true;
		return false;
	}

	velocity += acceleration * dt;
	UpdatePosition(dt);
	ResolveCollisions();
	return surface->IsInGoal(getPosition(), radius);
}

bool RollingBall::Update(float time)
{
	timeAccumulator += time;
	while (timeAccumulator >= dt)
	{
		if (FixedUpdate())
			return true;
		timeAccumulator -= dt;
	}
	return false;
}

bool RollingBall::TryPlace(QVector3D pos)
{
	CollisionObject* collisionObject = surface->SurfaceSphereCollision(pos, radius);
	if (collisionObject != nullptr)
	{
		setPosition(pos);
		return true;
	}
	return false;
}

void RollingBall::ResolveCollisions()
{
	contactObject = nullptr;
	const int maxIterations = 3;
	for (int i = 0; i < maxIterations; ++i)
	{
		CollisionObject* c = surface->GetCollision(getPosition(), radius);
		if (c)
		{
			ResolveCollision(c);
		}
		c = surface->SurfaceSphereCollision(getPosition(), radius);
		if (!c) break;

		ResolveCollision(c);
		contactObject = c;
	}
}

void RollingBall::ResolveCollision(CollisionObject* collision)
{
	if (collision == nullptr)
		return;

	QVector3D normal = collision->normal;
	QVector3D pos = getPosition() + normal * (radius - collision->distance);
	setPosition(pos);
	velocity = velocity - ((std::max(0.0f, RESTITUTION + restitutionVariance) + 1) * (QVector3D::dotProduct(velocity, normal)) * normal);
}