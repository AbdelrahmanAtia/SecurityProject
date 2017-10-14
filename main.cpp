using namespace std;

class BigInt
{
public:
	vector <long long> digits;
	int size;  //length of digits vector
	int signNum = 1;  //positive number

	static const int w = 700;
	int b = 10;  //base of input integer
	int zero[w + 1];  // w = 400

	//this constructor converts input string into a vector of integers
	BigInt(string n)
	{
		char sign = n.at(0);

		if (sign == '-')
		{
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

	BigInt(vector <long long> temp_digits)
	{
		digits = temp_digits;
		size = digits.size();
	}
}
