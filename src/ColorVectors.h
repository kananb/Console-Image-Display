#ifndef GUARD_COLOR_VECTORS_H
#define GUARD_COLOR_VECTORS_H
#pragma once




class cvec3d {

public:

	cvec3d(int red, int green, int blue);
	cvec3d();


	cvec3d operator+(const cvec3d &rhs) const;
	cvec3d operator-(const cvec3d &rhs) const;

	cvec3d operator*(const cvec3d &rhs) const;
	cvec3d operator*(double rhs) const;
	cvec3d operator*(int rhs) const;

	int operator[](size_t index) const;
	int & operator[](size_t index);


	unsigned int distanceTo(const cvec3d &cvec) const;


private:
	int red, green, blue;


};

#endif // GUARD_COLOR_VECTORS_H