#include "Camera.h"

Camera::Camera() 
{
	init();
}

void Camera::init()
{
    mProjectionMatrix.setToIdentity();
    mViewMatrix.setToIdentity();
}
void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(degrees, aspect, nearplane, farplane);

    //Flip projection because of Vulkan's -Y axis
	// Now done with Qts clipCorrectionMatrix() which is more correct than this hack.
	// It is corrected in Renderer::setViewProjectionMatrix(), just before it is pushed to the shader.
    // mProjectionMatrix.scale(1.0f, -1.0f, 1.0f);
}

void Camera::update()
{
    // ***** Rotation ***** :
    // no roll, since that is not needed for now

    mForward = rotateX(QVector3D{ 0, 0, -1 }, mPitch);
    //rotateY rotates around world up, so this does not roll the camera:
    mForward = rotateY(mForward, mYaw);


    // ***** Movement ***** :

    // move camera along the forward vector:
    mPosition += mForward * mCameraMovement.z();

    // the local right vector of the camera:
    mRight = rotateY(QVector3D{ 1, 0, 0 }, mYaw);
    // move camera along the right vector:
    mPosition += mRight * mCameraMovement.x();

    // move camera along worldUp vector
    mPosition += mUp * mCameraMovement.y();

    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(mPosition, mPosition + mForward, mUp);
}