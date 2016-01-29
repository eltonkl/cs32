#include <string>
#include <stack>
#include <iostream>

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
	stack<Coord> coordinates;
	coordinates.push(Coord(sr, sc));
	maze[sr][sc] = '/';

	while (!coordinates.empty()) 
	{
		Coord current(coordinates.top());
		coordinates.pop();
		if (current.r() == er && current.c() == ec)
			return true;

		if (current.r() > 0 && maze[current.r() - 1][current.c()] == '.')
		{
			coordinates.push(Coord(current.r() - 1, current.c()));
			maze[current.r() - 1][current.c()] = '/';
		}
		if (current.c() + 1 < nCols && maze[current.r()][current.c() + 1] == '.')
		{
			coordinates.push(Coord(current.r(), current.c() + 1));
			maze[current.r()][current.c() + 1] = '/';
		}
		if (current.r() + 1 < nRows && maze[current.r() + 1][current.c()] == '.')
		{
			coordinates.push(Coord(current.r() + 1, current.c()));
			maze[current.r() + 1][current.c()] = '/';
		}
		if (current.c() > 0 && maze[current.r()][current.c() - 1] == '.')
		{
			coordinates.push(Coord(current.r(), current.c() - 1));
			maze[current.r()][current.c() - 1] = '/';
		}
	}
	return false;
}