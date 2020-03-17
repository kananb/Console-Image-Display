#include "ConsoleImageViewer.h"
#include "ValToAscii.h"

using namespace cid;




DisplayConfig::DisplayConfig(unsigned int flags, const std::vector<cv::Vec3b> &imageColors, const std::vector<cv::Vec3b> &textColors) :
	flags(flags), imageColors(imageColors), textColors(textColors) {
}

DisplayConfig::DisplayConfig(unsigned int flags, const std::vector<cv::Vec3b> &colors) :
	DisplayConfig(flags, colors, colors) {
}





ConsoleImageViewer::ConsoleImageViewer(const std::string &imagePath) :
	window(nullptr),
	image(), originalImage(imagePath) {
}

ConsoleImageViewer::~ConsoleImageViewer() {
	if (window != nullptr) delete window;
}



void ConsoleImageViewer::display(DisplayConfig config, short fontSize, float scaleFactor) {
	image = originalImage;
	if (config.textColors.size() > 16) config.textColors.resize(16);
	// make the darkest color the first color in the vector (makes ascii images look better)
	makeDarkestFirst(config.textColors);

	auto lfCheckFlag = [config](unsigned int mask, unsigned int flag) {
		return (config.flags & mask) == flag;
	};

	if (window == nullptr)
		window = new ConsoleWindow("Image", (short)image.getWidth(), (short)image.getHeight(), fontSize);
	else if (window->getFontSize() != fontSize)
		window->setFont(window->getFontName(), fontSize);
	/* RESIZE IMAGE AND BUFFER */
	resizeImage(fontSize, scaleFactor);
	window->setSize(image.getWidth(), image.getHeight());
	// used to access colors from the original image using coordinates for the processed image
	double xRatio = (double)originalImage.getWidth() / image.getWidth(), yRatio = (double)originalImage.getHeight() / image.getHeight();


	/* IMAGE DUPLICATION AND COLORING ALGORITHMS */
	// if the image is single-color, preprocess ascii, then there is no need to copy the original image
	// or apply the coloring algorithm
	if (lfCheckFlag(DCFGMASK_IMAGETYPE, IMAGETYPE_PIXEL) ||
		lfCheckFlag(DCFGMASK_ASCIIMODE, ASCIIMODE_POSTPROCESS) || config.textColors.size() > 1) {

		// apply the desired coloring effect
		if (lfCheckFlag(DCFGMASK_COLORMODE, COLORMODE_DITHER_ATKINSON))
			image.ditherAtkinson(config.imageColors);
		else if (lfCheckFlag(DCFGMASK_COLORMODE, COLORMODE_DITHER_FLOYDSTEINBERG))
			image.ditherFloydSteinberg(config.imageColors);
		else
			image.nearestValue(config.imageColors);
	}


	/* FILL WINDOW BUFFER */
	if (lfCheckFlag(DCFGMASK_IMAGETYPE, IMAGETYPE_PIXEL)) {
		CHAR_INFO cinfo = { 0 };
		cinfo.Char.AsciiChar = (char)219;

		for (int y = 0; y < window->getHeight(); ++y) {
			for (int x = 0; x < window->getWidth(); ++x) {
				// select and assign text colors
				size_t colorIndex = ConsoleImage::FindClosestPalletteColorIndex(image.index(x, y), config.textColors);

				cinfo.Attributes = (WORD)((config.textColors.size() < 16) ? 15 - colorIndex : colorIndex);
				window->setBufferElement(y, x, cinfo);
			}
		}
	}
	else {
		CHAR_INFO cinfo = { 0 };
		for (int y = 0; y < window->getHeight(); ++y) {
			for (int x = 0; x < window->getWidth(); ++x) {
				// convert pixel intensity into ascii character
				cv::Vec3b vecColor = (lfCheckFlag(DCFGMASK_ASCIIMODE, ASCIIMODE_PREPROCESS) && config.textColors.size() > 1) ?
					originalImage.index((int)(x * xRatio), (int)(y * yRatio)) : image.index(x, y);
				cinfo.Char.AsciiChar = ValToAscii::convert((vecColor[2] + vecColor[1] + vecColor[0]) / 3);

				// select and assign text colors
				size_t colorIndex = ConsoleImage::FindClosestPalletteColorIndex(image.index(x, y), config.textColors);

				cinfo.Attributes = (WORD)((config.textColors.size() < 16) ? 15 - colorIndex : colorIndex);
				window->setBufferElement(y, x, cinfo);
			}
		}
	}


	/* UPDATE COLOR TABLE */
	for (size_t i = 0; i < config.textColors.size(); ++i) {
		window->setColorTableValue(
			(config.textColors.size() < 16) ? 15 - i : i,
			RGB(config.textColors[i][0], config.textColors[i][1], config.textColors[i][2])
		);
	}

	window->updateBuffer();
}



const ConsoleImage & ConsoleImageViewer::getImage() const {
	return originalImage;
}




void ConsoleImageViewer::makeDarkestFirst(std::vector<cv::Vec3b> &colors) {
	static cv::Vec3b black(0, 0, 0);

	size_t darkestColor = 0;
	double colorDistance = ConsoleImage::ColorDistance(colors[darkestColor], black);
	for (size_t i = 1; i < colors.size(); ++i) {
		if (ConsoleImage::ColorDistance(colors[i], black) < colorDistance) {
			darkestColor = i;
			colorDistance = ConsoleImage::ColorDistance(colors[i], black);
		}
	}

	if (darkestColor != 0) {
		cv::Vec3b temp = colors[0];
		colors[0] = colors[darkestColor];
		colors[darkestColor] = temp;
	}
}


void ConsoleImageViewer::resizeImage(short fontSize, float scaleFactor) {
	COORD largestSize = window->getLargestSize();

	// determine how many rows need to be removed to make the image appear like it has square pixels in the console
	short rows = (fontSize >= 3 && fontSize <= 5) || fontSize == 7 || (fontSize >= 10 && fontSize <= 12) || (fontSize >= 15 && fontSize <= 17) ? 2 : 3;

	// only scale if we have to
	if (scaleFactor == 1 && image.getWidth() <= largestSize.X && (image.getHeight() / rows) <= largestSize.Y) goto MakeSquare;

	// adjust the scale factor so that the image isn't too big
	if (image.getWidth() * scaleFactor > largestSize.X || (image.getHeight() * scaleFactor) / rows > largestSize.Y)
		scaleFactor = fmin(largestSize.X / (float)image.getWidth(), largestSize.Y / (float)(image.getHeight() / rows));

	// scale the image down to the largest that it can without being too big
	image.scale(scaleFactor);
MakeSquare:
	image.keepRows(rows);
}