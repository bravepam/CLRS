#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>
#include"murmurhash.h"
#include"BloomFilter.h"

using namespace std;

void testMurmurhash()
{
	murmurhash mh;
	const char* str1 = "1234567810101010";
	const char* str2 = "45678";
	cout << mh.MurmurHash64A(str1, sizeof(str1), 1) << endl;
	cout << mh.MurmurHash64A(str2, sizeof(str2), 1) << endl;
}

void testBloomFilter()
{
	ifstream infile("data.txt");
	vector<string> keys;
	string str;
	while (infile >> str)
		keys.emplace_back(move(str));
	BloomFilter bf(keys.size(), 0.01);
	bf.createFilter(keys);
	for (int i = 0; i != keys.size(); ++i)
	{
		bool exist = bf.keyMayMatch(keys[i]);
		cout << keys[i] << " : " << boolalpha << exist << endl;
	}
	cout << "----------------------------" << endl;
	ifstream infile2("data2.txt");
	vector<string> keys2;
	while (infile2 >> str)
		keys2.emplace_back(move(str));
	for (int i = 0; i != keys2.size(); ++i)
	{
		bool exist = bf.keyMayMatch(keys2[i].data());
		auto iter = find(keys.begin(), keys.end(), keys2[i]);
		if (exist && iter != keys.end())
			cout << *iter << ' ' << keys2[i] << endl;
	}
}

int main()
{
	testMurmurhash();
	testBloomFilter();
	getchar();
	return 0;
}
