#include "ConsoleImageViewer.h"
#include "ImageCommand.h"

#include <conio.h>
#include <sstream>
#include <fstream>




class InvalidSyntaxException : public std::exception {
public:
	InvalidSyntaxException(const char* msg) : std::exception(msg) {}
	InvalidSyntaxException() : std::exception() {}
};



void DisplayHelp() {
	/* --- USAGE --- */
	printf("Usage:");


	printf("\n\n     *.exe \"IMAGE\" [OPTION]...\n\n");



	/* --- OPTIONS --- */
	printf("\nOptions:");


	printf("\n\n     -fs n\t\t(Font size) [default: 1]\n\t\t\tDisplays the image in font size 'n'\n");

	printf("\n\n     -sf n\t\t(Scale factor) [default: 1.0]\n\t\t\tScale the image by 'n'\n");

	printf("\n\n     -it [p|a|r]\t(Image type) [default: p]\n\t\t\tChanges the characters used to display the image\n");
	printf("\n\t\t\t[p: pixel (ascii 219)]\n\t\t\t[a: ascii]\n\t\t\t[r: ascii (reduced characters)]\n");

	printf("\n\n     -am [pre|post]\t(Ascii mode) [default: pre]\n\t\t\tChanges whether ascii characters are assigned before or\n\t\t\tafter image processing\n");
	printf("\n\t\t\t[pre: pre-process]\n\t\t\t[post: post-process]\n\n\t\t\tNOTE: this option is ignored if the image type is pixel\n");

	printf("\n\n     -cm [a|f|n]\t(Color mode) [default: a]\n\t\t\tChanges the algorithm used to process the image\n");
	printf("\n\t\t\t[a: atkinson dithering]\n\t\t\t[f: floyd-steinberg dithering]\n\t\t\t[n: nearest value]\n");

	printf("\n\n     -ic [\"FILE\"|$*]\t(Image colors) [default: $default]\n\t\t\tChoose the set of colors the image processing is\n\t\t\tapplied with\n");
	printf("\n\t\t\t[\"FILE\": path to file containing colors]\n\t\t\t[$*: predefined color sets]\n\t\t\t\t-$grey: 16 shades of grey\n\t\t\t\t-$default: default command prompt colors\n\t\t\t\t-$bw: black and white\n\t\t\t\t-$approx(n): n approximate colors\n\n\t\t\tNOTE: colors specified in a file should be delimited\n\t\t\tby new lines with r,g,b elements delimited by commas\n");

	printf("\n\n     -tc [\"FILE\"|$*]\t(Text colors) [default: $default]\n\t\t\tChoose the set of colors the text is displayed with\n");
	printf("\n\t\t\t[\"FILE\": path to file containing colors]\n\t\t\t[$*: predefined color sets]\n\t\t\t\t-$grey: 16 shades of grey\n\t\t\t\t-$default: default command prompt colors\n\t\t\t\t-$bw: black and white\n\t\t\t\t-$approx(n): n approximate colors\n\n\t\t\tNOTE: colors specified in a file should be delimited\n\t\t\tby new lines with r,g,b elements delimited by commas\n");



	/* --- EXAMPLES --- */
	printf("\nExamples:");


	printf("\n\n     *.exe \"image.png\"");
	printf("\n\n\t\t\tDisplays image.png using pixels, atkinson dithered\n\t\t\tand colored with the default colors\n");

	printf("\n\n     *.exe \"image.png\" -fs 4 -it a -am post -cm f");
	printf("\n\n\t\t\tDisplays image.png using post-process ascii in font\n\t\t\tsize 4, floyd-steinberg dithered and colored with\n\t\t\tthe default colors\n");

	printf("\n\n     *.exe \"image.png\" -ic \"colors.txt\" -tc $grey");
	printf("\n\n\t\t\tDisplays image.png using pixels, atkinson dithered\n\t\t\twith the colors defined in colors.txt and colored with\n\t\t\t16-color greyscale\n");

	printf("\n\n     *.exe \"image.png\" -ic $approx(15) -tc $approx(15)");
	printf("\n\n\t\t\tDisplays image.png using pixels, atkinson dithered\n\t\t\tand colored with 15 approximated colors\n");



	printf("\n");
}



int parseApprox(const char * str) {
	const static char * target = "$approx("; // len 8 (not including null character)

	int i = 0;
	for (; i < 8; ++i) {
		if (str[i] != target[i] || str[i] == 0) return -1;
	}

	std::ostringstream oss;
	while (str[i] != ')' && str[i] != 0) {
		oss << str[i];
		++i;
	}

	try {
		return std::stoi(oss.str());
	}
	catch (std::exception e) {
		return -1;
	}
}

void getColorsFromFile(const std::string & file, std::vector<cv::Vec3b> & colors) {
	std::ifstream fin(file, std::ios::beg);

	std::string line, token;
	cv::Vec3b color;
	while (std::getline(fin, line)) {
		std::istringstream iss(line);
		for (int i = 0; i < 3; ++i) {
			std::getline(iss, token, ',');
			try {
				color[i] = std::stoi(token);
			}
			catch (std::exception e) {
				throw InvalidSyntaxException("Invalid color format in file.");
			}
		}
		colors.push_back(color);
	}
}


void ParseInput(int argc, char ** argv) {
	if (argc <= 1) return DisplayHelp();

	cid::ImageCommand command;
	command.imagePath = argv[1];
	command.fontSize = 1;
	command.scaleFactor = 1.f;

	command.flags = 0;
	command.imageColorOption = cid::COLOROPTION_DEFAULT;
	command.textColorOption = cid::COLOROPTION_DEFAULT;


	for (size_t i = 2; i < argc; ++i) {
		if (!strcmp("-fs", argv[i])) {
			try {
				command.fontSize = std::stoi(argv[++i]);
			}
			catch (std::exception e) {
				throw InvalidSyntaxException("Invalid font size parameter provided.");
			}
		}
		else if (!strcmp("-sf", argv[i])) {
			command.scaleFactor = (float)std::atof(argv[++i]);
			if (command.scaleFactor <= 0) {
				throw InvalidSyntaxException("Invalid scale factor parameter provided.");
			}
		}
		else if (!strcmp("-it", argv[i])) {
			if (!strcmp("p", argv[++i])) {
				command.flags |= cid::IMAGETYPE_PIXEL;
			}
			else if (!strcmp("a", argv[i])) {
				command.flags |= cid::IMAGETYPE_ASCII;
			}
			else if (!strcmp("r", argv[i])) {
				command.flags |= cid::IMAGETYPE_ASCII_SMALL;
			}
			else {
				throw InvalidSyntaxException("Invalid image type parameter provided.");
			}
		}
		else if (!strcmp("-am", argv[i])) {
			if (!strcmp("pre", argv[++i])) {
				command.flags |= cid::ASCIIMODE_PREPROCESS;
			}
			else if (!strcmp("post", argv[i])) {
				command.flags |= cid::ASCIIMODE_POSTPROCESS;
			}
			else {
				throw InvalidSyntaxException("Invalid ascii mode parameter provided.");
			}
		}
		else if (!strcmp("-cm", argv[i])) {
			if (!strcmp("a", argv[++i])) {
				command.flags |= cid::COLORMODE_DITHER_ATKINSON;
			}
			else if (!strcmp("f", argv[i])) {
				command.flags |= cid::COLORMODE_DITHER_FLOYDSTEINBERG;
			}
			else if (!strcmp("n", argv[i])) {
				command.flags |= cid::COLORMODE_NEAREST_VALUE;
			}
			else {
				throw InvalidSyntaxException("Invalid color mode parameter provided.");
			}
		}
		else if (!strcmp("-ic", argv[i])) {
			if (argv[++i][0] == '$') {
				if (!strcmp("$grey", argv[i])) {
					command.imageColorOption = cid::COLOROPTION_GREY;
				}
				else if (!strcmp("$default", argv[i])) {
					command.imageColorOption = cid::COLOROPTION_DEFAULT;
				}
				else if (!strcmp("$bw", argv[i])) {
					command.imageColorOption = cid::COLOROPTION_BLACK_AND_WHITE;
				}
				else if ((command.numApproximateImageColors = parseApprox(argv[i])) > 0) {
					command.imageColorOption = cid::COLOROPTION_APPROXIMATE;
				}
				else {
					throw InvalidSyntaxException("Invalid color mode parameter provided.");
				}
			}
			else {
				command.imageColorOption = cid::COLOROPTION_FILE;
				if (argv[i][0] == '"') {
					std::ostringstream oss;
					while (argv[i][strlen(argv[i] - 1)] != '"') {
						oss << argv[i++];
					}
					command.imageColorFile = oss.str();
				}
				else {
					command.imageColorFile = argv[i];
				}
			}
		}
		else if (!strcmp("-tc", argv[i])) {
			if (argv[++i][0] == '$') {
				if (!strcmp("$grey", argv[i])) {
					command.textColorOption = cid::COLOROPTION_GREY;
				}
				else if (!strcmp("$default", argv[i])) {
					command.textColorOption = cid::COLOROPTION_DEFAULT;
				}
				else if (!strcmp("$bw", argv[i])) {
					command.textColorOption = cid::COLOROPTION_BLACK_AND_WHITE;
				}
				else if ((command.numApproximateTextColors = parseApprox(argv[i])) > 0) {
					command.textColorOption = cid::COLOROPTION_APPROXIMATE;
				}
				else {
					throw InvalidSyntaxException("Invalid color mode parameter provided.");
				}
			}
			else {
				command.textColorOption = cid::COLOROPTION_FILE;
				if (argv[i][0] == '"') {
					std::ostringstream oss;
					while (argv[i][strlen(argv[i] - 1)] != '"') {
						oss << argv[i++];
					}
					command.textColorFile = oss.str();
				}
				else {
					command.textColorFile = argv[i];
				}
			}
		}
		else {
			std::ostringstream oss;
			oss << "Invalid option: " << argv[i];
			throw InvalidSyntaxException(oss.str().c_str());
		}
	}


	cid::ConsoleImageViewer viewer(command.imagePath);


	std::vector<cv::Vec3b> imageColors;
	switch (command.imageColorOption) {
	case cid::COLOROPTION_GREY:
		imageColors = cid::COLORS_GREYSCALE;
		break;
	case cid::COLOROPTION_BLACK_AND_WHITE:
		imageColors = cid::COLORS_BLACK_AND_WHITE;
		break;
	case cid::COLOROPTION_FILE:
		getColorsFromFile(command.imageColorFile, imageColors);
		break;
	case cid::COLOROPTION_APPROXIMATE:
		viewer.getImage().getApproximateColors(command.numApproximateImageColors, imageColors);
		break;
	default:
		imageColors = cid::COLORS_DEFAULT;
		break;
	}


	std::vector<cv::Vec3b> textColors;
	switch (command.textColorOption) {
	case cid::COLOROPTION_GREY:
		textColors = cid::COLORS_GREYSCALE;
		break;
	case cid::COLOROPTION_BLACK_AND_WHITE:
		textColors = cid::COLORS_BLACK_AND_WHITE;
		break;
	case cid::COLOROPTION_FILE:
		getColorsFromFile(command.textColorFile, textColors);
		break;
	case cid::COLOROPTION_APPROXIMATE:
		if (command.imageColorOption == cid::COLOROPTION_APPROXIMATE && command.numApproximateImageColors == command.numApproximateTextColors) {
			textColors = imageColors;
		}
		else {
			viewer.getImage().getApproximateColors(command.numApproximateTextColors, textColors);
		}
		break;
	default:
		textColors = cid::COLORS_DEFAULT;
		break;
	}



	viewer.display(
		cid::DisplayConfig(
			command.flags,
			imageColors,
			textColors
		),
		command.fontSize, command.scaleFactor
	);


	while (!_kbhit()) { Sleep(250); }
}



int main(int argc, char ** argv) {
	try {
		ParseInput(argc, argv);
	}
	catch (InvalidSyntaxException e) {
		printf("%s\nSee the help page for usage by running the program with no arguments.\n", e.what());
	}

	return 0;
}