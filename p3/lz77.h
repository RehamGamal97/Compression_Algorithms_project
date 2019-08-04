#ifndef _LZ77_H
#define _LZ77_H
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<algorithm>
#include<math.h>
#include<stack>
#include<fstream>
#include<set>
using namespace std;

// Struct to hold the triplet for each entry
struct Triplet{
  // Starting position of the longest match in the previous string
  // i.e. number of symbols to look back.
  unsigned int j;
  
  // Length of the longest match i.e. number of symbols to copy
  // starting at the match position.
  unsigned int k;
  
  // Uncompressed code of the symbol following the match. The number
  // of bits here will depend on the size of the alphabet in use.
  unsigned char c;
};

class LZ77 {
public:
  
	int encode(const vector<unsigned char>& msg, int S, int T, int A, vector<Triplet>* encoded_msg);
	void decode(const vector<Triplet>& encoded_msg, vector<unsigned char>* decoded_msg);
                
private:
	Triplet* getTripletForMatches(string mes, vector<unsigned char> SearchBuffer);
  
};

#endif