#include "Arithmetic.h"

using namespace std;



double Arithmetic::binary_to_decimal(const bitset<32>& fraction)
{
	double dec = 0;
	int c = 1;
	for (int i = fraction.size()-1; i>=0; i--)
	{
		dec = dec + fraction[i] * pow(2,-(c++));
	}
	return dec;
}

bool sortbysec(const pair<unsigned char, float> &a,const pair<unsigned char, float> &b)
{
	return (a.second < b.second);
}

void Arithmetic::decode(const vector<bitset<32>>& encoded_msg, int num_symbols_per_block, vector<unsigned char>* decoded_msg)
{
	double x, y;
	int z;
	while (cin >> x >> y >> z)
	{
		probline.push_back(make_tuple(z, x, y));
	}
		for (int i = 0; i < encoded_msg.size(); i++)
		{
			float lower = 0.0;
			float upper = 0.0;
			float l2 = 0.0;
			float u2 = 0.0;
			float TagGroup = binary_to_decimal(encoded_msg[i]);
			for (int j = 0; j < num_symbols_per_block; j++)
			{
				if (j == 0)
				{
					lower = 0;
					upper = 1.0;
					float symbol_prob = (TagGroup - lower) / (upper - lower);
					for (int z = 0; z < probline.size(); z++)
					{
						if (get<1>(probline[z]) <= symbol_prob && get<2>(probline[z]) >= symbol_prob)
						{
							decoded_msg->push_back(get<0>(probline[z]));
							l2 = get<1>(probline[z]);
							u2 = get<2>(probline[z]);
							break;
						}
					}
				}
				else
				{
					float temp = lower;
					lower = lower + (upper - lower)*l2;
					upper = temp + (upper - temp)*u2;
					float symbol_prob = (TagGroup - lower) / (upper - lower);
					for (int z = 0; z < probline.size(); z++)
					{
						if (get<1>(probline[z]) <= symbol_prob && get<2>(probline[z]) >= symbol_prob)
						{
							decoded_msg->push_back(get<0>(probline[z]));
							l2 = get<1>(probline[z]);
							u2 = get<2>(probline[z]);
							break;
						}
					}
				}
			}
		}


}

int Arithmetic::encode(const vector<unsigned char>& msg, int num_symbols_per_block, vector<bitset<32>>* encoded_msg)
{
	int BitsNum = 0;
	vector <pair<unsigned char, float>> vect;  //for char and prob value
	vector<unsigned char> Mes_Copy = msg;      //as mes is constant
	sort(Mes_Copy.begin(), Mes_Copy.end());
	int k = 0;
	int l = 0;
	unsigned char val = 0;
	float probability = 0.0;
	for (k = 0; k < Mes_Copy.size() - 1; k++)
	{
		val = Mes_Copy[k];
		probability++;
		if (Mes_Copy[k] != Mes_Copy[k + 1])
		{
			vect.push_back(make_pair(val, probability));
			val = 0;
			probability = 0;
			l++;
		}
	}
	if (vect[l - 1].first == Mes_Copy[k])
	{
		vect[l - 1].second++;
	}
	else
	{
		val = Mes_Copy[k];
		probability++;
		vect.push_back(make_pair(val, probability));
	}
	float total = Mes_Copy.size() * 1.0;
	for (int i = 0; i < vect.size(); i++)
	{
		vect[i].second = vect[i].second / total;
	}
	sort(vect.begin(), vect.end(), sortbysec);
	float temp;
	for (int i = 0; i < vect.size(); i++)
	{
		unsigned char val = 0;
		float start = 0.0;
		float end = 0.0;
		if (i == 0)
		{
			val = vect[i].first;
			start = 0;
			end = vect[i].second;
			temp = end;
			probline.push_back(make_tuple(val, start, end));
		}
		else
		{
			val = vect[i].first;
			start = temp;
			end = vect[i].second + temp;
			temp = end;
			probline.push_back(make_tuple(val, start, end));
		}
	}
	float Lower = 0.0;
	float Upper = 0.0;
	float l2 = 0.0;
	float u2 = 0.0;
	float tag = 0.0;
	for (int i = 0; i < msg.size(); i += num_symbols_per_block)
	{
		for (int j = i; j < i + num_symbols_per_block && j < msg.size(); j++)
		{
			if (i == j)
			{
				int z = 0;
				for (; z < probline.size(); z++)
				{
					if (msg[j] == get<0>(probline[z]))
					{
						Lower = get<1>(probline[z]);
						Upper = get<2>(probline[z]);
						break;
					}
				}
			}
			else
			{
				int z = 0;
				for (; z < probline.size(); z++)
				{
					if (msg[j] == get<0>(probline[z]))
					{
						l2 = get<1>(probline[z]);
						u2 = get<2>(probline[z]);
						break;
					}
				}
				float temp = Lower;
				Lower = Lower + (Upper - Lower)*l2;
				Upper = temp + (Upper - temp)*u2;
			}
		}
		tag = (Lower + Upper) / 2;
		int k = 0;
		if (fabs(Upper - Lower) < 1e-10)
			k = 32;
		else
			k = ceil(log2(1.0 / (Upper - Lower))) + 1;
		BitsNum += k;
		encoded_msg->push_back(decimal_to_binary(tag));
	}
	return BitsNum;
}

bitset<32> Arithmetic::decimal_to_binary(double fraction)
{
	bitset<32> bin;
	if (fraction > 1) {
		bin.set();
		return bin;
	}
	int i = 31;
	while (fraction != 0 && i>=0) {
		fraction *= 2;
		if (fraction >= 1) {
			fraction--;
			bin[i] = 1;
		}
		else {
			bin[i] = 0;
		}
		i--;
	}
	return  bin;
}
void Arithmetic::printCDF()
{
	cout << "CDF:" << endl;
	for (int i = 0; i < probline.size(); i++)
	{
		cout << get<1>(probline[i])<< " " << get<2>(probline[i]) << " " <<(int) get<0>(probline[i]) << endl;
	}
}
void Arithmetic::printInFile(ofstream & out)
{
	out << "CDF:" << endl;
	for (int i = 0; i < probline.size(); i++)
	{
		out << get<1>(probline[i]) << " " << get<2>(probline[i]) << " " << (int)get<0>(probline[i]) << endl;
	}
}
Arithmetic::Arithmetic()
{}

Arithmetic::~Arithmetic()
{
}