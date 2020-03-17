#include "ConsoleWindow.h"

#include <stdexcept>

using namespace cid;




ConsoleWindow::ConsoleWindow(const std::string &windowTitle, short width, short height, short fontSize, const std::string &fontName) :
	hConsole(NULL), windowTitle(windowTitle),
	bufferSize({ width, height }), largestSize({ 0 }), consoleBuffer(width * height, { 0 }),
	fontSize(fontSize), fontName(fontName) {
	initializeWindow();
}

ConsoleWindow::~ConsoleWindow() {
	destroy();
}


void ConsoleWindow::destroy() {
	CloseHandle(hConsole);
	FreeConsole();
}



void ConsoleWindow::updateBuffer() {
	SMALL_RECT windowRect = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 };
	updateFont();
	updateColorTable();
	WriteConsoleOutputA(hConsole, &consoleBuffer[0], bufferSize, { 0, 0 }, &windowRect);
}



void ConsoleWindow::setSize(short width, short height) {
	bufferSize = { width, height };
	updateFont();
	updateBufferSize();
}

short ConsoleWindow::getWidth() const {
	return bufferSize.X;
}

short ConsoleWindow::getHeight() const {
	return bufferSize.Y;
}

COORD ConsoleWindow::getLargestSize() const {
	return GetLargestConsoleWindowSize(hConsole);
}


void ConsoleWindow::fillBuffer(CHAR_INFO value) {
	for (size_t i = 0; i < consoleBuffer.size(); ++i)
		consoleBuffer[i] = value;
}

void ConsoleWindow::setBufferElement(short row, short col, CHAR_INFO value) {
	consoleBuffer[row * bufferSize.X + col] = value;
}

CHAR_INFO ConsoleWindow::getBufferElement(short row, short col) const {
	return consoleBuffer[row * bufferSize.X + col];
}


void ConsoleWindow::setColorTableValue(size_t index, COLORREF value) {
	if (index < 0 || index > 15) return;
	colorTable[index] = value;
}

COLORREF ConsoleWindow::getColorTableValue(size_t index) const {
	if (index < 0 || index > 15) return 0;
	return colorTable[index];
}



void ConsoleWindow::setFont(const std::string &fontName, short fontSize) {
	this->fontName = fontName;
	this->fontSize = fontSize;
	updateFont();
}

const std::string & ConsoleWindow::getFontName() const {
	return fontName;
}

short ConsoleWindow::getFontSize() const {
	return fontSize;
}




bool ConsoleWindow::initializeWindow() {
	if (!FreeConsole()) return false;
	if (!AllocConsole()) return false;

	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (hConsole == INVALID_HANDLE_VALUE) return false;

	if (!SetConsoleTitleA(windowTitle.c_str())) goto HandleFail;
	if (!SetConsoleActiveScreenBuffer(hConsole)) goto HandleFail;

	initializeLocalColorTable();
	if (!updateFont()) return false;
	return updateBufferSize();

HandleFail:
	CloseHandle(hConsole);
	return false;
}



bool ConsoleWindow::updateBufferSize() {
	if (bufferSize.X < 0 || bufferSize.Y < 0) bufferSize = largestSize;
	else {
		bufferSize.X = min(bufferSize.X, largestSize.X);
		bufferSize.Y = min(bufferSize.Y, largestSize.Y);
	}

	consoleBuffer.clear();
	consoleBuffer.resize(bufferSize.X * bufferSize.Y);


	SMALL_RECT windowRect = { 0, 0, 1, 1 };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &windowRect)) return false;

	if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) return false;

	windowRect = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &windowRect)) return false;

	return true;
}

bool ConsoleWindow::updateColorTable() {
	CONSOLE_SCREEN_BUFFER_INFOEX bufinfo = { 0 };
	bufinfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	if (!GetConsoleScreenBufferInfoEx(hConsole, &bufinfo)) return false;

	for (size_t i = 0; i < 16; ++i)
		bufinfo.ColorTable[i] = colorTable[i];

	if (!SetConsoleScreenBufferInfoEx(hConsole, &bufinfo)) return false;

	return true;
}

bool ConsoleWindow::updateFont() {
	CONSOLE_FONT_INFOEX cfont = { 0 };

	cfont.cbSize = sizeof(cfont);
	cfont.dwFontSize.Y = fontSize;
	lstrcpyW(cfont.FaceName, (LPCWSTR)fontName.c_str());

	if (!SetCurrentConsoleFontEx(hConsole, TRUE, &cfont)) return false;
	largestSize = getLargestSize();

	return true;
}


void ConsoleWindow::initializeLocalColorTable() {
	const COLORREF rgDefaultColorTable[16] = {
		RGB(0, 0, 0), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
		RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
		RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
		RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255) };
	for (size_t i = 0; i < 16; ++i)
		colorTable[i] = rgDefaultColorTable[i];
}