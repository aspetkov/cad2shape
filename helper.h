
#ifndef HELPER_H
#define HELPER_H
#include <string>
#include <cstring>
#include <vector>
#include "data_class.h"

using namespace std;

static std::string RemoveSpaceFromString(std::string s);

std::string ConcatLinesCoordinates(std::string AllCoordInputstring, std::string addLineCoord);

bool IsLineNumber(string s);

static char * ConvertDecToHex(int dec);

string  ReadLineFromBytes(structCharArray &temp_str);

std::string& trim(std::string& s);

std::string& trim(std::string& s, char c);

std::vector<std::string> split(const std::string &s, char delim);

std::vector<std::string> splitDataRow(const std::string &s, int fieldsCount);

string ConvertCyrCharToLatin(string str);

#endif