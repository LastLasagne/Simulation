#include "rollingball.h"

RollingBall::RollingBall() {}

void RollingBall::Update(float time)
{
	if (isResting)
		return;

	//handle movement
	QVector3D acceleration = QVector3D(0,0,0);	
	bool isAccelerating = false;
	if (contactObject != nullptr)
	{
		//roll down
		QVector3D normal = contactObject->triangle->normal;
		QVector3D NORMAL = (normal * normal.z());

		//QVector3D acceleration = ((QVector3D::dotProduct(GRAVITY, normal)) * normal);
		//float gravityLength = GRAVITY.length();
		//QVector3D accelerating = mass * gravityLength * ((QVector3D(normal.x() * normal.z(), normal.y() * normal.z(), normal.z() * normal.z() - 1)) );
		QVector3D accelerationForce = NORMAL + GRAVITY.normalized();
		isAccelerating = abs(accelerationForce.length()) > 0.0001;

		float frictionValue = NORMAL.length() * FRICTION;
		QVector3D frictionForce = velocity.normalized() * frictionValue;

		acceleration = (accelerationForce - frictionForce) / mass;
		//acceleration = accelerationDirection * (GRAVITY.length() * (accelerationValue - frictionValue));
	}
	else
	{
		//free fall
		acceleration = GRAVITY - (DRAG * velocity);
		isAccelerating = true;
	}

	//stop if not accelerating and friction higher then velocity
	if (isAccelerating == false && velocity.length() < acceleration.length())
	{
		velocity = QVector3D(0, 0, 0);
		isResting = true;
		return;
	}
		
	velocity += acceleration * time;	
	UpdatePosition(time);


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
		velocity = velocity - ((RESTITUTION + 1) * (QVector3D::dotProduct(velocity, collisionObject->triangle->normal)) * collisionObject->triangle->normal);
		//this should happen only if the objects do not seperate after the collision, which is the case when
		if (velocity.length() < 0.01 || QVector3D::dotProduct(velocity, collisionObject->triangle->normal) < 0.01)
		{
			collisionObject->distance = radius;
			contactObject = collisionObject;
		}
		else
			contactObject = nullptr;
	}
	else
	{
		//Wasnt touching, is not touching now as there was no collision
		if (contactObject == nullptr)
			return;

		//was touching, is still above the same triangle
		if (collisionObject->triangle->normal.normalized() == contactObject->triangle->normal.normalized())
			return;

		//was touching, is now above another triangle
			//todo handle concave cases
		contactObject = nullptr;
	}
}

void RollingBall::ResolveCollision(CollisionObject collisionObject)
{
	QVector3D normal = velocity.normalized();
	QVector3D position = getPosition() - normal * (radius - collisionObject.distance);
	setPosition(position);
}