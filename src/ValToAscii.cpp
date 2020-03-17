#include "ValToAscii.h"

using namespace cid;




const unsigned char ValToAscii::asciiFull[70] = {
	' ', '.', '\'', '`', '^', '"', ',', ':', ';', 'I',
	'l', '!', 'i', '>', '<', '~', '+', '_', '-', '?',
	']', '[', '}', '{', '1', ')', '(', '|', '\\', '/',
	't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z',
	'X', 'Y', 'U', 'J', 'C', 'L', 'Q', '0', 'O', 'Z',
	'm', 'w', 'q', 'p', 'd', 'b', 'k', 'h', 'a', 'o',
	'*', '#', 'M', 'W', '&', '8', '%', 'B', '@', '$'
};

const unsigned char ValToAscii::asciiShort[10] = {
	' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'
};




unsigned char ValToAscii::convert(unsigned char intensity, AsciiSet set) {
	return (set == SET_FULL) ?
		asciiFull[(unsigned char)(intensity * (70 / 256.))] :
		asciiShort[(unsigned char)(intensity * (10 / 256.))];
}