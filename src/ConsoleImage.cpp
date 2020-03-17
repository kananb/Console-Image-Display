#include "ConsoleImage.h"

#include <windows.h>

using namespace cid;




ConsoleImage::ConsoleImage(const std::string &imagePath) :
	imagePath(imagePath),
	image(cv::imread(imagePath, CV_LOAD_IMAGE_COLOR)) {
	cv::cvtColor(image, image, CV_BGR2RGB);
}

ConsoleImage::ConsoleImage(const ConsoleImage &copy) :
	imagePath(copy.imagePath),
	image(copy.image.clone()) {
}

ConsoleImage::ConsoleImage() :
	imagePath(""),
	image() {
}


ConsoleImage & ConsoleImage::operator=(const ConsoleImage &rhs) {
	imagePath = rhs.imagePath;
	image = rhs.image.clone();
	return *this;
}



void ConsoleImage::load(const std::string &imagePath) {
	this->imagePath = imagePath;
	image = cv::imread(imagePath, CV_LOAD_IMAGE_COLOR);
	cv::cvtColor(image, image, CV_BGR2RGB);
}


void ConsoleImage::show() const {
	cv::imshow(imagePath, image);
	cv::waitKey(0);
}



void ConsoleImage::scale(float factor) {
	if (factor > 1)
		cv::resize(image, image, cv::Size(), factor, factor, cv::INTER_CUBIC);
	else if (factor < 1)
		cv::resize(image, image, cv::Size(), factor, factor, cv::INTER_AREA);
}


void ConsoleImage::removeRows(short n) {
	cv::Mat result(image.rows - image.rows / n, image.cols, image.type());

	int yresult = 0;
	for (int y = 0; y < image.rows; ++y) {
		if (y % n == 0) continue;
		for (int x = 0; x < image.cols; ++x) {
			if (yresult >= result.rows) break;
			result.at<cv::Vec3b>(yresult, x) = image.at<cv::Vec3b>(y, x);
		}
		++yresult;
	}

	image = result;
}

void ConsoleImage::removeColumns(short n) {
	cv::Mat result(image.rows, image.cols - image.cols / n, image.type());

	int xresult = 0;
	for (int x = 0; x < image.cols; ++x) {
		if (x % n == 0) continue;
		for (int y = 0; y < image.rows; ++y) {
			if (xresult >= result.cols) break;
			result.at<cv::Vec3b>(y, xresult) = image.at<cv::Vec3b>(y, x);
		}
		++xresult;
	}

	image = result;
}


void ConsoleImage::keepRows(short n) {
	cv::Mat result(image.rows / n, image.cols, image.type());

	int yresult = 0;
	for (int y = 0; y < image.rows; ++y) {
		if (y % n != 0) continue;
		for (int x = 0; x < image.cols; ++x) {
			if (yresult >= result.rows) break;
			result.at<cv::Vec3b>(yresult, x) = image.at<cv::Vec3b>(y, x);
		}
		++yresult;
	}

	image = result;
}

void ConsoleImage::keepColumns(short n) {
	cv::Mat result(image.rows, image.cols / n, image.type());

	int xresult = 0;
	for (int x = 0; x < image.cols; ++x) {
		if (x % n != 0) continue;
		for (int y = 0; y < image.rows; ++y) {
			if (xresult >= result.cols) break;
			result.at<cv::Vec3b>(y, xresult) = image.at<cv::Vec3b>(y, x);
		}
		++xresult;
	}

	image = result;
}


void ConsoleImage::ditherFloydSteinberg(const std::vector<cv::Vec3b> &colors) {
	static float w1 = 1. / 8;
	cv::Vec3s quantError;

	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x) {
			const cv::Vec3b &oldPixel = image.at<cv::Vec3b>(y, x);
			const cv::Vec3b &newPixel = FindClosestPalletteColor(oldPixel, colors);
			
			quantError[0] = (short)oldPixel[0] - newPixel[0];
			quantError[1] = (short)oldPixel[1] - newPixel[1];
			quantError[2] = (short)oldPixel[2] - newPixel[2];
			image.at<cv::Vec3b>(y, x) = newPixel;

			if (x + 1 < image.cols) {
				image.at<cv::Vec3b>(y, x + 1)[0] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y, x + 1)[0] + (quantError[0] * w1)));
				image.at<cv::Vec3b>(y, x + 1)[1] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y, x + 1)[1] + (quantError[1] * w1)));
				image.at<cv::Vec3b>(y, x + 1)[2] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y, x + 1)[2] + (quantError[2] * w1)));
			}
			if (x + 2 < image.cols) {
				image.at<cv::Vec3b>(y, x + 2)[0] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y, x + 2)[0] + (quantError[0] * w1)));
				image.at<cv::Vec3b>(y, x + 2)[1] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y, x + 2)[1] + (quantError[1] * w1)));
				image.at<cv::Vec3b>(y, x + 2)[2] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y, x + 2)[2] + (quantError[2] * w1)));
			}
			if (y + 1 < image.rows) {
				image.at<cv::Vec3b>(y + 1, x)[0] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x)[0] + (quantError[0] * w1)));
				image.at<cv::Vec3b>(y + 1, x)[1] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x)[1] + (quantError[1] * w1)));
				image.at<cv::Vec3b>(y + 1, x)[2] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x)[2] + (quantError[2] * w1)));
			}
			if (y + 2 < image.rows) {
				image.at<cv::Vec3b>(y + 2, x)[0] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 2, x)[0] + (quantError[0] * w1)));
				image.at<cv::Vec3b>(y + 2, x)[1] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 2, x)[1] + (quantError[1] * w1)));
				image.at<cv::Vec3b>(y + 2, x)[2] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 2, x)[2] + (quantError[2] * w1)));
			}
			if (x - 1 >= 0 && y + 1 < image.rows) {
				image.at<cv::Vec3b>(y + 1, x - 1)[0] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x - 1)[0] + (quantError[0] * w1)));
				image.at<cv::Vec3b>(y + 1, x - 1)[1] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x - 1)[1] + (quantError[1] * w1)));
				image.at<cv::Vec3b>(y + 1, x - 1)[2] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x - 1)[2] + (quantError[2] * w1)));
			}
			if (x + 1 < image.cols && y + 1 < image.rows) {
				image.at<cv::Vec3b>(y + 1, x + 1)[0] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x + 1)[0] + (quantError[0] * w1)));
				image.at<cv::Vec3b>(y + 1, x + 1)[1] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x + 1)[1] + (quantError[1] * w1)));
				image.at<cv::Vec3b>(y + 1, x + 1)[2] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x + 1)[2] + (quantError[2] * w1)));
			}
		}
	}
}

void ConsoleImage::ditherAtkinson(const std::vector<cv::Vec3b> &colors) {
	static float w1 = 7. / 16, w2 = 5. / 16, w3 = 3. / 16, w4 = 1. / 16;
	cv::Vec3s quantError;

	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x) {
			const cv::Vec3b &oldPixel = image.at<cv::Vec3b>(y, x);
			const cv::Vec3b &newPixel = FindClosestPalletteColor(oldPixel, colors);

			quantError[0] = (short)oldPixel[0] - newPixel[0];
			quantError[1] = (short)oldPixel[1] - newPixel[1];
			quantError[2] = (short)oldPixel[2] - newPixel[2];
			image.at<cv::Vec3b>(y, x) = newPixel;

			if (x + 1 < image.cols) {
				image.at<cv::Vec3b>(y, x + 1)[0] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y, x + 1)[0] + (quantError[0] * w1)));
				image.at<cv::Vec3b>(y, x + 1)[1] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y, x + 1)[1] + (quantError[1] * w1)));
				image.at<cv::Vec3b>(y, x + 1)[2] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y, x + 1)[2] + (quantError[2] * w1)));
			}
			if (y + 1 < image.rows) {
				image.at<cv::Vec3b>(y + 1, x)[0] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x)[0] + (quantError[0] * w2)));
				image.at<cv::Vec3b>(y + 1, x)[1] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x)[1] + (quantError[1] * w2)));
				image.at<cv::Vec3b>(y + 1, x)[2] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x)[2] + (quantError[2] * w2)));
			}
			if (x - 1 >= 0 && y + 1 < image.rows) {
				image.at<cv::Vec3b>(y + 1, x - 1)[0] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x - 1)[0] + (quantError[0] * w3)));
				image.at<cv::Vec3b>(y + 1, x - 1)[1] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x - 1)[1] + (quantError[1] * w3)));
				image.at<cv::Vec3b>(y + 1, x - 1)[2] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x - 1)[2] + (quantError[2] * w3)));
			}
			if (x + 1 < image.cols && y + 1 < image.rows) {
				image.at<cv::Vec3b>(y + 1, x + 1)[0] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x + 1)[0] + (quantError[0] * w4)));
				image.at<cv::Vec3b>(y + 1, x + 1)[1] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x + 1)[1] + (quantError[1] * w4)));
				image.at<cv::Vec3b>(y + 1, x + 1)[2] = (uchar)min(255, max(0, image.at<cv::Vec3b>(y + 1, x + 1)[2] + (quantError[2] * w4)));
			}
		}
	}
}

void ConsoleImage::nearestValue(const std::vector<cv::Vec3b> &colors) {
	for (int y = 0; y < image.rows; ++y)
		for (int x = 0; x < image.cols; ++x)
			image.at<cv::Vec3b>(y, x) = FindClosestPalletteColor(image.at<cv::Vec3b>(y, x), colors);
}

void ConsoleImage::getApproximateColors(size_t numColors, std::vector<cv::Vec3b> &outputColors) const {
	if (numColors <= 1) {
		averageByClusters(outputColors, 1);
		return;
	}
	averageByClusters(outputColors, (int)numColors * 8);
	getUniqueColors(outputColors, numColors * 3);
	averageAlikePairs(outputColors, numColors);
}

std::vector<cv::Vec3b> ConsoleImage::getApproximateColors(size_t numColors) const {
	std::vector<cv::Vec3b> outputColors;
	getApproximateColors(numColors, outputColors);
	return outputColors;
}




int ConsoleImage::getWidth() const {
	return image.cols;
}

int ConsoleImage::getHeight() const {
	return image.rows;
}



cv::Vec3b & ConsoleImage::index(int x, int y) {
	return image.at<cv::Vec3b>(y, x);
}

const cv::Vec3b & ConsoleImage::index(int x, int y) const {
	return image.at<cv::Vec3b>(y, x);
}


const std::string & ConsoleImage::path() const {
	return imagePath;
}




void ConsoleImage::averageByClusters(std::vector<cv::Vec3b> &colors, int numClusters) const {
	if (numClusters > image.cols || numClusters > image.rows) numClusters = min(image.cols, image.rows);
	int clusterWidth = image.cols / numClusters, clusterHeight = image.rows / numClusters;
	int clusterArea = clusterWidth * clusterHeight;

	colors.resize(numClusters * numClusters, cv::Vec3b(0, 0, 0));

	// split the image up into a bunch of boxes/clusters and find the average color for each cluster
	long averageColor[3] = { 0, 0, 0 };
	for (int cy = 0; cy < numClusters; ++cy) {
		for (int cx = 0; cx < numClusters; ++cx) {
			// iterate through each pixel in the cluster
			for (int r = 0; r < clusterHeight; ++r) {
				for (int c = 0; c < clusterWidth; ++c) {
					averageColor[0] += image.at<cv::Vec3b>(r + cy * clusterHeight, c + cx * clusterWidth)[0];
					averageColor[1] += image.at<cv::Vec3b>(r + cy * clusterHeight, c + cx * clusterWidth)[1];
					averageColor[2] += image.at<cv::Vec3b>(r + cy * clusterHeight, c + cx * clusterWidth)[2];
				}
			}

			averageColor[0] /= clusterArea;
			averageColor[1] /= clusterArea;
			averageColor[2] /= clusterArea;
			colors[cy * numClusters + cx][0] = (uchar)averageColor[0];
			colors[cy * numClusters + cx][1] = (uchar)averageColor[1];
			colors[cy * numClusters + cx][2] = (uchar)averageColor[2];
		}
	}
}

void ConsoleImage::getUniqueColors(std::vector<cv::Vec3b> &colors, size_t desiredColorAmnt) const {
	size_t colorCounter = 0;
	int minDistance = 441; // 441 is the furthest distance two colors can be from each other

	// checks if there is already a color that has been counted that is within minDistance from the current color
	static auto IsUnique = [](const cv::Vec3b &color, size_t colorCounter, int minDistance, const std::vector<cv::Vec3b> &colors) {
		for (size_t i = 0; i < colorCounter; ++i) {
			if (ColorDistance(colors[i], color) <= minDistance) return false;
		}
		return true;
	};

	cv::Vec3b temp;
	while (colorCounter <= desiredColorAmnt) {
		for (size_t i = colorCounter; i < colors.size(); ++i) {
			if (IsUnique(colors[i], colorCounter, minDistance, colors)) {
				temp = colors[colorCounter];
				colors[colorCounter++] = colors[i];
				colors[i] = temp;
			}
		}
		minDistance -= 20;
	}

	if (colors.size() > desiredColorAmnt)
		colors.resize(desiredColorAmnt);
}

void ConsoleImage::averageAlikePairs(std::vector<cv::Vec3b> &colors, size_t desiredColorAmnt) const {
	if (colors.size() <= desiredColorAmnt || colors.size() <= 1) return;

	size_t colorPair[2];
	double pairDistance;

	int closestColor;
	double colorDistance;
	cv::Vec3b temp;

	while (colors.size() > desiredColorAmnt || colors.size() <= 1) {
		// loop through each color in the array and find the color closest to it
		// use the distance between each color and the color closest to it to find the
		// closest pair of colors
		colorPair[0] = 0; colorPair[1] = 1;
		pairDistance = ColorDistance(colors[colorPair[0]], colors[colorPair[1]]);
		for (int i = 0; i < colors.size(); ++i) {
			closestColor = (i + 1) % colors.size();
			colorDistance = ColorDistance(colors[i], colors[closestColor]);

			for (int j = 0; j < colors.size(); ++j) {
				if (j == i) continue;
				if (ColorDistance(colors[i], colors[j]) < colorDistance) {
					closestColor = j;
					colorDistance = ColorDistance(colors[i], colors[closestColor]);
				}
			}

			if (colorDistance < pairDistance) {
				colorPair[0] = i;
				colorPair[1] = closestColor;
				pairDistance = ColorDistance(colors[colorPair[0]], colors[colorPair[1]]);
			}
		}

		// average pair and store in first color in pair
		colors[colorPair[0]][0] = (colors[colorPair[0]][0] + colors[colorPair[1]][0]) / 2;
		colors[colorPair[0]][1] = (colors[colorPair[0]][1] + colors[colorPair[1]][1]) / 2;
		colors[colorPair[0]][2] = (colors[colorPair[0]][2] + colors[colorPair[1]][2]) / 2;

		// swap last color with second color in pair
		if (colorPair[1] != colors.size() - 1) {
			temp = colors[colors.size() - 1];
			colors[colors.size() - 1] = colors[colorPair[1]];
			colors[colorPair[1]] = temp;
		}

		// swap first color with first color in pair
		if (colorPair[0] != 0) {
			temp = colors[0];
			colors[0] = colors[colorPair[0]];
			colors[colorPair[0]] = temp;
		}

		// resize vector to exclude the second color in the pair
		// resizing a vector to become smaller won't reallocate memory
		colors.resize(colors.size() - 1);
	}
}





const cv::Vec3b & ConsoleImage::FindClosestPalletteColor(cv::Vec3b color, const std::vector<cv::Vec3b> &colors) {
	return colors[FindClosestPalletteColorIndex(color, colors)];
}

size_t ConsoleImage::FindClosestPalletteColorIndex(cv::Vec3b color, const std::vector<cv::Vec3b> &colors) {
	if (colors.size() < 1) return 0;

	size_t closestColorIndex = 0;
	double shortestDistance = ColorDistance(color, colors[closestColorIndex]);
	for (size_t i = 0; i < colors.size(); ++i) {
		if (ColorDistance(color, colors[i]) < shortestDistance) {
			closestColorIndex = i;
			shortestDistance = (unsigned int)ColorDistance(color, colors[closestColorIndex]);
			if ((int)shortestDistance == 0) break;
		}
	}

	return closestColorIndex;
}


double ConsoleImage::ColorDistance(const cv::Vec3b &c1, const cv::Vec3b &c2) {
	return sqrt(
		pow(c1[0] - c2[0], 2) +
		pow(c1[1] - c2[1], 2) +
		pow(c1[2] - c2[2], 2));
}