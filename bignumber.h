#ifndef BIGNUMBER_H
#define BIGNUMBER_H
/**
BigNumber is a class to deal with computation between big numbers.

@author: geo-like
@version: 0.1 2018/09/12
@copyright: Ke Li
*/
#include<sstream>
#include<iomanip>
#include<iostream>
#include<vector>
#include<string>
using namespace std;
/**
@variables
	digits: the numbers in a vector.
	decimalPos: the position of Decimal point.
	sign: the sign to mark wheather the number is positive or negative.
*/
class BigNumber
{

private:
	vector<int> digits;
	int decimalPos;
	enum SIGN
	{
		NEGATIVE = -1,
		POSITIVE = 1
	} sign;

	inline void setSign(SIGN _sign);
	inline void setSign(int _sign);
public:
	BigNumber();
	BigNumber(const double number);
	BigNumber(const int number);
	BigNumber(char* charArray);
	BigNumber(vector<int> intArray);
	BigNumber(string s);
	BigNumber(const BigNumber& bignumber);

	int getDecimalNum() const;
	string toString();

	BigNumber operator + (const BigNumber& rightVal);
	BigNumber operator - (const BigNumber& rightVal);
	BigNumber operator * (const BigNumber& rightVal);
	BigNumber operator / (const BigNumber& rightVal);
	BigNumber operator = (const double number);

	bool operator < (const BigNumber& rightVal);
	bool operator > (const BigNumber& rightVal);
	bool operator == (const BigNumber& rightVal);

	static BigNumber exectue(BigNumber leftVal, BigNumber rightVal, char operation);
	static string exectue(string _leftVal, string _rightVal, char operation);
	int compare(const BigNumber& leftVal, const BigNumber& rightVal);
	static int compareOneByOne(const vector<int>& leftVal, const vector<int>& rightVal);
	static void alignNumbers(BigNumber& leftVal, BigNumber& rightVal);
	static void deleteFrontZeros(BigNumber& bignumber);
	static void deleteFrontZeros(vector<int>& num);
	void dropExtralZeros(BigNumber bignumber);
	void print();
};
#endif
