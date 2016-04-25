#include"SkipList.h"
#include<iostream>
#include<string>
#include<random>
#include<ctime>

using namespace std;
using iter = skiplist<int, int>::iterator;

void skiplistEmptyTest(const skiplist<int,int> &list)
{
	cout << boolalpha << list.empty() << endl;
	cout << boolalpha << list.contain(2) << endl;
	cout << boolalpha << list.contain(0) << endl;
}

void skipListIteratorTest(const skiplist<int, int> &list)
{
	iter it(&list);
	//it.key();
	cout << boolalpha << it.valid() << endl;
	it.seek(2);
	cout << boolalpha << it.valid() << endl;
}

void skiplistTest()
{
	skiplist<int, int> list;
	skiplistEmptyTest(list);
	skipListIteratorTest(list);
	//linear_congruential_engine<unsigned long, 16807, 0, 2147483647> rnd{ (unsigned int)time(0) };
	linear_congruential_engine<unsigned long, 16807, 0, 2147483647> rnd{};
	const int M = 1000, N = 10;
	for (int i = 0; i != N; ++i)
	{
		int key = rnd() % M, value = rnd() % M;
		list.insert(key, value);
	}
	iter it(&list);
	int i = 0;
	for (it.first(); it.valid(); it.next())
	{
		cout << i << " : " << it.key() << ' ' << it.value() << endl;
		++i;
	}
	list.insert(2, 1000);
	list.insert(1000, 1000);
	skiplistEmptyTest(list);
	skipListIteratorTest(list);
	cout << "-------------------------------" << endl;
	i = 0;
	for (it.first(); it.valid(); it.next())
	{
		cout << i << " : " << it.key() << ' ' << it.value() << endl;
		++i;
	}
	cout << "-------------------------------" << endl;
	list.erase(1000);
	list.erase(2);
	i = 0;
	for (it.first(); it.valid(); it.next())
	{
		cout << i << " : " << it.key() << ' ' << it.value() << endl;
		++i;
	}
	skiplistEmptyTest(list);
	skipListIteratorTest(list);
	list.insert(2, 1000);
	list.insert(1000, 1000);
	list.edit(2, 1024);
	list.edit(1000, 10240);
	cout << "-------------------------------" << endl;
	i = 0;
	for (it.first(); it.valid(); it.next())
	{
		cout << i << " : " << it.key() << ' ' << it.value() << endl;
		++i;
	}
	//list.edit(1, 1111); //edit采用assert，对于不存在的键将会崩溃
}

int main()
{
	skiplistTest();
	getchar();
	return 0;
}