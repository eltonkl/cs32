#include "provided.h"
#include <string>
#include <vector>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
	string encoded;
	for (vector<unsigned short>::const_iterator it = numbers.begin(); it != numbers.end(); it++)
		encoded.append(convertNumberToBitString(*it));
	for (unsigned int i = 0; i < encoded.size(); i++)
	{
		if (encoded[i] == '0')
			encoded[i] = ' ';
		else
			encoded[i] = '\t';
	}
	return encoded;
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{
	if ((bitString.size() % 16) != 0)
		return false;
	numbers.clear();
	for (unsigned int i = 0; i < bitString.size(); i += 16)
	{
		string bits = bitString.substr(i, 16);
		for (int j = 0; j < 16; j++)
		{
			if (bits[j] == ' ')
				bits[j] = '0';
			else if (bits[j] == '\t')
				bits[j] = '1';
			else
				return false;
		}
		unsigned short number;
		if (!convertBitStringToNumber(bits, number))
			return false;
		numbers.push_back(number);
	}
	return true;
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
