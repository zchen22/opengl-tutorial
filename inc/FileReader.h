#pragma once

#include <string>

class FileReader {
public:
	FileReader(const std::string& fileName);
	~FileReader();
	int GetSize(long int& size);
	int ReadAsString(std::string& str);
private:
	const std::string fileName_;
};
