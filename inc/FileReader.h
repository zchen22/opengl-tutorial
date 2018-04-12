#pragma once

class FileReader {
public:
	FileReader();
	FileReader(const char* fileName);
	~FileReader();
	long int GetSize();
	int ReadAsString(char* str);
private:
	const char* fileName_;
	long int size_;
};
