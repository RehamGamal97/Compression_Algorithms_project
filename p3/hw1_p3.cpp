#include"lz77.h"
int main(int argc, char* argv[]) 
{
	string function = argv[1];
	string condition = argv[2];
	if (function == "-encode")
	{
		LZ77 lz;
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
				arr[i].push_back(temp);
			}
		}
		int s, t, A;
		if (condition == "-diff")
		{
			for (int i = 0; i < y; i++)
			{
				stack<unsigned char>st;
				st.push(arr[i][0]);
				for (int j = x - 1; j > 0; j--)
				{
					st.push(arr[i][j] - arr[i][j - 1]);
				}
				while (!st.empty())
				{
					message.push_back(st.top());
					st.pop();
				}
			}
			s = stoi(argv[3]);
			t = stoi(argv[4]);
			A = stoi(argv[5]);
		}
		else
		{
			for (int i = 0; i < y; i++)
			{
				for (int j = 0; j < x; j++)
				{
					message.push_back(arr[i][j]);
				}
			}
			s = stoi(argv[2]);
			t = stoi(argv[3]);
			A = stoi(argv[4]);
		}
		vector<Triplet>encoded_message;
		int BitsNum = lz.encode(message, s, t, A, &encoded_message);
		ofstream out("image.enc");
		cout << "Bits Number : " << BitsNum << ",  Compression Ratio : " << x*y*8.0 / BitsNum << endl;
		out << "Bits Number : " << BitsNum << ",  Compression Ratio : " << x*y*8.0 / BitsNum << endl;
		out << "Triplets : " << endl;
		cout << "Triplets : " << endl;
		for (int i = 0; i < encoded_message.size(); i++)
		{
			cout << encoded_message[i].j << " " << encoded_message[i].k << " " << (unsigned int)encoded_message[i].c << endl;
			out << encoded_message[i].j << " " << encoded_message[i].k << " " << (unsigned int)encoded_message[i].c << endl;
		}
	}
	else if (function == "-decode")
	{
		LZ77 lz;
		string s;	cin >> s;
		getline(cin, s);
		getline(cin, s);
		vector<Triplet>encoded_message;
		vector<unsigned char>decoded_message;
		unsigned int a, b, c;
		ofstream out("output.pgm");
		while (cin >> a >> b >> c)
		{
			encoded_message.push_back({ a,b,(unsigned char)c });
		}
		lz.decode(encoded_message, &decoded_message);
		int x = stoi(argv[2]), y = stoi(argv[3]);
		int tmp = 0;
		out << "P2" << endl;
		out << x << " " << y << endl;
		out << 255 << endl;
		for (int i = 0; i < y; i++)
		{
			for (int j = 0; j < x; j++)
			{
				cout << (unsigned int)decoded_message[tmp] << " ";
				out << (unsigned int)decoded_message[tmp] << " ";
				tmp++;
			}
			cout << endl;
			out << endl;
		}
	}
	else
	{
		cout << "enter right input" << endl;
	}
}
