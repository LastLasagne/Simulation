#include "VisualObject.h"

VisualObject::VisualObject()
{
    mMatrix.setToIdentity();
}

void VisualObject::move(float x, float y, float z)
{
    mMatrix.translate(x, y, z);
}

void VisualObject::scale(float s)
{
    mMatrix.scale(s);
}

void VisualObject::rotate(float t, float x, float y, float z)
{
    mMatrix.rotate(t, x, y, z);
}

void VisualObject::rotate(float t, QVector3D xyz)
{
    mMatrix.rotate(t, xyz.x(), xyz.y(), xyz.z());
}

QVector3D VisualObject::getPosition()
{
    float x = mMatrix(0, 3); // Position in the x-axis
    float y = mMatrix(1, 3); // Position in the y-axis
    float z = mMatrix(2, 3); // Position in the z-axis
    return QVector3D(x, y, z);
}

void VisualObject::setPosition(float x, float y, float z)
{
    mMatrix(0, 3) = x; // Position in the x-axis
    mMatrix(1, 3) = y; // Position in the y-axis
    mMatrix(2, 3) = z; // Position in the z-axis
}

void VisualObject::setPosition(QVector3D pos)
{
	mMatrix(0, 3) = pos.x(); // Position in the x-axis
	mMatrix(1, 3) = pos.y(); // Position in the y-axis
	mMatrix(2, 3) = pos.z(); // Position in the z-axis
}
