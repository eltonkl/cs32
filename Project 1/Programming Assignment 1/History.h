#ifndef HISTORY_H
#define HISTORY_H

class History
{
public:
	History(int nRows, int nCols);
	~History();
	bool record(int r, int c);
	void display() const;

private:
	int _rows;
	int _cols;
	int *_grid;
};

#endif