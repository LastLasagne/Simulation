#include "oktaederclass.h"

oktaederClass::oktaederClass(): VisualObject()
{
    QVector3D v0{0.f,0.f,1.f};
    QVector3D v1{1.f,0.f,0.f};
    QVector3D v2{0.f,1.f,0.f};
    QVector3D v3{-1.f,0.f,0.f};
    QVector3D v4{0.f,-1.f,0.f};
    QVector3D v5{0.f,0.f,-1.f};
    subDivide(v0,v1,v2,n);
    subDivide(v0,v2,v3,n);
    subDivide(v0,v3,v4,n);
    subDivide(v0,v4,v1,n);
    subDivide(v5,v2,v1,n);
    subDivide(v5,v3,v2,n);
    subDivide(v5,v4,v3,n);
    subDivide(v5,v1,v4,n);
}

void oktaederClass::subDivide(const QVector3D &a, const QVector3D &b, const QVector3D &c, int n)
{
    if(n>0){
        QVector3D v1=a+b;
        v1.normalize();
        QVector3D v2=a+c;
        v2.normalize();
        QVector3D v3=c+b;
        v3.normalize();
        subDivide(a,v1,v2,n-1);
        subDivide(c,v2,v3,n-1);
        subDivide(b,v3,v1,n-1);
        subDivide(v3,v2,v1,n-1);
    }
    else{
        lagTriangel(a,b,c);
        // qDebug()<< "boo";
    }
}

void oktaederClass::lagTriangel(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
{
    Vertex v(v1,v1,{0.f,0.f});
    mVertices.push_back(v);
    v=Vertex{v2,v2,{0.f,1.f}};
    mVertices.push_back(v);
    v=Vertex{v3,v3,{1.f,0.f}};
    mVertices.push_back(v);

    //mMatrix.translate(0,5,0);
}
