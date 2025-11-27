#include "rollingball.h"

RollingBall::RollingBall() { }

void RollingBall::FixedUpdate()
{
	if (isResting)
		return;

	//handle movement
	QVector3D acceleration = QVector3D(0, 0, 0);
	bool isAccelerating = false;
	if (contactObject != nullptr)
	{
		//roll down
		QVector3D normal = contactObject->normal;
		//QVector3D NORMAL = (normal * normal.z());

		acceleration = GRAVITY - ((QVector3D::dotProduct(GRAVITY, normal)) * normal);
		//float gravityLength = GRAVITY.length();
		//QVector3D accelerating = mass * gravityLength * ((QVector3D(normal.x() * normal.z(), normal.y() * normal.z(), normal.z() * normal.z() - 1)) );
		isAccelerating = abs(acceleration.length()) > 0.005;
		//QVector3D accelerationForce = NORMAL + GRAVITY.normalized();

		////todo fix friction
		//float frictionValue = normal.length() * FRICTION;
		//QVector3D frictionForce = QVector3D(0, 0, 0); //velocity.normalized()* frictionValue;

		//acceleration = (accelerationForce - frictionForce) / mass;
		//acceleration = accelerationDirection * (GRAVITY.length() * (accelerationValue - frictionValue));

		QVector3D tangentVel = velocity - QVector3D::dotProduct(velocity, normal) * normal;
		acceleration -= tangentVel * FRICTION;
	}
	else
	{
		//free fall
		acceleration = GRAVITY;//-(DRAG * velocity);
		isAccelerating = true;
	}

	//stop if not accelerating and friction higher then velocity
	if (isAccelerating == false && velocity.length() < acceleration.length())
	{
		velocity = QVector3D(0, 0, 0);
		isResting = true;
		return;
	}

	velocity += acceleration * dt;
	UpdatePosition(dt);

	//handle rotation
	//rotate(velocity.length() * time * 180 / 3.14f / 0.1f, QVector3D::crossProduct(normal, velocity));

	//check collision
	CollisionObject* collisionObject = surface->SurfaceSphereCollision(getPosition(), radius);

	//not above any triangle
	if (collisionObject == nullptr)
	{
		contactObject = nullptr;
		return;
	}

	if (collisionObject->isColliding)
	{
		//handle collision
		ResolveCollision(*collisionObject);
		velocity = velocity - ((RESTITUTION + 1) * (QVector3D::dotProduct(velocity, collisionObject->normal)) * collisionObject->normal);

		if (velocity.length() < 0.01 || QVector3D::dotProduct(velocity, collisionObject->normal) < 0.01)
		{
			collisionObject->distance = radius;
			contactObject = collisionObject;
		}
		else
			contactObject = nullptr;
	}
	else
	{

	}
	{
		//Wasnt touching, is not touching now as there was no collision
		if (contactObject == nullptr)
			return;

		//was touching, is still above the same triangle
		if (collisionObject->normal.normalized() == contactObject->normal.normalized())
			return;

		//was touching, is now above another triangle
			//todo handle concave cases
		contactObject = nullptr;
	}
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
	CollisionObject* collisionObject = surface->SurfaceSphereCollision(pos, radius);
	if (collisionObject != nullptr && collisionObject->isColliding)
	{
		setPosition(pos);
		return true;
	}
	return false;
}

void RollingBall::ResolveCollision(CollisionObject collisionObject)
{
	QVector3D normal = velocity.normalized();
	QVector3D position = getPosition() - normal * (radius - collisionObject.distance);
	setPosition(position);
}