#ifndef GUARD_IMAGE_TO_ASCII_H
#define GUARD_IMAGE_TO_ASCII_H
#pragma once



namespace cid {




	class ValToAscii {

		const static unsigned char asciiFull[70];
		const static unsigned char asciiShort[10];

	public:

		enum AsciiSet : unsigned char {
			SET_FULL, SET_SHORT
		};

		static unsigned char convert(unsigned char intensity, AsciiSet set = SET_FULL);
	};




};

#endif // GUARD_IMAGE_TO_ASCII_H