#pragma once
#ifdef ALONE
#include"lib.h"
#endif // DEBUG

class bitmartix
{
private:
	std::bitset<1> ** elem;
	int col;
	int raw;
public:
	bitmartix();

	bitmartix(int a, int b);

	int craw();

	int ccol();

	std::bitset<1> getelem(int i, int j);

	void set(int i, int j, bool temp);

	void set(int i, int j, std::bitset<1>temp);

	std::string makebits();

	void setrow(std::string rowval, int row);
	void setcol(std::string colstr, int col);
	friend std::ostream& operator<<(std::ostream& out, const bitmartix& a);
	bitmartix operator *(const bitmartix& b);
	bitmartix operator +(const bitmartix& b);
	std::string to_string();
	std::string to_bits();
	~bitmartix();

};

