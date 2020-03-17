#ifndef GUARD_CONSOLE_IMAGE_H
#define GUARD_CONSOLE_IMAGE_H
#pragma once


#include "opencv2/opencv.hpp"

#include <string>
#include <vector>



namespace cid {




	class ConsoleImage {

	public:

		static const cv::Vec3b & FindClosestPalletteColor(cv::Vec3b color, const std::vector<cv::Vec3b> &colors);
		static size_t FindClosestPalletteColorIndex(cv::Vec3b color, const std::vector<cv::Vec3b> &colors);

		static double ColorDistance(const cv::Vec3b &c1, const cv::Vec3b &c2);


	public:

		ConsoleImage(const std::string &imagePath);
		ConsoleImage(const ConsoleImage &copy);
		ConsoleImage();

		ConsoleImage & operator=(const ConsoleImage &rhs);


		void load(const std::string &imagePath);

		void show() const;


		void scale(float factor);

		void removeRows(short n);
		void removeColumns(short n);

		void keepRows(short n);
		void keepColumns(short n);

		void ditherFloydSteinberg(const std::vector<cv::Vec3b> &colors);
		void ditherAtkinson(const std::vector<cv::Vec3b> &colors);
		void nearestValue(const std::vector<cv::Vec3b> &colors);

		void getApproximateColors(size_t numColors, std::vector<cv::Vec3b> &outputColors) const;
		std::vector<cv::Vec3b> getApproximateColors(size_t numColors) const;


		int getWidth() const;
		int getHeight() const;


		cv::Vec3b & index(int x, int y);
		const cv::Vec3b & index(int x, int y) const;

		const std::string & path() const;


	private:

		void averageByClusters(std::vector<cv::Vec3b> &colors, int numClusters) const;
		void getUniqueColors(std::vector<cv::Vec3b> &colors, size_t desiredColorAmnt) const;
		void averageAlikePairs(std::vector<cv::Vec3b> &colors, size_t desiredColorAmnt) const;


	private:

		std::string imagePath;
		cv::Mat image;


	};




};

#endif // GUARD_CONSOLE_IMAGE_H