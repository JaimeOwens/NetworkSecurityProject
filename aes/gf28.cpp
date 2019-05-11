#include "gf28.h"
gf28::gf28(char temp)
{
	for (int i = 0; i < 8; i++) {
		bool t = temp & 0x01;
		this->elem.set(i, t);
		temp = temp >> 1;
	}
	for (int i = 8; i < 16; i++)
		this->elem.set(i, 0);
}

gf28::gf28()
{
	this->elem.reset();
}

gf28::gf28(std::bitset<16> a)
{
	this->elem = a;
}

gf28 gf28::operator*(const gf28& b)
{
	std::bitset<16> base(std::string("0000000100011011"));
	std::bitset<16> temp;
	temp.reset();
	for (int i = 0; i < 16; i++) {
		if (b.elem[i] == 1) {

			temp ^= (this->elem << i);
		}
	}
	for (int i = 15; i >= 8; i--) {
		if (temp[i] == 1) {
			temp = temp ^ (base << (i - 8));
		}
	}
	return gf28(temp);
}

gf28 gf28::operator+(const gf28& b)
{
	std::bitset<16> temp;
	temp = this->elem ^ b.elem;
	return gf28(temp);
}

gf28& gf28::operator=(const gf28 b)
{
	for (int i = 0; i < 16; i++)
		this->elem = b.elem;
	return *this;
}

gf28& gf28::operator=(const int& b)
{
	this->elem = b;
	return *this;
}

gf28& gf28::operator=(const std::string& b)
{
	std::string temp1 = b;

	for (int i = 0; i < 4; i++) 
	{
		char a = temp1[i];
		temp1[i] = temp1[7 - i];
		temp1[7 - i] = a;
	}
	std::bitset<16> temp(temp1);
	this->elem = temp;
	return *this;
}

gf28& gf28::operator=(const char& b)
{
	this->elem = b;
	return *this;
}

gf28& gf28::operator=(const unsigned char& b)
{
	this->elem = b;
	return *this;
}

const gf28 gf28::operator!()
{
	gf28 x(this->elem);
	gf28 temp = x;
	for (int i = 0; i < 253; i++)
		x = x * temp;
	return x;
}

unsigned int gf28::getval()
{
	return this->elem.to_ulong();
}


std::string gf28::getstrval()
{
	return this->elem.to_string().substr(8, 16);
}

gf28::~gf28()
{
	this->elem.reset();
}

std::ostream& operator<<(std::ostream& out, const gf28& a)
{
	out << a.elem;
	return out;
}
