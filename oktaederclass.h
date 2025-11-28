#ifndef OKTAEDERCLASS_H
#define OKTAEDERCLASS_H
#include "VisualObject.h"
#include "Vertex.h"

class oktaederClass: public VisualObject
{
public:
    oktaederClass();
    int n = 1;
    //int n=24*pow(4,n);
    void subDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n);
    void lagTriangel(const QVector3D& a, const QVector3D& b, const QVector3D& c);
};

#endif // OKTAEDERCLASS_H
