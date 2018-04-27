#include "FileReader.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>

FileReader::FileReader(const std::string& fileName)
    : fileName_(fileName) {
}

FileReader::~FileReader() {
}

int FileReader::GetSize(long int& size) {
	std::ifstream f(fileName_, std::ios::in);
	if (!f) {
		fprintf(stderr, "ERROR: cannot open file '%s'\n", fileName_.c_str());
		exit(EXIT_FAILURE);
	}
	f.seekg(0, std::ios::end);
	size = f.tellg();
	f.close();
	return 0;
}

int FileReader::ReadAsString(std::string& str) {
	std::ifstream f(fileName_, std::ios::in);
	if (!f.is_open()) {
		fprintf(stderr, "ERROR: cannot open file '%s'\n", fileName_.c_str());
		exit(EXIT_FAILURE);
	}
	f.seekg(0, std::ios::end);
	str.resize(f.tellg());
	f.seekg(0, std::ios::beg);
	f.read(&str[0], str.size());
	f.close();
	return 0;
}
