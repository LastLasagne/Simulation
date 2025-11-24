#ifndef CAMERA_H
#define CAMERA_H
#include <QMatrix4x4>

class Camera
{
public:
    Camera();
    ~Camera() {}

    void init();
    void perspective(int degrees, double aspect, double nearplane, double farplane);

	inline QMatrix4x4 viewMatrix() const { return mViewMatrix; }
	inline QMatrix4x4 projectionMatrix() const { return mProjectionMatrix; }

	void resetMovement() { mCameraMovement = QVector3D(0, 0, 0); }  
    void update();

    inline void setProjectionMatrix(const QMatrix4x4 &newProjectionMatrix){ mProjectionMatrix = newProjectionMatrix; }

    QMatrix4x4 mProjectionMatrix{};
    QMatrix4x4 mViewMatrix{};

    QVector3D mPosition{ 0.f, 0.f, 0.f };

    QVector3D mForward{ 0, 0, -1 };  // Forward vector local to the camera
    QVector3D mUp{ 0, 1, 0 };        // Using world up
    QVector3D mRight{ 1, 0, 0 };     // Right vector local to the camera

    float mPitch{ 0.f };     // The pitch of the camera - in Euler angle
    float mYaw{ 0.f };       // The yaw of the camera - in Euler angle

    // How much should the camera move next frame in cameraForward coordinates
    QVector3D mCameraMovement{ 0, 0, 0 };


    // ******* Utility functions - should be in some common place so we can reuse it in all of our code:

    // Rotates vector v by angleEuler degrees around the selected world axis
    QVector3D rotateX(const QVector3D& v, float angleEuler)
    {
        float angleRad = angleEuler * M_PI / 180;
        float c = cos(angleRad);
        float s = sin(angleRad);
        return QVector3D
        (
            v.x(),
			v.y() * c - s * v.z(),
			v.y() * s + c * v.z()
        );
    }

    // Rotates vector v by angleEuler degrees around the selected world axis
    QVector3D rotateY(const QVector3D& v, float angleEuler)
    {
        float angleRad = angleEuler * M_PI / 180;
        float c = cos(angleRad);
        float s = sin(angleRad);
        return QVector3D
        (
            c * v.x() + s * v.z(),
            v.y(),
            -s * v.x() + c * v.z()
        );
    }

    // Rotates vector v by angleEuler degrees around the selected world axis
    QVector3D rotateZ(const QVector3D& v, float angleEuler)
    {
        float angleRad = angleEuler * M_PI / 180;
        float c = cos(angleRad);
        float s = sin(angleRad);
        return QVector3D
        (
            c * v.x() - s * v.y(),
            s * v.x() + c * v.y(),
            v.z()
        );
    }
};

#endif // CAMERA_H
