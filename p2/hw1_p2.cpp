
#include "Arithmetic.h"

int main(int argc, char* argv[]) 
{
	string function = argv[1];
	Arithmetic arith;
	if (function == "-bin_to_dec")
	{
		string bin = argv[2];
		bitset<32>Binary(bin);
		cout << arith.binary_to_decimal(Binary) << endl;
	}
	else if (function == "-dec_to_bin")
	{
		double dec = stod(argv[2]);
		cout << arith.decimal_to_binary(dec) << endl;
	}
	else if (function == "-encode")
	{
		string type;
		cin >> type;
		int x, y, max;
		cin >> x >> y >> max;
		vector<unsigned char>message;
		vector<vector<unsigned int>>arr;
		arr.resize(y);
		for (int i = 0; i < y; i++)
		{
			for (int j = 0; j < x; j++)
			{
				int temp;	cin >> temp;
				message.push_back(temp);
				arr[i].push_back(temp);
			}
		}
		int k = stoi(argv[2]);
		vector<bitset<32>>encoded_msg;
		int BitsNum = arith.encode(message, k, &encoded_msg);
		ofstream out("image.enc");
		cout << "Bits Number " << BitsNum << ",  Compression Ratio: " << y*x*8.0 / BitsNum << endl;
		out << "Bits Number " << BitsNum << ",  Compression Ratio: " << y*x*8.0 / BitsNum << endl;
		cout << "Tags:" << endl;
		out << "Tags:" << endl;
		for (int i = 0; i < encoded_msg.size(); i++)
		{
			cout << encoded_msg[i] << endl;
			out << encoded_msg[i] << endl;
		}
		arith.printCDF();
		arith.printInFile(out);
	}
	else if (function == "-decode")
	{
		int k = stoi(argv[2]), x = stoi(argv[3]), y = stoi(argv[4]), max = stoi(argv[5]);
		vector<unsigned char>decoded_msg;
		string s, t;	
		getline(cin, s);	
		getline(cin, t);
		vector<bitset<32>>encoded_msg;
		ofstream out("output.pgm");
		while (cin >> s)
		{
			if (s == "CDF:")
				break;
			encoded_msg.push_back(bitset<32>(s));
		}
		arith.decode(encoded_msg, k, &decoded_msg);
		out << "P2" << endl;
		out << x << " " << y << endl;
		out << max << endl;
		int c = 0;
		for (int i = 0; i < y; i++)
		{
			for (int j = 0; j < x; j++)
			{
				cout << (unsigned int)decoded_msg[c] << " ";
				out << (unsigned int)decoded_msg[c] << " ";
				c++;
			}
			cout << endl;
			out << endl;
		}
	}
	else
	{
		cout << "enter right input" << endl;
	}
	return 0;
}

