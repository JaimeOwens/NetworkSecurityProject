#include "bitmartix.h"



bitmartix::bitmartix()
{
}

bitmartix::bitmartix(int a, int b)
{
	this->raw = a;
	this->col = b;
	this->elem = new std::bitset<1> * [a];
	for (int i = 0; i < a; i++) {
		this->elem[i] = new std::bitset<1>[b];
	}
}

int bitmartix::craw()
{
	return this->raw;
}

int bitmartix::ccol()
{
	return this->col;
}

std::bitset<1> bitmartix::getelem(int i, int j)
{
	return this->elem[i][j];
}

void bitmartix::set(int i, int j, bool temp)
{
	this->elem[i][j] = temp;
}


void bitmartix::set(int i, int j, std::bitset<1> temp)
{
	this->elem[i][j] = temp;
}

std::string bitmartix::makebits()
{
	std::string temp = "";
	for (int i = 0; i < this->raw; i++)
		for (int j = 0; j < this->col; j++)
			temp += this->elem[i][j].to_string();
	return temp;
}

void bitmartix::setrow(std::string rowval, int row)
{
	for (int i = 0; i<rowval.size(); i++)
		if (rowval[i] == '0')
			this->elem[row][i] = 0;
		else
		{
			this->elem[row][i] = 1;
		}
}

void bitmartix::setcol(std::string colstr, int col)
{
	for (int i = 0; i < colstr.size(); i++)
		if (colstr[i] == '0')
			this->elem[this->raw - 1 - i][col] = 0;
		else
		{
			this->elem[this->raw - 1 - i][col] = 1;
		}

}

bitmartix bitmartix::operator*(const bitmartix& b)
{
	bitmartix temp(this->raw, b.col);
	int n = this->col;
	std::bitset<1>elems;
	for (int i = 0; i < this->raw; i++)
		for (int j = 0; j < b.col; j++) {
			elems = this->elem[i][0] & b.elem[0][j];
			for (int r = 1; r < n; r++) {
				std::bitset<1> te = this->elem[i][r] & b.elem[r][j];
				elems ^= te;
			}
			temp.set(i, j, elems);
			elems.reset();
		}
	return temp;
}

bitmartix bitmartix::operator+(const bitmartix& b)
{
	bitmartix temp(this->raw, this->col);
	for (int i = 0; i < this->raw; i++)
		for (int j = 0; j < this->col; j++)
			temp.set(i, j, this->elem[i][j] ^ b.elem[i][j]);
	return temp;
}

std::string bitmartix::to_string()
{
	int count = this->col * this->raw / 8;//count of byte
	char* str = new char[count + 1];
	std::string a = this->makebits();
	for (int i = 0; i < count; i++) {
		std::bitset<8> temp;
		for (int j = 0; j < 8; j++) {
			temp.set(j, a[i * 8 + j]);
		}
		str[i] = (char)temp.to_ulong();
	}
	std::string s(str);
	delete[]str;
	return s;
}

std::string bitmartix::to_bits()
{
	std::string a = "";
	for (int i = 0; i < this->raw; i++) {
		for (int j = 0; j < this->col; j++) {
			if (this->elem[i][j] == 0)
				a += "0";
			else
			{
				a += "1";
			}

		}

	}
	return a;
}

bitmartix::~bitmartix()
{
	for (int i = 0; i < this->raw; i++)
		delete[] this->elem[i];
	delete []this->elem;
}

std::ostream& operator<<(std::ostream& out, const bitmartix& a)
{
	std::string temp = "";
	for (int i = 0; i < a.raw; i++) {
		for (int j = 0; j < a.col; j++) {
			temp += a.elem[i][j].to_string();
			temp += " ";
		}
		temp += "\n";
	}
	out << temp;
	return out;
}
