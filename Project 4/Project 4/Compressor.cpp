#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>
using namespace std;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
	unsigned int capacity = 16384;
	if ((s.size()/2 + 512) < capacity)
		capacity = s.size()/2 + 512;
	HashTable<string, unsigned short> H(capacity * 2, capacity);

	for (int i = 0; i < 256; i++)
	{
		string c(1, i);
		H.set(c, i, true);
	}
	
	int nextFreeID = 256;
	string runSoFar;
	vector<unsigned short> V;
	for (unsigned int i = 0; i < s.size(); i++)
	{
		string expandedRun = runSoFar + s[i];
		unsigned short us;
		if (H.get(expandedRun, us))
		{
			runSoFar = expandedRun;
			continue;
		}
		unsigned short x;
		H.get(runSoFar, x);
		V.push_back(x);
		H.touch(runSoFar);
		runSoFar = "";
		unsigned short cv;
		H.get(string(1, s[i]), cv);
		V.push_back(cv);
		if (!H.isFull())
		{
			H.set(expandedRun, nextFreeID, false);
			nextFreeID++;
		}
		else
		{
			unsigned short disc;
			string temp;
			H.discard(temp, disc);
			H.set(expandedRun, disc, false);
		}
	}
	if (!runSoFar.empty())
	{
		unsigned short x;
		H.get(runSoFar, x);
		V.push_back(x);
	}

	V.push_back(capacity);
	numbers = move(V);
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
	if (numbers.empty())
		return false;
	int last = numbers.size() - 1;
	HashTable<unsigned short, string> H(numbers[last] * 2, numbers[last]);
	for (int i = 0; i < 256; i++)
	{
		string c(1, i);
		H.set(i, c, true);
	}
	int nextFreeID = 256;
	string runSoFar;
	string output;

	for (unsigned int i = 0; i < numbers.size() - 1; i++)
	{
		unsigned short us = numbers[i];
		if (us <= 255)
		{
			string a;
			H.get(us, a);
			output += a;

			if (runSoFar.empty())
				runSoFar += a;
			else
			{
				string expandedRun = runSoFar + a;
				if (!H.isFull())
				{
					H.set(nextFreeID, expandedRun, false);
					nextFreeID++;
				}
				else
				{
					unsigned short key;
					string temp;
					H.discard(key, temp);
					H.set(key, expandedRun, false);
				}
				runSoFar = "";
			}
		}
		else
		{
			string S;
			if (!H.get(us, S))
				return false;
			H.touch(us);
			output += S;
			runSoFar = S;
		}
	}
	s = move(output);
	return true;
}
