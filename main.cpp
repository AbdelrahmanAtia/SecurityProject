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


	// division is implemented using donald knuth algorithm
	BigInt divide(BigInt & other)
	{
		BigInt divisionResult = divide(other, "q");
		if (signNum != other.signNum)
			divisionResult.signNum = -1;
		return divisionResult;
	}
	BigInt divide(BigInt & other, string str)
	{
		int  x[w + 1];
		fillArrayX(x, digits);
		int y[w + 1];
		fillArrayY(y, other.digits);

		int q[w + 1];
		int r[w + 1];

		fill_n(q, w + 1, 0);
		fill_n(r, w + 1, 0);

		division(x, y, q, r);

		string quotient = toString(q);
		string remainder = toString(r);


		if (str.compare("q") == 0)
			return  BigInt(quotient);
		else
			return  BigInt(remainder);

	}



	void fillArrayX(int x[], vector <long long> v)
	{
		int counter = 0;
		for (int s = v.size() - 1; s >= 0; s--)
		{
			string val = to_string(v[s]);
			while (val.size() < 9)
			{
				val = '0' + val;
			}
			x[counter] = val.at(8) - '0';   counter++;
			x[counter] = val.at(7) - '0';   counter++;
			x[counter] = val.at(6) - '0';   counter++;
			x[counter] = val.at(5) - '0';   counter++;
			x[counter] = val.at(4) - '0';   counter++;
			x[counter] = val.at(3) - '0';   counter++;
			x[counter] = val.at(2) - '0';   counter++;
			x[counter] = val.at(1) - '0';   counter++;
			x[counter] = val.at(0) - '0';   counter++;

		}
		while (counter <= w)
		{
			x[counter] = 0;
			counter++;
		}

	}

	void fillArrayY(int y[], vector <long long> v)
	{
		int counter = 0;
		for (int s = v.size() - 1; s >= 0; s--)
		{
			string val = to_string(v[s]);
			while (val.size() < 9)
			{
				val = '0' + val;
			}
			y[counter] = val.at(8) - '0';   counter++;
			y[counter] = val.at(7) - '0';   counter++;
			y[counter] = val.at(6) - '0';   counter++;
			y[counter] = val.at(5) - '0';   counter++;
			y[counter] = val.at(4) - '0';   counter++;
			y[counter] = val.at(3) - '0';   counter++;
			y[counter] = val.at(2) - '0';   counter++;
			y[counter] = val.at(1) - '0';   counter++;
			y[counter] = val.at(0) - '0';   counter++;
		}
		while (counter <= w)
		{
			y[counter] = 0;
			counter++;
		}
	}
	string toString(int a[])
	{
		string result = "";
		for (int i = 0; i < length(a); i++)
			result = to_string(a[i]) + result;

		return result;
	}

	//algorithm 9
	void division(int x[], int y[], int q[], int r[])
	{
		int m, n, y1;
		m = length(y);
		if (m == 1)
		{
			y1 = y[m - 1];
			if (y1 > 0)
			{
				quotient(q, x, y1);
				remainder(r, x, y1);
			}
			else
				cout << "OVERFLOW_division" << "\n";
		}
		else
		{
			n = length(x);
			if (m > n)
			{
				q = zero;
				//r = x;  this is wrong , the correction is to use a loop

				for (int p = 0; p < n; p++) {
					r[p] = x[p];
				}
			}
			else
			{
				//2 <= m <= n <= w
				longDivide(x, y, q, r, n, m);
			}
		}
	}

	// algorithm 8
	void longDivide(int x[], int y[], int q[], int r[], int n, int m)
	{
		//2 <= m <= n <= w
		int d[w + 1];
		int dq[w + 1];
		int f, k, qt;

		f = b / (y[m - 1] + 1);

		product(r, x, f);
		product(d, y, f);

		// q = zero;  //useless , because it is already defined

		for (k = n - m; k >= 0; k--)
		{
			// 2 <= m <= k+m <= n <= w
			qt = trial(r, d, k, m);
			product(dq, d, qt);
			if (smaller(r, dq, k, m))
			{
				qt = qt - 1;
				product(dq, d, qt);
			}
			q[k] = qt;
			difference(r, dq, k, m);
		}
		quotient(r, r, f);
	}

	//algorithm 7
	void difference(int r[], int dq[], int k, int m)
	{
		int borrow, diff, i;
		borrow = 0;
		for (i = 0; i <= m; i++)
		{
			diff = r[i + k] - dq[i] - borrow + b;
			r[i + k] = diff % b;
			borrow = 1 - diff / b;
		}
		if (borrow != 0)
			cout << "OVERFLOW_diffrence" << "\n";
	}

	// algorithm 6
	bool smaller(int r[], int dq[], int k, int m)
	{
		// 0 <= k <= k + m <= w  you have to verify this assumption
		int i, j;
		i = m;  j = 0;

		while (i != j)
		{
			if (r[i + k] != dq[i])
				j = i;
			else
				i = i - 1;
		}
		if (r[i + k] < dq[i])
			return true;
		else
			return false;
	}

	// algorithm 5
	int trial(int r[], int d[], int k, int m)
	{
		//2 <= m <= k+m <= w
		int d2, km, r3;
		km = k + m;
		r3 = (r[km] * b + r[km - 1]) * b + r[km - 2];
		d2 = d[m - 1] * b + d[m - 2];
		return min(r3 / d2, b - 1);
	}

	//Algorithm 4
	void remainder(int x[], int y[], int k)
	{

		int carry, i, m;
		m = length(y);

		//x = zero;
		fill_n(x, w + 1, 0);
		carry = 0;
		for (i = m - 1; i >= 0; i--)
		{
			carry = (carry * b + y[i]) % k;
		}
		x[0] = carry;
	}

	//algorithm 3
	void quotient(int x[], int y[], int k)
	{

		int carry, i, m, temp;
		m = length(y);

		carry = 0;
		for (i = m - 1; i >= 0; i--)
		{
			temp = carry * b + y[i];
			x[i] = temp / k;
			carry = temp % k;
		}
	}


	//algorithm 2

	void product(int x[], int y[], int k)
	{
		int carry, i, m, temp;

		m = length(y);
		//	x =zero;
		fill_n(x, w + 1, 0);
		carry = 0;

		for (i = 0; i <= (m - 1); i++)
		{
			temp = y[i] * k + carry;
			x[i] = temp % b;
			carry = temp / b;
		}

		if (m <= w)
		{
			x[m] = carry;
		}
		else if (carry != 0)
			cout << "OVERFLOW_product" << "\n";
	}

	// algorithm 1
	int length(int x[])
	{
		int i, j;
		i = w; j = 0;
		while (i != j)
		{
			if (x[i] != 0)
				j = i;
			else
				i = i - 1;
		}
		return i + 1;
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

	BigInt mod(BigInt & other)
	{
		//if (isZero(other.digitsString))
		//cout << "non-positive modulus" << "\n";

		return divide(other, "r");
	}

	BigInt modModified(BigInt & other)
	{
		/*if (isZero(other.digitsString))
		cout << "non-positive modulus" << "\n";*/


		BigInt thisObject = *this;

		BigInt q = thisObject.divide(other);
		BigInt r = thisObject.sub(q.multiply(other));


		if (r.signNum == -1)
		{
			BigInt factor = r.divide(other); // it should be negative
			factor.signNum = 1;
			BigInt temp = other.multiply(factor);
			r = r.add(temp);
		}
		//add other one more time if result is still negative

		if (r.signNum == -1)
			r = r.add(other);

		return r;

	}

	BigInt modPow(BigInt & exponent, BigInt & m) {

		/*if (isZero(m.digitsString))
		cout<<"non-positive modulo"<<"\n";*/

		BigInt zero("0");
		BigInt one("1");
		BigInt two("2");
		if (exponent.equals(one))
			return mod(m);

		BigInt s("1");
		BigInt t = *this;
		BigInt u = exponent;


		while (!u.equals(zero))
		{
			string str = to_string(u.digits[u.size - 1]);
			int ch = str.at(str.size() - 1) - '0';

			if (ch % 2 != 0)
				s = s.multiply(t).mod(m);

			u = u.divide(two);
			t = t.multiply(t).mod(m);
		}
		return s;
	}



	BigInt modInv(BigInt & other)
	{
		/**
		 * using extended ecluid algorithm
		 */

		//other couldn't be negative
		BigInt ZERO("0");
		BigInt ONE("1");

		BigInt A1 = ONE;    BigInt A2 = ZERO;   BigInt A3 = other;

		BigInt B1 = ZERO;   BigInt B2 = ONE;    BigInt B3 = *this;

		BigInt T1 = ZERO;   BigInt T2 = ZERO;    BigInt T3 = ZERO;

		BigInt Q = ZERO;

		int i = 0;
		while (true)
		{
			i++;
			if (B3.equals(ZERO))
			{

				//inverse does not exist
				return BigInt("-1");
				break;
			}

			if (B3.equals(ONE))
			{

				BigInt result = B2.mod(other);
				// make result positive

				if (result.signNum == -1)
				{
					BigInt factor = result.divide(other); // it should be negative
					factor.signNum = 1;
					BigInt temp = other.multiply(factor);
					result = result.add(temp);
				}
				//add other one more time if result is still negative

				if (result.signNum == -1)
					result = result.add(other);

				return result;

			}

			Q = A3.divide(B3);

			T1 = A1.sub(Q.multiply(B1));
			if (T1.signNum == -1)  T1 = T1.modModified(other);

			T2 = A2.sub(Q.multiply(B2));
			if (T2.signNum == -1)  T2 = T2.modModified(other);

			T3 = A3.sub(Q.multiply(B3));
			if (T3.signNum == -1)  T3 = T3.modModified(other);

			A1 = B1; A2 = B2; A3 = B3;

			B1 = T1; B2 = T2; B3 = T3;
		}
	}


	bool equals(BigInt & g)
	{
		int aIndex = 0;
		int bIndex = 0;

		while (aIndex < size)
		{
			if (digits[aIndex] == 0)
				aIndex++;
			else
				break;
		}
		while (bIndex < g.size)
		{
			if (g.digits[bIndex] == 0)
				bIndex++;
			else
				break;
		}

		int aActualSize = size - aIndex;
		int bActualSize = g.size - bIndex;

		if (aActualSize != bActualSize)
			return false;
		else
		{
			for (int s = 0; s < aActualSize; s++)
			{
				if (digits[s + aIndex] != g.digits[s + bIndex])
					return false;
			}
			return true;
		}

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

	BigInt p1("11");
	BigInt p2("222");
	BigInt p3("6");
	BigInt p4("111");
	BigInt p5("16");


	BigInt addResult = p1.add(p2);
	BigInt subResult = p1.sub(p2);
	BigInt mulResult = p1.multiply(p2);
	BigInt divResult = p2.divide(p1);
	BigInt modResult = p2.mod(p1);
	BigInt modPowResult =  p1.modPow(p3, p2);
	BigInt modInv = p4.modInv(p5);


	cout<<"p1 + p2 = "<< addResult.toString()<<"\n";
	cout<<"p1 - p2 = "<< subResult.toString()<<"\n";
	cout<<"p1 * p2 = "<< mulResult.toString()<<"\n";
	cout<<"p2 / p1 = "<< divResult.toString()<<"\n";
	cout<<"p2 % p1 = "<< modResult.toString()<<"\n";
	cout<<"(p1 ^ p3) mod p2 = "<< modPowResult.toString()<<"\n";
	cout<<"p4 modInv p5 = "<< modInv.toString()<<"\n";



}

