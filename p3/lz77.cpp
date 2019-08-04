#include "lz77.h"


Triplet* LZ77::getTripletForMatches(string mes, vector<unsigned char> SearchBuffer)
{
	int BufferSize = SearchBuffer.size();
	int stringSize = mes.length();
	int j = 0;
	for (int i = 0; i <= BufferSize - stringSize; i++)
	{
		for (j = 0; j < stringSize; j++)
			if (SearchBuffer[i + j] != mes[j])
				break;

		if (j == stringSize)
		{
			Triplet *tag = new Triplet;
			tag->c = mes[0];
			tag->j = SearchBuffer.size() - i;
			for (int n = 0; n < SearchBuffer.size(); n++)
			{
				if (SearchBuffer[n] == mes[0])
				{
					int it;
					for (it = 0; (n + it)<SearchBuffer.size() && it < mes.size(); it++)
					{
						if (mes[it] != SearchBuffer[n + it])
							break;
					}
					if (it == mes.size()) {
						tag->j = SearchBuffer.size() - n;//update the offset
					}
				}
			}
			tag->k = mes.length();
			return tag;
		}

	}
	Triplet *tag = new Triplet;
	if (mes.length() == 1)
	{
		tag->j = 0;
		tag->k = 0;
		tag->c = mes[j];
	}
	else
	{
		tag->j = 0;
		tag->k = mes.length();
		tag->c = mes[j];

	}
	return tag;
}


int LZ77::encode(const vector<unsigned char>& msg, int S, int T, int A, vector<Triplet>* encoded_msg)
{
	bool check = false;
	vector<unsigned char> SB;
	vector<unsigned char> LAB;
	Triplet *temp = new Triplet();
	Triplet *checks = new Triplet();
	for (int i = 0; i < T; i++) {
		LAB.push_back(msg[i]);
	}

	for (int i = LAB.size(); i < msg.size() || LAB.size() != 0; i++) 
	{
		string next = "";
		int j = 0;
		do
		{
			next += LAB[j];
			temp = getTripletForMatches(next, SB);
			if (temp->k != 0 && temp->j != 0)
			{  // no matches
				checks->c = LAB[j + 1];
				checks->j = temp->j;
				checks->k = temp->k;
				j++;
			}
		} while (temp->j != 0 && j<LAB.size() - 1);
		if (temp->k == 0 && next.size() - 1<T) 
		{
			checks->j = temp->j;
			checks->k = temp->k;
			checks->c = temp->c;		
		}
		encoded_msg->push_back(*checks);
		int L = 0;
		while (L <= checks->k)
		{
			SB.push_back(LAB[L]);
			L++;
		}
		if (checks->k == 0)
		{
			LAB.erase(LAB.begin());
		}
		else 
		{
			LAB.erase(LAB.begin(), LAB.begin() + checks->k + 1);
		}
		if (SB.size() > S) 
		{
			if (checks->j == 0)
			{
				SB.erase(SB.begin());
			}
			else 
			{
				while (SB.size() != S) 
				{
					SB.erase(SB.begin());
				}
			}
		}

		while (i < msg.size() && LAB.size() < T) 
		{
			LAB.push_back(msg[i]);
			i++;
			check = true;
		}
		i--;
	}
	int max1 = 0, max2 = 0;
	for (int i = 0; i < encoded_msg->size(); i++)
	{
		max1 = max(max1, (int)(*encoded_msg)[i].j);
		max2 = max(max2, (int)(*encoded_msg)[i].k);
	}
	return encoded_msg->size() * 8 + ceil(log2(max1)) + ceil(log2(max2));
}

void LZ77::decode(const vector<Triplet>& encoded_msg, vector<unsigned char>* decoded_msg)
{
	vector<unsigned char>SB;
	vector<unsigned char>LAB;
	for (int i = 0; i < encoded_msg.size(); i++) 
	{
		if (encoded_msg[i].j == 0 && encoded_msg[i].k == 0)  //no previus
		{
			SB.push_back(encoded_msg[i].c);
		}
		else
		{
			int j = SB.size() - encoded_msg[i].j;
			for (int k = 0; k < encoded_msg[i].k; k++) 
			{
				if (j < SB.size()) 
				{
					LAB.push_back(SB[j]);
					j++;
				}
				else
				{
					for (int counter = 0; counter < encoded_msg[i].k - k; counter++) 
					{
						LAB.push_back(LAB[counter]);
					}

				}
			}
			LAB.push_back(encoded_msg[i].c);
			for (int k = 0; k < LAB.size(); k++) 
			{
				SB.push_back(LAB[k]);
			}
			LAB.erase(LAB.begin(), LAB.end());
		}
	}

	for (int i = 0; i < SB.size(); i++) 
	{
		decoded_msg->push_back(SB[i]);
	}
}


