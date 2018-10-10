#include "bignumber.h"
//print vector
void print_num(vector<int> num)
{
	for (auto n : num)
	{
		cout << n;
	}
	cout << endl;
}

inline void BigNumber::setSign(SIGN _sign)
{
	this->sign = _sign;
}

inline void BigNumber::setSign(int _sign)
{
	if (_sign >= 0)
		this->sign = SIGN::POSITIVE;
	else
		this->sign = SIGN::NEGATIVE;
}

BigNumber::BigNumber()
{
	this->digits = {};
	this->decimalPos = 0;
	setSign(SIGN::POSITIVE);
}

BigNumber::BigNumber(const double number)
{
	stringstream ss;
	ss << setprecision(15) << number;
	BigNumber newNumber = BigNumber(ss.str());
	this->digits = newNumber.digits;
	this->decimalPos = newNumber.decimalPos;
	setSign(newNumber.sign);
}

BigNumber::BigNumber(const int number)
{
	stringstream ss;
	ss << number;
	BigNumber newNumber = BigNumber(ss.str());
	this->digits = newNumber.digits;
	this->decimalPos = newNumber.decimalPos;
	setSign(newNumber.sign);
}

BigNumber::BigNumber(vector<int> intArray)
{
	this->decimalPos = intArray.size();
	this->digits = intArray;
	setSign(SIGN::POSITIVE);
}

BigNumber::BigNumber(char * charArray)
{
	int hasSign = 0;
	if (charArray[0] == '-')
	{
		setSign(SIGN::NEGATIVE);
		hasSign = 1;
	}
	else if (charArray[0] == '+')
	{
		setSign(SIGN::POSITIVE);
		hasSign = 1;
	}
	else
		setSign(SIGN::POSITIVE);

	int charArrayLength = strlen(charArray);
	bool hasDecimal = false;

	// Convert char array into an int vector
	for (int i = hasSign; i < charArrayLength; i++) {
		if (charArray[i] == '.') {
			hasDecimal = true;
			this->decimalPos = i - hasSign;
		}
		else {
			this->digits.push_back(charArray[i] - 48);
		}
	}
	if (!hasDecimal)
		this->decimalPos = this->digits.size();
}

BigNumber::BigNumber(string s)
{
	int hasSign = 0;
	if (s[0] == '-')
	{
		setSign(SIGN::NEGATIVE);
		hasSign = 1;
	}
	else if (s[0] == '+')
	{
		setSign(SIGN::POSITIVE);
		hasSign = 1;
	}
	else
		setSign(SIGN::POSITIVE);

	int charArrayLength = s.size();
	bool hasDecimal = false;
	// Convert char array into an int vector
	for (int i = hasSign; i < charArrayLength; i++) {
		if (s[i] == '.') {
			hasDecimal = true;
			this->decimalPos = i - hasSign;
		}
		else {
			this->digits.push_back(s[i] - 48);
		}
	}
	if (!hasDecimal)
		this->decimalPos = this->digits.size();
}

BigNumber::BigNumber(const BigNumber & bignumber)
{
	this->digits = bignumber.digits;
	this->decimalPos = bignumber.decimalPos;
	setSign(bignumber.sign);
}

int BigNumber::getDecimalNum() const
{
	return this->digits.size() - this->decimalPos;
}

string BigNumber::toString()
{
	string s = "";

	for (int i = 0; i < this->digits.size(); i++) {
		s += char(this->digits[i] + 48);
	}
	if (this->getDecimalNum() > 0) {
		if (this->decimalPos >= 0)
			s.insert(this->decimalPos, ".");
		if (this->decimalPos <= 0)
			s = "0" + s;
	}
	if (this->sign == SIGN::NEGATIVE)
		s = "-" + s;
	return s;
}

BigNumber BigNumber::operator+(const BigNumber & _rightVal)
{
	// make sure left value is larger.
	BigNumber largerVal(*this);
	BigNumber smallerVal(_rightVal);
	alignNumbers(largerVal, smallerVal);
	if (largerVal.digits.size() < smallerVal.digits.size()) { //小数对齐之后，长度大的绝对值大	
		BigNumber temp(largerVal);
		largerVal = smallerVal;
		smallerVal = temp;
	}
	if (largerVal.sign != smallerVal.sign) { //若异号，则以绝对值大的数字符号为准，求差
		smallerVal.setSign(largerVal.sign);
		return largerVal - smallerVal;
	}

	vector<int> leftVal = largerVal.digits;
	vector<int> rightVal = smallerVal.digits;
	int longSize = leftVal.size();
	int shortSize = rightVal.size();

	vector<int> result(longSize + 1, 0);// Define result's size as longSize plus 1

	for (int i = 1; i <= longSize; i++)
	{
		if (i <= shortSize)
			result[i - 1] += leftVal[longSize - i] + rightVal[shortSize - i];
		else
			result[i - 1] += leftVal[longSize - i];

		result[i] += result[i - 1] / 10;
		result[i - 1] %= 10;
	}
	reverse(result.begin(), result.end());
	if (result[0] != 0)
	{
		if (largerVal.decimalPos > -1)
			largerVal.decimalPos++;
	}
	else {
		for (int i = 1; i < result.size(); i++)
			result[i - 1] = result[i];
		result.pop_back();
	}
	BigNumber newResult;
	newResult.digits = result;
	newResult.decimalPos = largerVal.decimalPos;
	newResult.setSign(largerVal.sign);
	return newResult;
}

BigNumber BigNumber::operator-(const BigNumber & _rightVal)
{
	// make sure left value is larger.
	bool isswaped = false;
	BigNumber largerVal(*this);
	BigNumber smallerVal(_rightVal);

	BigNumber newResult;
	newResult.setSign(largerVal.sign);

	alignNumbers(largerVal, smallerVal);
	if (largerVal.sign != smallerVal.sign) { //若异号，符号以左值为准，求和
		smallerVal.setSign(largerVal.sign);
		return largerVal + smallerVal;
	}

	else if (compareOneByOne(largerVal.digits, smallerVal.digits) == -1){
		//如果绝对值左值小于右值，则交换二者，并标记符号变化
		BigNumber temp(largerVal);
		largerVal = smallerVal;
		smallerVal = temp;
		isswaped = true;
	}

	vector<int> leftVal = largerVal.digits;
	vector<int> rightVal = smallerVal.digits;
	int longSize = leftVal.size();
	int shortSize = rightVal.size();
	// Define result's size as longSize
	vector<int> result(longSize, 0);
	for (int i = 1; i <= longSize; i++)
	{
		if (i <= shortSize)
			result[i - 1] += leftVal[longSize - i] - rightVal[shortSize - i];
		else
			result[i - 1] += leftVal[longSize - i];
		if (result[i - 1] < 0 && i < longSize)
		{
			result[i] -= 1;
			result[i - 1] += 10;
		}
		else if (result[i - 1] < 0 && i == longSize) {
			newResult.setSign(-1 * newResult.sign);
			result[i - 1] = -result[i - 1];
		}
	}
	reverse(result.begin(), result.end());

	while (largerVal.decimalPos >= 1 && result[0] == 0) {
		for (int i = 1; i < result.size(); i++)
			result[i - 1] = result[i];
		result.pop_back();
		largerVal.decimalPos--;
	}


	if (isswaped)
		newResult.setSign(-1 * newResult.sign);

	newResult.digits = result;
	newResult.decimalPos = largerVal.decimalPos;
	if (decimalPos == -1)
		deleteFrontZeros(newResult);
	return newResult;
}

BigNumber BigNumber::operator*(const BigNumber & _rightVal)
{
	// make sure left value is larger.
	BigNumber largerVal(*this);
	BigNumber smallerVal(_rightVal);
	alignNumbers(largerVal, smallerVal);
	if (compareOneByOne(largerVal.digits, smallerVal.digits) == -1)	{ //交换左右值，确保左值绝对值最大
		BigNumber temp(largerVal);
		largerVal = smallerVal;
		smallerVal = temp;
	}
	
	vector<int> leftVal = largerVal.digits;
	vector<int> rightVal = smallerVal.digits;
	int longSize = leftVal.size();
	int shortSize = rightVal.size();

	// Define result's size as longSize + shortSize +1
	int resultSize = longSize + shortSize + 1;
	vector<int> result(resultSize, 0);

	for (int i = 0; i < shortSize; i++) {
		for (int j = 0; j < longSize; j++) {
			int curLeft = longSize - j - 1;
			int curRight = shortSize - i - 1;
			result[j + i] += leftVal[curLeft] * rightVal[curRight];
			result[j + i + 1] += result[j + i] / 10;
			result[j + i] %= 10;
		}
	}
	reverse(result.begin(), result.end());
	int decimalPos = largerVal.decimalPos + smallerVal.decimalPos;
	while (decimalPos >= 1 && result[0] == 0) {
		for (int i = 1; i < result.size(); i++)
			result[i - 1] = result[i];
		result.pop_back();
		decimalPos--;
	}

	BigNumber newResult;
	newResult.digits = result;
	newResult.decimalPos = newResult.digits.size() - largerVal.getDecimalNum() - smallerVal.getDecimalNum();
	newResult.setSign(largerVal.sign * smallerVal.sign);
	return newResult;
}

BigNumber BigNumber::operator/(const BigNumber & _rightVal)
{
	if (BigNumber("0") == _rightVal)
	{
		cout << "cannot divided by 0!";
		return BigNumber();
	}
	else if (BigNumber("0") == *this)
	{
		return BigNumber("0");
	}
	else if (*this == _rightVal)
	{
		return BigNumber("1");
	}
	vector<int> leftVal = (*this).digits;
	vector<int> rightVal = _rightVal.digits;
	vector<int> quotient = {}; //商
	BigNumber newResult;
	
	newResult.decimalPos = 0;

	int decimalRight = _rightVal.getDecimalNum();
	int decimalPosLeft = this->decimalPos;

	for (int i = 0; i < decimalRight; i++) {
		decimalPosLeft++;
		if (decimalPosLeft > leftVal.size())
		{
			leftVal.push_back(0);
		}
	}
	while (compareOneByOne(leftVal, rightVal) == -1) {
		leftVal.push_back(0);
	}

	bool isTempZerosPaded = false;
	int current = rightVal.size();
	vector<int> temp(leftVal.begin(), leftVal.begin() + rightVal.size());
	while ((compareOneByOne(temp, rightVal) != 0 || compareOneByOne(vector<int>(1, 0), temp) != 0) &&
		quotient.size() < 40)
	{
		int count = 0;
		while (compareOneByOne(temp, rightVal) < 0)
		{
			if (current >= leftVal.size())
			{
				if (compareOneByOne(vector<int>(1, 0), temp) == 0)
					break;
				else {
					temp.push_back(0);
					isTempZerosPaded = true;
				}
					
			}
			else
				temp.push_back(leftVal[current++]);
			count++;
			if (count > 1) {
				quotient.push_back(0);
			}
			if (current == decimalPosLeft) {
				decimalPosLeft = 0;
				newResult.decimalPos = quotient.size()+1;
			}
		}

		int cur = 0;
		while (compareOneByOne(temp, rightVal) >= 0) {
			temp = (BigNumber(temp) - BigNumber(rightVal)).digits;
			cur++;
		}
		quotient.push_back(cur);
		if (compareOneByOne(vector<int>(1, 0), temp) == 0 && current >= leftVal.size())
			break;
	}
	if (!isTempZerosPaded && this->digits.size() == _rightVal.digits.size())
		newResult.decimalPos = quotient.size();
	newResult.digits = quotient;
	newResult.setSign(this->sign * _rightVal.sign);
	return  newResult;
}

BigNumber BigNumber::operator=(const double number)
{
	return BigNumber(number);
}

bool BigNumber::operator<(const BigNumber & rightVal)
{
	if (compare(*this, rightVal) == -1)
		return true;
	else
		return false;
}

bool BigNumber::operator>(const BigNumber & rightVal)
{
	if (compare(*this, rightVal) == 1)
		return true;
	else
		return false;
}

bool BigNumber::operator==(const BigNumber & rightVal)
{
	if (compare(*this, rightVal) == 0)
		return true;
	else
		return false;
}

BigNumber BigNumber::exectue(BigNumber leftVal, BigNumber rightVal, char operation)
{
	BigNumber res;
	switch (operation) {
	case '+':
		res = leftVal + rightVal; break;
	case '-':
		res = leftVal - rightVal; break;
	case '*':
		res = leftVal * rightVal; break;
	case '/':
		res = leftVal / rightVal; break;
	}
	return res;
}

string BigNumber::exectue(string _leftVal, string _rightVal, char operation)
{
	BigNumber res;
	BigNumber leftVal(_leftVal);
	BigNumber rightVal(_rightVal);
	switch (operation) {
	case '+':
		res = leftVal + rightVal;
	case '-':
		res = leftVal - rightVal;
	case '*':
		res = leftVal * rightVal;
	case '/':
		res = leftVal / rightVal;
	}
	return res.toString();
}

int BigNumber::compare(const BigNumber & _leftVal, const BigNumber & _rightVal)
{
	BigNumber leftVal(_leftVal);
	BigNumber rightVal(_rightVal);
	alignNumbers(leftVal, rightVal);

	if (_leftVal.sign == SIGN::POSITIVE && _rightVal.sign == SIGN::POSITIVE)
	{
		return compareOneByOne(leftVal.digits, rightVal.digits);
	}
	else if (_leftVal.sign == SIGN::NEGATIVE && _rightVal.sign == SIGN::NEGATIVE)
	{
		return -1 * compareOneByOne(leftVal.digits, rightVal.digits);
	}
	else if (_leftVal.sign == SIGN::POSITIVE && _rightVal.sign == SIGN::NEGATIVE)
	{
		BigNumber zero = BigNumber("0");
		BigNumber zero1 = BigNumber("0");
		alignNumbers(leftVal, zero);
		alignNumbers(rightVal, zero1);

		if (compareOneByOne(leftVal.digits, zero.digits) == 0 &&
			compareOneByOne(rightVal.digits, zero1.digits) == 0)
			return 0;
		else
			return 1;
	}
	else if (_leftVal.sign == SIGN::NEGATIVE && _rightVal.sign == SIGN::POSITIVE)
	{
		BigNumber zero = BigNumber("0");
		BigNumber zero1 = BigNumber("0");
		alignNumbers(leftVal, zero);
		alignNumbers(rightVal, zero1);

		if (compareOneByOne(leftVal.digits, zero.digits) == 0 &&
			compareOneByOne(rightVal.digits, zero1.digits) == 0)
			return 0;
		else
			return -1;
	}

}

int BigNumber::compareOneByOne(const vector<int>& _leftVal, const vector<int>& _rightVal)
{
	vector<int> leftVal(_leftVal);
	vector<int> rightVal(_rightVal);

	deleteFrontZeros(leftVal);
	deleteFrontZeros(rightVal);

	int leftSize = leftVal.size();
	int rightSize = rightVal.size();
	if (leftSize < rightSize)
		return -1;
	else if (leftSize > rightSize)
		return 1;
	else if (leftSize == 0 && rightSize == 0)
		return 0;
	else {
		for (int i = 0; i < leftSize; i++) {
			if (leftVal[i] < rightVal[i])
				return -1;
			else if (leftVal[i] > rightVal[i])
				return 1;
		}
	}
	return 0;
}

void BigNumber::alignNumbers(BigNumber & leftVal, BigNumber & rightVal)
{
	int leftDecimalSize = leftVal.getDecimalNum();
	int rightDecimalSize = rightVal.getDecimalNum();
	if (leftDecimalSize == rightDecimalSize)
		return;
	BigNumber* pSmallerVal = &rightVal;
	if (leftDecimalSize < rightDecimalSize)
	{
		pSmallerVal = &leftVal;
	}
	for (int i = 0; i < abs(leftDecimalSize - rightDecimalSize); i++)
	{
		pSmallerVal->digits.push_back(0);
	}

}

void BigNumber::deleteFrontZeros(BigNumber & bignumber)
{
	deleteFrontZeros(bignumber.digits);
}

void BigNumber::deleteFrontZeros(vector<int>& num)
{
	// find the start pos of zeros
	int zeroPos = -1;
	for (int i = 0; i < num.size(); i++)
	{
		if (num[i] != 0)
			break;
		else
			zeroPos = i;
	}
	// move back to front with the step of zeroPos + 1
	for (int i = 0; zeroPos >= 0 && i < num.size() - zeroPos - 1; i++)
		num[i] = num[i + zeroPos + 1];
	for (int i = 0; i <= zeroPos; i++)
		num.pop_back();
}

void BigNumber::dropExtralZeros(BigNumber bignumber)
{
	while (bignumber.decimalPos >= 1 && bignumber.digits[0] == 0) {
		for (int i = 1; i < bignumber.digits.size(); i++)
			bignumber.digits[i - 1] = bignumber.digits[i];
		bignumber.digits.pop_back();
		bignumber.decimalPos--;
	}
}

void BigNumber::print()
{
	cout << this->toString();
}
