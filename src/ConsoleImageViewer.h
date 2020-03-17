#ifndef GUARD_CONSOLE_IMAGE_VIEWER_H
#define GUARD_CONSOLE_IMAGE_VIEWER_H
#pragma once


#include "ConsoleWindow.h"
#include "ConsoleImage.h"



namespace cid {




	const std::vector<cv::Vec3b> COLORS_GREYSCALE{
		cv::Vec3b(0, 0, 0), cv::Vec3b(17, 17, 17), cv::Vec3b(34, 34, 34), cv::Vec3b(51, 51, 51),
		cv::Vec3b(68, 68, 68), cv::Vec3b(85, 85, 85), cv::Vec3b(102, 102, 102), cv::Vec3b(119, 119, 119),
		cv::Vec3b(136, 136, 136), cv::Vec3b(153, 153, 153), cv::Vec3b(170, 170, 170), cv::Vec3b(187, 187, 187),
		cv::Vec3b(204, 204, 204), cv::Vec3b(221, 221, 221), cv::Vec3b(238, 238, 238), cv::Vec3b(255, 255, 255)
	};
	const std::vector<cv::Vec3b> COLORS_DEFAULT{
		cv::Vec3b(0, 0, 0), cv::Vec3b(0, 0, 128), cv::Vec3b(0, 128, 0), cv::Vec3b(0, 128, 128),
		cv::Vec3b(128, 0, 0), cv::Vec3b(128, 0, 128), cv::Vec3b(128, 128, 0), cv::Vec3b(192, 192, 192),
		cv::Vec3b(128, 128, 128), cv::Vec3b(0, 0, 255), cv::Vec3b(0, 255, 0), cv::Vec3b(0, 255, 255),
		cv::Vec3b(255, 0, 0), cv::Vec3b(255, 0, 255), cv::Vec3b(255, 255, 0), cv::Vec3b(255, 255, 255)
	};
	const std::vector<cv::Vec3b> COLORS_BLACK_AND_WHITE{
		cv::Vec3b(0, 0, 0), cv::Vec3b(255, 255, 255)
	};


	enum ImageFlags : unsigned int {
		IMAGETYPE_PIXEL					= 0x00000000, // displays image using ASCII char 219
		IMAGETYPE_ASCII					= 0x00000001, // displays image using 70 different ascii characters
		IMAGETYPE_ASCII_SMALL			= 0x00000003, // displays image using 10 different ascii characters

		ASCIIMODE_PREPROCESS			= 0x00000000, // ascii characters are gotten from the original image
		ASCIIMODE_POSTPROCESS			= 0x00000100, // ascii characters are gotten after the coloring algorithm is applied

		COLORMODE_DITHER_ATKINSON		= 0x00000000, // reduces image's color range by applying the atkinson dithering algoirthm
		COLORMODE_DITHER_FLOYDSTEINBERG	= 0x00010000, // reduces image's color range by applying the floyd-steinberg dithering algorithm
		COLORMODE_NEAREST_VALUE			= 0x00030000  // reduces image's color range by nearest value/neighbor
	};
	enum ImageFlagMasks : unsigned int {
		DCFGMASK_IMAGETYPE = 0x000000FF,
		DCFGMASK_ASCIIMODE = 0x0000FF00,
		DCFGMASK_COLORMODE = 0x00FF0000,
	};



	class DisplayConfig {

	public:

		DisplayConfig(unsigned int flags, const std::vector<cv::Vec3b> &imageColors, const std::vector<cv::Vec3b> &textColors);
		DisplayConfig(unsigned int flags, const std::vector<cv::Vec3b> &colors);



		unsigned int flags;
		std::vector<cv::Vec3b> imageColors, textColors;
	};




	class ConsoleImageViewer {

	public:


		ConsoleImageViewer(const std::string &imagePath);
		~ConsoleImageViewer();


		void display(DisplayConfig displayMode, short fontSize = 1, float scaleFactor = 1);


		const ConsoleImage & getImage() const;


	private:

		void makeDarkestFirst(std::vector<cv::Vec3b> &colors);

		void resizeImage(short fontSize, float scaleFactor);


	private:

		ConsoleWindow *window;
		ConsoleImage image, originalImage;


	};




};

#endif // GUARD_CONSOLE_IMAGE_VIEWER_H