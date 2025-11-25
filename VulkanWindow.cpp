#include "VulkanWindow.h"
#include "Renderer.h"
#include <QKeyEvent>

VulkanWindow::VulkanWindow()
{
}

QVulkanWindowRenderer* VulkanWindow::createRenderer()
{
    //Makes a new instance of the RenderWindow (our Renderer) class
    mRenderer = new Renderer(this, true); // last true == try MSAA

    //Camera is made when the renderer is made, so we know it is valid now
//We set values on the camera a lot from this class, so it is convenient to have a pointer to it
    mCamera = &dynamic_cast<Renderer*>(mRenderer)->mCamera;

    return mRenderer;
}

void VulkanWindow::setCameraSpeed(float value)
{
    mCameraSpeed += value;

    //Keep within some min and max values
    if(mCameraSpeed < 0.01f)
        mCameraSpeed = 0.01f;
    if (mCameraSpeed > 0.3f)
        mCameraSpeed = 0.3f;
}

void VulkanWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_G)
    {
    }
    if(event->key() == Qt::Key_F)
    {
    }
    if (event->key() == Qt::Key_Escape)
    {
        QCoreApplication::quit();       //Shuts down the whole program
    }

    if (event->key() == Qt::Key_0)
        mIndex = 0;
    if (event->key() == Qt::Key_1)
        mIndex = 1;

    //    You get the keyboard input like this
    if(event->key() == Qt::Key_W)
    {
        mInput.W = true;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = true;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = true;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = true;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = true;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = true;
    }
    if(event->key() == Qt::Key_Z)
    {
    }
    if(event->key() == Qt::Key_X)
    {
    }
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = true;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = true;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = true;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = true;
    }
    if(event->key() == Qt::Key_Space)
    {
        mInput.SPACE = true;
    }
}

void VulkanWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        mInput.W = false;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = false;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = false;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = false;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = false;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = false;
    }
    if(event->key() == Qt::Key_Z)
    {
    }
    if(event->key() == Qt::Key_X)
    {
    }
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = false;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = false;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = false;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = false;
    }
    if(event->key() == Qt::Key_Space)
    {
        mInput.SPACE = false;
    }
}

void VulkanWindow::wheelEvent(QWheelEvent *event)
{
    //QWheelEvent gives an x and y value in a QPoint
    //Y == Vertical scroll, X = Horizontal scroll
    QPoint numDegrees = event->angleDelta();

    //if RMB, change the speed of the camera
    if (mInput.RMB)
    {
        if (numDegrees.y() < 1)
            setCameraSpeed(-0.002f);
        if (numDegrees.y() > 1)
            setCameraSpeed(0.002f);
    }
    qDebug("CameraSpeed: %f", mCameraSpeed);
}

void VulkanWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = true;
    if (event->button() == Qt::LeftButton)
        mInput.LMB = true;
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = true;
	mInput.MOUSEX = event->pos().x();
	mInput.MOUSEY = event->pos().y();
}

void VulkanWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = false;
    if (event->button() == Qt::LeftButton)
        mInput.LMB = false;
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = false;
}

void VulkanWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (mInput.RMB)
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;

        mCamera->mYaw += mMouseXlast * mCameraRotateSpeed;
        mCamera->mPitch += mMouseYlast * mCameraRotateSpeed;

    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();

}

void VulkanWindow::handleInput()
{
    //If camera is not set, don't try to update it!
    if (!mCamera)
        return;

    mCamera->resetMovement();  //reset last frame movement

    if (mInput.RMB)
    {
        if (mInput.W)
            mCamera->mCameraMovement.setZ(mCamera->mCameraMovement.z() + mCameraSpeed); //forward
        if (mInput.S)
            mCamera->mCameraMovement.setZ(mCamera->mCameraMovement.z() - mCameraSpeed); //backward
        if (mInput.D)
            mCamera->mCameraMovement.setX(mCamera->mCameraMovement.x() + mCameraSpeed); //right
        if (mInput.A)
            mCamera->mCameraMovement.setX(mCamera->mCameraMovement.x() - mCameraSpeed); //left
        if (mInput.Q)
            mCamera->mCameraMovement.setY(mCamera->mCameraMovement.y() - mCameraSpeed); //down
        if (mInput.E)
            mCamera->mCameraMovement.setY(mCamera->mCameraMovement.y() + mCameraSpeed); //up
    }

    if (mInput.LMB)
    {
		Renderer* renderer = dynamic_cast<Renderer*>(mRenderer);

		float width = QWindow::width();
		float height = QWindow::height();
        float x = (2.0f * mInput.MOUSEX / QWindow::width()) - 1.0f;
        float y = (2.0f * mInput.MOUSEY / QWindow::height()) -1.0f;
		QVector4D clip = QVector4D(x, y, 0.0f, 1.0f);

        //far
		QMatrix4x4 mProjectionMatrix;
        mProjectionMatrix.setToIdentity();
        mProjectionMatrix.perspective(22.5f, width / height, 1.0f, 100.0f);
        mProjectionMatrix = mProjectionMatrix * clipCorrectionMatrix();
		QMatrix4x4 invProj = mProjectionMatrix.inverted();

		QVector4D eye = invProj * clip;
        eye = QVector4D(eye.x(), eye.y(), -1.0f, 0.0);
        QVector4D worldFar = mCamera->viewMatrix().inverted() * eye;


        mProjectionMatrix.setToIdentity();
        mProjectionMatrix.perspective(11.25, width / height, 1.0f, 100.0f);
        mProjectionMatrix = mProjectionMatrix * clipCorrectionMatrix();
        invProj = mProjectionMatrix.inverted();
        eye = invProj * clip;
        eye = QVector4D(eye.x(), eye.y(), -1.0f, 0.0);
        QVector4D worldNear = mCamera->viewMatrix().inverted() * eye;

		QVector3D farDir = (QVector3D(worldFar)).normalized();
		QVector3D nearDir = (QVector3D(worldNear)).normalized();
		QVector3D rayEnd = mCamera->mPosition + 20 * farDir;
		QVector3D rayStart = mCamera->mPosition + 2 * nearDir;

        //float ndc_x = (2.0f * mInput.MOUSEX) / QWindow::width() - 1.0f;
        //float ndc_y = 1.0f - (2.0f * mInput.MOUSEY) / QWindow::height(); // Flip Y
        //QVector4D clipNear = { ndc_x, ndc_y, 0.0f, 1.0f };
        //QVector4D clipFar = { ndc_x, ndc_y, 1.0f, 1.0f };
        //QMatrix4x4 invProj = mCamera->projectionMatrix().inverted();
        //QMatrix4x4 invView = mCamera->viewMatrix().inverted();
        //QVector4D eyeNear = invProj * clipNear;
        //QVector4D eyeFar = invProj * clipFar;
        //if (eyeNear[3] != 0.0f)
        //    eyeNear /= eyeNear.w();

        //if (eyeFar[3] != 0.0f)
        //    eyeFar /= eyeFar.w();
        //QVector3D worldNear = (invView * eyeNear).toVector3D();
        //QVector3D worldFar = (invView * eyeFar).toVector3D();
        //QVector3D dir = (worldFar - worldNear).normalized();

        renderer->SpawnBall(mCamera->mPosition, rayStart, rayEnd);
		mInput.LMB = false;
    }
}