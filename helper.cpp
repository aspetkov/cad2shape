#include "stdafx.h"
#include <cstring>
#include <string>
#include <fstream> 
#include <sstream>
#include "helper.h"
#include "data_class.h"
#include "helper_template.cpp"

using namespace std;

bool IsLineNumber(string s)
{
	const char digitarray[] = { ' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	if (s == "") return false;

	const char *sChar = s.c_str();

	for each (char u in digitarray)
	{
		if (sChar[0] == u)  return true;
	}
	return false;
}



static char * ConvertDecToHex(int dec)
{
	int rem;
	string s = "";
	while (dec > 0)   // Do this whilst the quotient is greater than 0.
	{
		rem = dec % 16; // Get the remainder.
		if (rem > 9)
		{
			// Map the character given that the remainder is greater than 9.
			switch (rem)
			{
			case 10: s = "A" + s; break;
			case 11: s = "B" + s; break;
			case 12: s = "C" + s; break;
			case 13: s = "D" + s; break;
			case 14: s = "E" + s; break;
			case 15: s = "F" + s; break;
			}
		}
		else
		{
			s = char(rem + 48) + s; // Converts integer (0-9) to ASCII code.
		}
		dec = dec / 16;
	}
}

string  ReadLineFromBytes(structCharArray &temp_str)
{
	char previous = 0;
	char current = 0;
	string s = "";
	int cint;
	int cint1 = 0;


	while (temp_str.currentChar < temp_str.sizeCharArray)
	{
		cint = temp_str.pChars[temp_str.currentChar];
		if (cint < 0)
		{
			cint1 = cint + 256;
		}

		if (cint < 0 && cint1 <= 191 && cint1 >= 128)
		{
			cint = cint + 64;
			current = cint;
			s.push_back(cint);
			temp_str.currentChar += 1;
		}

		else if (cint == -43)
		{
			current = -71;
			s.push_back(current);
			temp_str.currentChar += 1;
		}
		else
		{
			current = cint;
			s.push_back(cint);
			temp_str.currentChar += 1;
		}

		if (current == '\r')
		{
			previous = '\r';
		}
		else if ((current == '\n' && previous == '\r') || (temp_str.currentChar == temp_str.sizeCharArray))
		{
			s.pop_back();
			s.pop_back();
			s = RemoveSpaceFromString(s);
			return s;
		}
	}
}

std::string& trim(std::string& s)
{
	const char* remChar = " \t\n\r\f\v";
	s.erase(s.find_last_not_of(remChar) + 1);
	s.erase(0, s.find_first_not_of(remChar));
	return s;
}

std::string& trim(std::string& s, char c)
{
	const char* remChar = " \t\n\r\f\v";
	s.erase(s.find_last_not_of(remChar) + 1);
	s.erase(0, s.find_first_not_of(remChar));
	s.erase(s.find_last_not_of(c) + 1);
	s.erase(0, s.find_first_not_of(c));
	return s;
}


static string RemoveSpaceFromString(string s)
{
	s = trim(s);
	const char *sChar = s.c_str();
	char delimiterCharSpace = ' ';
	string r = "";
	for (int j = 0; j < s.length(); j++)
	{
		if (sChar[j] != delimiterCharSpace)
		{
			r.push_back(sChar[j]);
		}
		else
		{
			if (j > 0)
			{
				if ((sChar[j - 1] != delimiterCharSpace) && (sChar[j] == delimiterCharSpace))
				{
					r.push_back(' ');
				}
			}
		}
	}
	return r;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

std::vector<std::string> splitDataRow(const std::string &s, int fieldsCount) {
	char delimiterCharComma = ',';
	size_t n = std::count(s.begin(), s.end(), delimiterCharComma);
	std::vector<std::string> elems;
	std::vector<std::string> elemsRev;
	split(s, delimiterCharComma, std::back_inserter(elems));

	int i = 0;
	string s1 = "";
	bool oneQuote = false;
	while (i < elems.size())
	{
		size_t n = std::count(elems[i].begin(), elems[i].end(), '\"');

		if ((n % 2 == 0) && !oneQuote)
		{
			if (s1 != "")
			{
				elemsRev.push_back(s1);
				s1 = "";
			}
			elemsRev.push_back(elems[i]);
			i++;
		}
		else if (n == 1)
		{
			oneQuote = !oneQuote;
			if (s1 == "")
			{
				s1 = elems[i];
			}
			else
			{
				s1 = s1 + ", " + elems[i];
			}
			i++;
		}
		else
		{
			if (s1 == "")
			{
				s1 = elems[i];
			}
			else
			{
				s1 = s1 + ", " + elems[i];
			}
			i++;
		}

	}
	if (elemsRev.size() > fieldsCount)
	{
		elemsRev.clear();
		while (elemsRev.size() < fieldsCount)
		{
			elemsRev.push_back("");
		}
		return elemsRev;
	}
	else
	{
		while (elemsRev.size() < fieldsCount)
		{
			elemsRev.push_back("");
		}
		return elemsRev;
	}
}



std::string ConcatLinesCoordinates(std::string AllCoordInputstring, std::string addLineCoord)

{
	char delimiterCharSpace = ' ';
	char delimiterCharSemicolon = ';';

	if (AllCoordInputstring == "") return addLineCoord;

	string AllCoordInputstringNew = AllCoordInputstring;
	vector <string>  splitstringsemicolon = split(AllCoordInputstring, delimiterCharSemicolon);
	vector <string>  splitLastLineCoordsemicolon = split(addLineCoord, delimiterCharSemicolon);

	vector <string>  firstAllCoordPoint = split(trim(splitstringsemicolon[0]), delimiterCharSpace);
	vector <string>  lastAllCoordPoint = split(trim(splitstringsemicolon[splitstringsemicolon.size() - 1]), delimiterCharSpace);
	vector <string>  firstLinePoint = split(trim(splitLastLineCoordsemicolon[0]), delimiterCharSpace);
	vector <string>  lastLinePoint = split(trim(splitLastLineCoordsemicolon[splitLastLineCoordsemicolon.size() - 1]), delimiterCharSpace);


	if ((lastAllCoordPoint[1] == firstLinePoint[1]) && (lastAllCoordPoint[2] == firstLinePoint[2]))
	{
		int NumberOfPoints = splitLastLineCoordsemicolon.size();
		for (int i = 0; i < NumberOfPoints; i++)
		{
			AllCoordInputstringNew = AllCoordInputstringNew + splitLastLineCoordsemicolon[i] + delimiterCharSemicolon;
		}
	}
	else if ((lastAllCoordPoint[1] == lastLinePoint[1]) && (lastAllCoordPoint[2] == lastLinePoint[2]))
	{
		int NumberOfPoints = splitLastLineCoordsemicolon.size();
		for (int i = NumberOfPoints - 1; i >= 0; i--)
		{
			AllCoordInputstringNew = AllCoordInputstringNew + splitLastLineCoordsemicolon[i] + delimiterCharSemicolon;
		}
	}
	else if ((firstAllCoordPoint[1] == firstLinePoint[1]) && (firstAllCoordPoint[2] == firstLinePoint[2]))
	{
		int NumberOfPoints = splitstringsemicolon.size();
		AllCoordInputstringNew = "";
		for (int i = NumberOfPoints - 1; i >= 0; i--)
		{
			AllCoordInputstringNew = AllCoordInputstringNew + splitstringsemicolon[i] + delimiterCharSemicolon;
		}

		NumberOfPoints = splitLastLineCoordsemicolon.size();
		for (int i = 0; i < NumberOfPoints; i++)
		{
			AllCoordInputstringNew = AllCoordInputstringNew + splitLastLineCoordsemicolon[i] + delimiterCharSemicolon;
		}
	}
	else if ((firstAllCoordPoint[1] == lastLinePoint[1]) && (firstAllCoordPoint[2] == lastLinePoint[2]))
	{
		AllCoordInputstringNew = addLineCoord;
		int NumberOfPoints = splitstringsemicolon.size();
		for (int i = 0; i < NumberOfPoints; i++)
		{
			AllCoordInputstringNew = AllCoordInputstringNew + splitstringsemicolon[i] + delimiterCharSemicolon;
		}
	}

	return AllCoordInputstringNew;
}

string ConvertCyrCharToLatin(string str)
{
	string s = "";

	std::map<char, std::string> cyrToLatin =
	{
		{ 'À', "A" },
		{ 'à', "a" },
		{ 'Á', "B" },
		{ 'á', "b" },
		{ 'Â', "V" },
		{ 'â', "v" },
		{ 'Ã', "G" },
		{ 'ã', "g" },
		{ 'Ä', "D" },
		{ 'ä', "d" },
		{ 'Å', "E" },
		{ 'å', "e" },
		{ 'Æ', "Zh" }, 
		{ 'æ', "zh" },
		{ 'Ç', "Z" },
		{ 'ç', "z" },
		{ 'È', "I" },
		{ 'è', "i" },
		{ 'É', "Y" },
		{ 'é', "y" },
		{ 'Ê', "K" },
		{ 'ê', "k" },
		{ 'Ë', "L" },
		{ 'ë', "l" },
		{ 'Ì', "M" },
		{ 'ì', "m" },
		{ 'Í', "N" },
		{ 'í', "n" },
		{ 'Î', "O" },
		{ 'î', "o" },
		{ 'Ï', "P" },
		{ 'ï', "p" },
		{ 'Ð', "R" },
		{ 'ð', "r" },
		{ 'Ñ', "S" },
		{ 'ñ', "s" },
		{ 'Ò', "T" },
		{ 'ò', "t" },
		{ 'Ó', "U" },
		{ 'ó', "u" },
		{ 'Ô', "F" },
		{ 'ô', "f" },
		{ 'Õ', "H" },
		{ 'õ', "h" },
		{ 'Ö', "Ts" }, 
		{ 'ö', "ts" },
		{ '×', "Ch" },
		{ '÷', "ch" },
		{ 'Ø', "Sh" },
		{ 'ø', "sh" },
		{ 'Ù', "Sht" },
		{ 'ù', "sht" },
		{ 'Ú', "A" },
		{ 'ú', "a" },
		{ 'Ü', "Y" },
		{ 'ü', "y" },
		{ 'Þ', "Yu" },
		{ 'þ', "yu" },
		{ 'ß', "Ya" },
		{ 'ÿ', "ya" },
	};
	std::map<char, std::string>::iterator it;

	for (std::string::size_type i = 0; i < str.size(); ++i) {
		it = cyrToLatin.find(str[i]);
		if (it != cyrToLatin.end()){
			s = s + cyrToLatin[str[i]];
		}
		else{
			s = s + str[i];
		}
		
	}
	return s;
	
}



