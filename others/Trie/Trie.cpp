#include<fstream>
#include<iostream>
#include<string>
#include"Trie.h"

int main()
{
	vector<string> words;
	string word;
	ifstream infile("words.txt");
	if (!infile)
	{
		cerr << "open file error" << endl;
		return -1;
	}
	while (infile >> word)
		words.emplace_back(word);
	Trie trie;
	trie.create(words);
	//trie.insert("class");
	/*for (int i = 0; i != words.size(); ++i)
	{
		cout << words[i] << ": " << trie.search(words[i]) << endl;
	}*/
	vector<pair<string, int>> counting = trie.countingFrequency(words.size());
	int count = 0;
	for (int i = 0; i != counting.size(); ++i)
	{
		count += counting[i].second;
		cout << counting[i].first << ' ' << counting[i].second << endl;
	}
	cout << "-----------------\nTotal: " << count << endl;
	getchar();
	return 0;
}