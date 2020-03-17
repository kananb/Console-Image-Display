#ifndef GUARD_CONSOLE_WINDOW_H
#define GUARD_CONSOLE_WINDOW_H
#pragma once


#include <windows.h>
#include <string>
#include <vector>



namespace cid {




	class ConsoleWindow {

	public:

		ConsoleWindow(const std::string &windowTitle, short width, short height, short fontSize = 1, const std::string &fontName = "Consolas");
		~ConsoleWindow();

		void destroy();


		void updateBuffer();


		void setSize(short width, short height);
		short getWidth() const;
		short getHeight() const;
		COORD getLargestSize() const;

		void fillBuffer(CHAR_INFO value);
		void setBufferElement(short row, short col, CHAR_INFO value);
		CHAR_INFO getBufferElement(short row, short col) const;

		void setColorTableValue(size_t index, COLORREF value);
		COLORREF getColorTableValue(size_t index) const;

		void setFont(const std::string &fontName, short fontSize);
		const std::string & getFontName() const;
		short getFontSize() const;


	protected:

		bool initializeWindow();


		bool updateBufferSize();
		bool updateColorTable();
		bool updateFont();

		void initializeLocalColorTable();


	private:

		HANDLE hConsole;
		std::string windowTitle;

		COORD bufferSize, largestSize;
		std::vector<CHAR_INFO> consoleBuffer;

		COLORREF colorTable[16];

		short fontSize;
		std::string fontName;


	};




};

#endif // GUARD_CONSOLE_WINDOW_H