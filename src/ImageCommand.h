#ifndef GUARD_IMAGE_COMMAND_H
#define GUARD_IMAGE_COMMAND_H
#pragma once


#include <string>



namespace cid {




	enum ColorOption {
		COLOROPTION_GREY, COLOROPTION_DEFAULT, COLOROPTION_BLACK_AND_WHITE, COLOROPTION_FILE, COLOROPTION_APPROXIMATE
	};



	struct ImageCommand {
	
		std::string imagePath;
		short fontSize;
		float scaleFactor;

		unsigned int flags;
		ColorOption imageColorOption, textColorOption;
		int numApproximateImageColors, numApproximateTextColors;
		std::string imageColorFile, textColorFile;

		
	};




}

#endif // GUARD_IMAGE_COMMAND_H