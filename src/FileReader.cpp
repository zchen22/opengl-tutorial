#include "FileReader.h"

#include <cstdio>
#include <cstdlib>

FileReader::FileReader() {
}

FileReader::FileReader(const char * fileName)
    : fileName_(fileName) {
	FILE* f = nullptr;
	fopen_s(&f, fileName_, "r");
	if (!f) {
		fprintf(stderr, "ERROR: cannot open file '%s'\n", fileName_);
		exit(EXIT_FAILURE);
	}
	fseek(f, 0, SEEK_END);
	size_ = ftell(f);
	fclose(f);
}

FileReader::~FileReader() {
}

long int FileReader::GetSize() {
	return size_;
}

int FileReader::ReadAsString(char* str) {
	FILE* f = nullptr;
	fopen_s(&f, fileName_, "r");
	if (!f) {
		fprintf(stderr, "ERROR: cannot open file '%s'\n", fileName_);
		exit(EXIT_FAILURE);
	}
	fread(str, 1, size_, f);
	fclose(f);
	return 0;
}
