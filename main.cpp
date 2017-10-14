#include<vector>
#include<iostream>
#include <string>

using namespace std;

class BigInt {
public:
	vector<long long> digits;
	int size;  //length of digits vector
	int signNum = 1;  //positive number

	static const int w = 700;
	int b = 10;
	int zero[w + 1];  // w = 400

	//this constructor converts input string into a vector of integers
	BigInt(string n) {
		char sign = n.at(0);

		if (sign == '-') {
			signNum = -1;
			n = n.substr(1);
		}

		int length = n.length();
		int rem = length % 9;

		//put string in the digits vector in which each element consists of a 9 digit integer
		if (rem != 0)
			digits.push_back(atoi(n.substr(0, rem).c_str()));

		for (int i = rem; i < length; i = i + 9)
			digits.push_back(atoi(n.substr(i, 9).c_str()));

		size = digits.size();

	}

	BigInt(vector<long long> temp_digits) {
		digits = temp_digits;
		size = digits.size();
	}

	BigInt add(BigInt & other) {
		BigInt thisObject = *this;
		BigInt result("0");
		if ((signNum == 1) && (other.signNum == 1)) {
			result = thisObject.addNum(other);
			result.signNum = 1;
			return result;
		} else if ((signNum == 1) && (other.signNum == -1)) {
			if (thisObject.compare(other) == 1) {
				result = thisObject.subNum(other);
				result.signNum = 1;
				return result;
			} else if (thisObject.compare(other) == -1) {
				result = other.subNum(thisObject);
				result.signNum = -1;
				return result;
			} else {
				return BigInt("0");
			}
		} else if ((signNum == -1) && (other.signNum == 1)) {
			if (thisObject.compare(other) == 1) {
				result = thisObject.subNum(other);
				result.signNum = -1;
				return result;
			} else if (thisObject.compare(other) == -1) {
				result = other.subNum(thisObject);
				result.signNum = 1;
				return result;
			} else {
				return BigInt("0");
			}
		} else if ((signNum == -1) && (other.signNum == -1)) {
			result = thisObject.addNum(other);
			result.signNum = -1;
			return result;
		}

	}

	BigInt addNum(BigInt other) {
		// if both numbers are +ve or -ve
		int tempSize = min(size, other.size);
		int aIndex = size - 1;
		int bIndex = other.size - 1;
		long long carry = 0;
		long long digit;
		vector<long long> result;

		for (int i = 0; i < tempSize; i++) {
			digit = digits[aIndex] + other.digits[bIndex] + carry;
			carry = digit / 1000000000;
			result.insert(result.begin(), digit % 1000000000);
			aIndex--;
			bIndex--;
		}
		if (aIndex != -1) {
			for (int s = aIndex; s >= 0; s--) {
				digit = digits[s] + carry;
				carry = digit / 1000000000;
				result.insert(result.begin(), digit % 1000000000);
			}
		} else if (bIndex != -1) {

			for (int s = bIndex; s >= 0; s--) {
				digit = other.digits[s] + carry;
				carry = digit / 1000000000;
				result.insert(result.begin(), digit % 1000000000);
			}
		}

		if (carry != 0)
			result.insert(result.begin(), carry);

		BigInt finalResult = BigInt(result);
		return finalResult;

	}
	BigInt sub(BigInt other)
	{
		BigInt thisObject = *this;
		BigInt result("0");
		if ((signNum == 1) && (other.signNum == 1))
		{
			if (thisObject.compare(other) == 1)
			{
				result = thisObject.subNum(other);
				result.signNum = 1;
				return result;
			}
			else if (thisObject.compare(other) == -1)
			{
				result = other.subNum(thisObject);
				result.signNum = -1;
				return result;
			}
			else
				return BigInt("0");
		}
		else if ((signNum == 1) && (other.signNum == -1))
		{
			result = thisObject.addNum(other);
			result.signNum = 1;
			return result;
		}
		else if ((signNum == -1) && (other.signNum == 1))
		{
			result = thisObject.addNum(other);
			result.signNum = -1;
			return result;
		}
		else if ((signNum == -1) && (other.signNum == -1))
		{
			if (thisObject.compare(other) == 1)
			{
				result = thisObject.subNum(other);
				result.signNum = -1;
				return result;
			}
			else if (thisObject.compare(other) == -1)
			{
				result = other.subNum(thisObject);
				result.signNum = 1;
				return result;
			}
			else
			{
				return BigInt("0");
			}
		}
	}
	//subtracts two numbers , the first num is the bigger , ignores sign of both numbers
	BigInt subNum(BigInt other) {
		int first_len = size;
		int second_len = other.size;
		vector<long long> result;
		int zeroCount = first_len - second_len;

		long long carry = 0;

		for (int i = first_len - 1; i >= 0; i--) {
			long long temp = digits[i];

			// Subtract the appropriate digit if necessary/
			if (i - zeroCount >= 0) {
				long long temp2 = other.digits[i - zeroCount];
				temp = temp - temp2;
			}

			// Subtract the carry from the last iteration.
			temp -= carry;

			carry = 0; // Reset for future calculations.

			// If temp is too small, you need to borrow.
			if (temp < 0) {
				temp = temp + 1000000000;
				carry = 1;
			}

			// Set the current digit - carry has already been set.
			result.insert(result.begin(), temp);
		}
		return BigInt(result);
	}


	BigInt multiply(BigInt & other)
	{

		/*if (zero(digits) || zero(other.digits))
		return  BigInt("0");*/

		long long digit;
		long long carry = 0;
		vector<long long> result;
		BigInt finalResult("0");
		int rightZero = 0;

		for (int i = other.size - 1; i >= 0; i--)
		{
			for (int j = size - 1; j >= 0; j--)
			{

				digit = other.digits[i] * digits[j] + carry;
				carry = digit / 1000000000;

				result.insert(result.begin(), digit % 1000000000);
			}

			if (carry != 0)
				result.insert(result.begin(), carry);

			for (int s = 0; s < rightZero; s++)
				result.push_back(0);

			rightZero++;

			BigInt temp = BigInt(result);

			finalResult.digits = finalResult.add(temp).digits;
			finalResult.size = finalResult.digits.size();
			result.clear();
			carry = 0;
		}
		if (signNum != other.signNum)
			finalResult.signNum = -1;

		return finalResult;
	}


	int compare(BigInt & g) {
		int aIndex = 0;
		int bIndex = 0;

		while (aIndex < size) {
			if (digits[aIndex] == 0)
				aIndex++;
			else
				break;
		}
		while (bIndex < g.size) {
			if (g.digits[bIndex] == 0)
				bIndex++;
			else
				break;
		}
		int aActualSize = size - aIndex;
		int bActualSize = g.size - bIndex;

		if (aActualSize > bActualSize)
			return 1;
		else if (aActualSize < bActualSize)
			return -1;
		else {
			for (int s = 0; s < aActualSize; s++) {
				int a1 = digits[s + aIndex];
				int a2 = g.digits[s + bIndex];
				if (a1 > a2)
					return 1;
				else if (a1 < a2)
					return -1;
			}
		}

		//both are equal
		return 0;
	}

	string removeZeros(string str) {

		int index = 0;

		while(index < str.size())
		{
			if(str.at(index) == '0')
				index++;

			else if(str.at(index) == '-')
				index++;

			else
				break;
		}

		if(index == str.size())
			str = "0";
		else if(signNum == 1)
			str = str.substr(index);
		else if(signNum == -1)
			str = '-' + str.substr(index);

		return str;

	}

	//converts bigInt to string, it is done only one at the end.
	string toString() {
		string result = "";
		if (signNum == -1)
			result = "-";

		for (int i = 0; i < size; i++) {
			string temp = to_string(digits[i]);
			while (temp.size() != 9) {
				temp = "0" + temp;
			}
			result = result + temp;
		}
		result = removeZeros(result);
		return result;
	}

};

int main() {

	BigInt p1("111");
	BigInt p2("222");

	BigInt addResult = p1.add(p2);
	BigInt subResult = p1.sub(p2);
	BigInt mulResult = p1.multiply(p2);

	cout<<"p1 + p2 = "<< addResult.toString()<<"\n";
	cout<<"p1 - p2 = "<< subResult.toString()<<"\n";
	cout<<"p1 * p2 = "<< mulResult.toString()<<"\n";

}

