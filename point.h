#ifndef POINT_H
#define POINT_H

#include <ostream>
#include <istream>

struct Point
{
public:
	float x;
	float y;
	float z;
	Point(float x, float y, float z) : x(x), y(y), z(z) {}
	Point() : x(0), y(0), z(0) {}

	//! Overloaded ostream operator which reads all vertex data from an open textfile stream
	friend std::istream& operator>> (std::istream&, Point&);
};

#endif // POINT_H
