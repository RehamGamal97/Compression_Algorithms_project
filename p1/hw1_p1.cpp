
#include"huffman.h"
int main(int argc, char* argv[])
{
	string first = "";
	string second = "";
	if (argc > 1)
		first = argv[1];
	if (argc > 2)
		second = argv[2];
	Huffman huff;
	string t;				
	cin >> t;
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
	if (first == "-entropy")
	{
		vector<Symbol>prob;
		float entropy = huff.compute_entropy(message, &prob);
		cout << "image's entropy = " << entropy << endl;
	}
	else if (first == "-tree")
	{
		vector<Symbol>prob;
		huff.compute_entropy(message, &prob);
		huff.build_tree(prob);
		huff.print_code_table();
	}
	else if (first == "-encode" && second == "")
	{
		vector<unsigned int>encoded;
		int BitsNum = huff.encode(message, &encoded);
		cout << "Bits Number = " << BitsNum << endl;
		for (int i = 0; i < encoded.size(); i++)
		{
			cout << encoded[i] << " ";
		}
		cout << endl;
	}
	else if (first == "-encode" && second == "-diff")
	{
		vector<unsigned int>encoded;
		vector<unsigned char>diff_message;
		for (int i = 0; i < y; i++)
		{
			stack<unsigned char>stk;
			stk.push(arr[i][0]);
			for (int j = x - 1; j > 0; j--)
			{
				stk.push(arr[i][j] - arr[i][j - 1]);
			}
			while (!stk.empty())
			{
				diff_message.push_back(stk.top());
				stk.pop();
			}
		}
		int BitsNum = huff.encode(diff_message, &encoded);
		cout << "Bits Number = " << BitsNum << endl;
		for (int i = 0; i < encoded.size(); i++)
		{
			cout << encoded[i] << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "enter right input" << endl;
	}
}


