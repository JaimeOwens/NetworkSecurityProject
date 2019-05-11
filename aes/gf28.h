#pragma once
#ifdef ALONE
#include"lib.h"
#endif // ALONE


class gf28
{
private:

	std::bitset<16> elem;

public:
	gf28(char t);

	gf28();

	gf28(std::bitset<16>a);

	gf28 operator * (const gf28& b);

	friend std::ostream& operator<<(std::ostream& out, const gf28& a);

	gf28  operator +(const gf28& b);

	gf28& operator = (const gf28 b);

	gf28& operator = (const int& b);

	gf28& operator = (const std::string& b);

	gf28& operator = (const char& b);

	gf28& operator = (const unsigned char& b);

	const gf28  operator !();

	unsigned int getval();

	std::string getstrval();

	~gf28();


};

