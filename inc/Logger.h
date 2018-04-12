#pragma once

class Logger {
public:
	Logger();
	~Logger();
	void Print(const char* format, ...);
	void Debug(const char* format, ...);
	void Warning(const char* format, ...);
	void Error(const char* format, ...);
};
