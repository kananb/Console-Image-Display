#include "ColorVectors.h"

#include <exception>




cvec3d::cvec3d(int red, int green, int blue) :
	red(red), green(green), blue(blue) {
}

cvec3d::cvec3d() :
	red(0), green(0), blue(0) {
}



cvec3d cvec3d::operator+(const cvec3d &rhs) const {
	return cvec3d(red + rhs.red, green + rhs.green, blue + rhs.blue);
}

cvec3d cvec3d::operator-(const cvec3d &rhs) const {
	return cvec3d(red - rhs.red, green - rhs.green, blue - rhs.blue);
}


cvec3d cvec3d::operator*(const cvec3d &rhs) const {
	return cvec3d(red * rhs.red, green * rhs.green, blue * rhs.blue);
}

cvec3d cvec3d::operator*(double rhs) const {
	return cvec3d((int)(red * rhs), (int)(green * rhs), (int)(blue * rhs));
}

cvec3d cvec3d::operator*(int rhs) const {
	return cvec3d(red * rhs, green * rhs, blue * rhs);
}



unsigned int cvec3d::distanceTo(const cvec3d &cvec) const {
	return (unsigned int)sqrt(pow(red - cvec.red, 2) + pow(green - cvec.green, 2) + pow(blue - cvec.blue, 2));
}


int cvec3d::operator[](size_t index) const {
	if (index == 0) return red;
	if (index == 1) return green;
	if (index == 2) return blue;
	throw std::exception("Vector index out of range.");
}

int & cvec3d::operator[](size_t index) {
	if (index == 0) return red;
	if (index == 1) return green;
	if (index == 2) return blue;
	throw std::exception("Vector index out of range.");
}
