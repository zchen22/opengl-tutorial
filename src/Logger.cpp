#include "Logger.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

Logger::Logger() {
}

Logger::~Logger() {
}

void Logger::Print(const char* format, ...) {
	va_list argList;
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);
}

void Logger::Debug(const char* format, ...) {
	fprintf(stderr, "DEBUG:: ");
	va_list argList;
	va_start(argList, format);
	Print(format, argList);
	va_end(argList);
}

void Logger::Warning(const char* format, ...) {
	fprintf(stderr, "WARNING:: ");
	va_list argList;
	va_start(argList, format);
	Print(format, argList);
	va_end(argList);
}

void Logger::Error(const char* format, ...) {
	fprintf(stderr, "ERROR:: ");
	va_list argList;
	va_start(argList, format);
	Print(format, argList);
	va_end(argList);
	exit(EXIT_FAILURE);
}
