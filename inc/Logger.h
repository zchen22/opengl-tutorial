#pragma once

#include <cstdio>

class Logger {
public:
	Logger();
	~Logger();
	void Print(const char* format, va_list argList);
	void Debug(const char* format, ...);
	void Warning(const char* format, ...);
	void Error(const char* format, ...);
};
