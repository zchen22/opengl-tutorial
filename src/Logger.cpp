#include "Logger.h"

#include <cstdarg>
#include <cstdlib>

Logger::Logger() {
}

Logger::~Logger() {
}

void Logger::Print(const char* format, va_list argList) {
	vfprintf(stderr, format, argList);
}

void Logger::Debug(const char* format, ...) {
	fprintf(stderr, "%-10s", "DEBUG");
	va_list argList;
	va_start(argList, format);
	Print(format, argList);
	va_end(argList);
}

void Logger::Warning(const char* format, ...) {
	fprintf(stderr, "%-10s", "WARNING");
	va_list argList;
	va_start(argList, format);
	Print(format, argList);
	va_end(argList);
}

void Logger::Error(const char* format, ...) {
	fprintf(stderr, "%-10s", "ERROR");
	va_list argList;
	va_start(argList, format);
	Print(format, argList);
	va_end(argList);
	exit(EXIT_FAILURE);
}
