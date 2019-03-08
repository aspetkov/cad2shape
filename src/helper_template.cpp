#include "stdafx.h"
#include <cstring>
#include <string>
#include <fstream> 
#include <sstream>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

template <typename T>
double atod(const std::basic_string<T> &str){
	std::basic_stringstream<T> stream(str);
	double res;
	return !(stream >> res) ? 0 : res;
}

template <typename T>
int atoi(const std::basic_string<T> &str){
	std::basic_stringstream<T> stream(str);
	int res;
	return !(stream >> res) ? 0 : res;
}