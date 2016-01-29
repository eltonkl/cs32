#include "HashTable.h"
#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include "provided.h"
#include "http.h"

using namespace std;

unsigned int computeHash(int key)
{
	return key;
}

unsigned int computeHash(int* a)
{
	return 1;
}

int main()
{
	HashTable<int*, int> aaa(5, 5);
	aaa.touch(nullptr);

	HashTable<int, int> swag(5, 5);
	int x = -1, y = -1;
	assert(!swag.isFull());
	assert(!swag.discard(x, y) && x == -1 && y == -1);
	assert(swag.set(3, 3, true));
	assert(!swag.discard(x, y) && x == -1 && y == -1);
	assert(swag.set(0, 0, false));
	assert(swag.set(0, 1, false));
	assert(swag.discard(x, y) && x == 0 && y == 1);
	x = -1, y = -1;
	assert(!swag.discard(x, y) && x == -1 && y == -1);
	x = 0;
	assert(swag.set(0, 5, false));
	assert(swag.get(x, y) && y == 5);
	assert(swag.set(4, 5, false));
	assert(swag.set(5, 6, false));
	assert(swag.set(6, 7, false));
	assert(!swag.set(7, 8, false));
	assert(swag.isFull());
	assert(swag.touch(0));
	assert(swag.discard(x, y) && x == 4 && y == 5);
	assert(swag.set(10, 25, true));
	assert(!swag.set(15, 25, false));
	assert(!swag.set(15, 25, false));
	assert(swag.set(6, 123, true));
	assert(swag.touch(6));
	assert(swag.discard(x, y) && x ==5 && y == 6);

	HashTable<string, string> motherfucker(10, 250);
	for (int i = 0; i < 250; i++)
	{
		ostringstream a, b;
		a << i;
		b << i * i * i;
		motherfucker.set(a.str(), b.str(), false);
	}
	assert(motherfucker.isFull());
	cout << "RETRIEVING VALUES FROM HASHTABLE, FROM 0 TO 249" << endl;
	cout << "VALUES ARE THE KEY TO THE THIRD POWER" << endl;
	for (int i = 0; i < 250; i++)
	{
		ostringstream a;
		a << i;
		string b;
		motherfucker.get(a.str(), b);
		cout << a.str() << ", " <<  b << endl;
	}
	for (int i = 249; i >= 0; i--)
	{
		ostringstream a;
		a << i;
		motherfucker.touch(a.str());
	}

	cout << "PRINTING KEY VALUE PAIRS IN REVERSE ORDER" << endl;
	for (int i = 0; i < 250; i++)
	{
		string a, b;
		motherfucker.discard(a, b);
		cout << a << ", " << b << endl;
	}

	cout << "IF THE ORDER ISN'T FROM 249 to 0 YOU FUCKED UP" << endl;

	HashTable<string, int> nameToAge(100, 200);
	nameToAge.set("Carey", 43);
	nameToAge.set("David", 97);
	nameToAge.set("Timothy", 43, true);
	nameToAge.set("Ivan", 28);
	nameToAge.set("Sally", 22);
	nameToAge.set("David", 55);
	nameToAge.touch("Carey");
	string a;
	int b;
	assert(nameToAge.discard(a, b) && a == "Ivan" && b == 28);
	assert(nameToAge.discard(a, b) && a == "Sally" && b == 22);

	string bitString = BinaryConverter::encode({ 1, 2, 3 });
	vector<unsigned short> numbers;
	BinaryConverter::decode(bitString, numbers);
	assert(numbers[0] == 1 && numbers[1] == 2 && numbers[2] == 3);

	string swagg("AAAAAAAAAB");
	vector<unsigned short> v;
	Compressor::compress(swagg, v);
	
	vector<unsigned short> V = { 65, 65, 256, 65, 65, 65, 256, 66, 257 };
	string asdf;
	Compressor::decompress(V, asdf);
	assert(swagg == asdf);

	string AAA;
	vector<unsigned short> ABC;
	Compressor::compress(AAA, ABC);

	string BBB;
	Compressor::compress(BBB, ABC);
	assert(AAA == BBB);

	string ASDF = "ASDF   \t\nBBBCCDEFG\r\nAS < fawe> \n fawueoijwaeoi <HHTMRW> \n\n\n\n GLGLGL \r\n";
	string MSG = "monosodium glutamate";
	string INEEDANADULT;
	string THEEND;
	Steg::hide(ASDF, MSG, INEEDANADULT);
	Steg::reveal(INEEDANADULT, THEEND);
	assert(MSG == THEEND);

	string SMALLBERG = R"AAA(<html>   
Q -QQ - 
BBB---   

GG 
BBB 
-
DDD
EEE </html>   )AAA";
	string NACHENBERG = "what in the world is going on";
	string BERGBERG;
	assert(Steg::hide(SMALLBERG, NACHENBERG, BERGBERG));
	string finals_week;
	assert(Steg::reveal(BERGBERG, finals_week));
	assert(finals_week == NACHENBERG);
	assert(!Steg::reveal(BERGBERG + ' ', finals_week));

	string result;
	assert(WebSteg::hideMessageInPage("http://google.com", "SMALLBERG IS WATCHING", result));
	string lesurt;
	HTTP().set("http://google.com", result);
	assert(WebSteg::revealMessageInPage("http://google.com", lesurt) && lesurt == "SMALLBERG IS WATCHING");
	{
		string page = "<html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ";
		string msg = "Hello World!";
		string output;
		Steg::hide(page, msg, output);

	}
	return 0;
}