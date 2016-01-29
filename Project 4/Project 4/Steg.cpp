#include "provided.h"
#include <string>
#include <vector>
using namespace std;

//str must not have a '\n' at the end of it
void stripSpacesAndTabs(string& str, bool stripCR)
{
	if (!str.empty())
	{
		//For whatever strange reason the last line could have CR without LF, 
		//so stripCR is used to indicate if the CR should be stripped
		if (stripCR && str[str.size() - 1] == '\r')
			str.erase(str.size() - 1);
		int charsToErase = 0;
		for (int j = str.size() - 1; j >= 0; j--)
		{
			if (str[j] == '\t' || str[j] == ' ')
				charsToErase++;
			else
				break;
		}
		str.erase(str.size() - charsToErase);
	}
}

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
	if (hostIn.empty())
		return false;

	hostOut.clear();
	vector<string> lines; //There's more efficient ways to write this function than use a vector, but this works and I'm not being paid for it
	unsigned int numLines = 0;

	string line;
	unsigned int i = 0;	
	while (i < hostIn.size())
	{
		if (hostIn[i] == '\n')
		{
			stripSpacesAndTabs(line, true);
			lines.push_back(line);
			line.clear();
			numLines++;
		}
		else if (i == (hostIn.size() - 1)) //The last line of hostIn might not have a \n
		{
			line += hostIn[i];
			stripSpacesAndTabs(line, false);
			lines.push_back(line);
			numLines++;
		}
		else
			line += hostIn[i];
		i++;
	}

	vector<unsigned short> compressed;
	Compressor::compress(msg, compressed);
	string encoded = BinaryConverter::encode(compressed);

	int firstSubstrSize = (encoded.size() / numLines) + 1;
	int secondSubstrSize = encoded.size() / numLines;
	size_t pos = 0;
	for (unsigned int i = 0; i < numLines; i++)
	{
		hostOut += lines[i];
		if (i < encoded.size() % numLines)
		{
			hostOut += encoded.substr(pos, firstSubstrSize);
			pos += firstSubstrSize;
		}
		else
		{
			hostOut += encoded.substr(pos, secondSubstrSize);
			pos += secondSubstrSize;
		}
		hostOut += '\n';
	}
	return true;
}

//str must not have a '\n' at the end of it
void keepSpacesAndTabs(string& str)
{
	if (!str.empty())
	{
		if (str[str.size() - 1] == '\r')
			str.erase(str.size() - 1);
		int charsToKeep = 0;
		for (int j = str.size() - 1; j >= 0; j--)
		{
			if (str[j] == '\t' || str[j] == ' ')
				charsToKeep++;
			else
				break;
		}
		str = str.substr(str.size() - charsToKeep);
	}
}

bool Steg::reveal(const string& hostIn, string& msg) 
{
	if (hostIn.empty())
		return false;

	string encoded;
	string line;
	unsigned int i = 0;
	while (i < hostIn.size())
	{
		if (hostIn[i] == '\n')
		{
			keepSpacesAndTabs(line);
			encoded += line;
			line.clear();
		}
		else if (i == (hostIn.size() - 1))
		{
			line += hostIn[i];
			keepSpacesAndTabs(line);
			encoded += line;
			line.clear();
		}
		else
			line += hostIn[i];
		i++;
	}

	vector<unsigned short> message;
	if (!BinaryConverter::decode(encoded, message))
		return false;
	if (!Compressor::decompress(message, msg))
		return false;

	return true;
}
