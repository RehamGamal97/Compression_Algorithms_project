#include "Huffman.h"

float Huffman::compute_entropy(const vector<unsigned char>& msg, vector<Symbol>* prob)
{
	vector<pair<int, unsigned char>>probability(256);
	for (int i = 0; i < msg.size(); i++)
	{
		probability[(int)msg[i]].second = msg[i];
		probability[(int)msg[i]].first++;
	}
	sort(probability.rbegin(), probability.rend());
	float entropy = 0;
	for (int i = 0; i < 256; i++)
	{
		if (probability[i].first == 0)
			break;
		float p = probability[i].first * 1.0 / msg.size();
		Symbol s;
		s.val = probability[i].second;
		s.probability = p;
		entropy = entropy + (p * log2(1.0 / p));
		prob->push_back(s);
	}
	return entropy;
}

void Huffman::build_tree(const vector<Symbol>& prob)
{
	priority_queue<pair<float, Node*>, vector<pair<float, Node*>>, greater<pair<float, Node*>> >q;
	for (int i = 0; i < prob.size(); i++)				
	{
		Node*temp = new Node();
		temp->val = prob[i];
		temp->left = nullptr;
		temp->right = nullptr;
		q.push(make_pair(prob[i].probability, temp));
	}
	while (q.size() > 1)
	{
		pair<float, Node*> one = q.top();
		q.pop();
		pair<float, Node*> two = q.top();
		q.pop();
		float p = one.first + two.first;			
		Symbol temp;
		temp.val = 0;
		temp.probability = p;
		Node*ptr = new Node();
		ptr->val = temp;
		ptr->left = one.second;
		ptr->right = two.second;
		q.push(make_pair(p,ptr));
	}
	tree = q.top().second;
	q.pop();

	MakeCodes(tree, "");
}
void Huffman::MakeCodes(Node*ptr, string str)
{
	if (ptr->left == nullptr && ptr->right == nullptr)
	{
		CodesMap[ptr->val.val] = str;
		return;
	}
	MakeCodes(ptr->left, str + '0');
	MakeCodes(ptr->right, str + '1');
}
void Huffman::print_code_table()
{
	cout << "Codes:" << endl;
	map<unsigned char, string>::iterator looper = CodesMap.begin();
	while (looper!=CodesMap.end())
	{
		cout << "Character: "<<(unsigned int)looper->first << " , Code: " << looper->second << endl;
		looper++;
	}
}
unsigned int Huffman::to_binary(string  s)
{
	int Num = 0;
	int c = 0;
	for (int i = s.length() - 1; i >= 0; i--)
	{
		int temp = s[i] - '0';
		Num = Num + temp*(pow(2, c));
		c++;
	}
	return Num;
}
int Huffman::encode(const vector<unsigned char>& msg, vector<unsigned int>* encoded_msg)
{
	vector<Symbol>prob;
	compute_entropy(msg, &prob);
	build_tree(prob);
	int BitsNum = 0;
	for (int i = 0; i < msg.size(); i++)
	{
		string code = CodesMap[msg[i]];
		unsigned int Num = to_binary(code);
		encoded_msg->push_back(Num);
		BitsNum = BitsNum + code.length();
	}
	return BitsNum;
}