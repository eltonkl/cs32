#include <iostream>

#include "History.h"
#include "globals.h"

using namespace std;

History::History(int nRows, int nCols) :
_rows(nRows), _cols(nCols)
{
	_grid = new int[nRows * nCols]();
}

History::~History()
{
	delete[] _grid;
}

bool History::record(int r, int c)
{
	if (r < 1 || r > _rows)
		return false;
	if (c < 1 || c > _cols)
		return false;

	_grid[(r - 1) * _cols + (c - 1)]++;
	return true;
}

void History::display() const
{
	char grid[MAXROWS][MAXCOLS];

	for (int r = 0; r < _rows; r++)
	{
		for (int c = 0; c < _cols; c++)
		{
			int current = _grid[r * _cols + c];

			if (current == 0)
				grid[r][c] = '.';
			else if (current >= 1 && current <= 26)
				grid[r][c] = 'A' + (current - 1);
			else if (current > 26)
				grid[r][c] = 'Z';
		}
	}

	clearScreen();
	for (int r = 0; r < _rows; r++)
	{
		for (int c = 0; c < _cols; c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;
}