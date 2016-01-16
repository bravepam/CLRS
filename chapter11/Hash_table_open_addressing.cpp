
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<vector>

using namespace std;

const int NIL = 0x80000000;//表示该位置未被使用
const int DELETE = 0x7fffffff;//表示该位置元素已被删除，可以使用，主要是为便于查找在该元素之后的元素
const int FAILURE = 0x7ffffffe;//表示失败状态

template < size_t hash_table_size >//哈希表长
class hash_table
{//采用开放寻址法的哈希表
private:
	typedef size_t (*hash_function)(int,size_t);
	vector<int> hash;//哈希表
	hash_function hash_fun;//哈希函数
public:
	hash_table(hash_function h) :hash_fun(h), hash(hash_table_size)
	{
		for (size_t i = 0; i != hash.size(); ++i)
			hash[i] = NIL;
	}
	size_t insert(int);
	size_t search(int);
	size_t erase(int);
	void print()
	{
		for (size_t i = 0; i != hash.size(); ++i)
		{
			if (hash[i] == NIL) cout << "NIL ";
			else if (hash[i] == DELETE) cout << "DELETE ";
			else cout << hash[i] << " ";
		}
		cout << endl;
	}
};

template <size_t hash_table_size>
size_t hash_table<hash_table_size>::insert(int value)
{//插入
	for (size_t i = 0; i != hash_table_size; ++i)
	{
		size_t hash_index = hash_fun(value, i);
		if (hash[hash_index] == NIL || hash[hash_index] == DELETE)
		{
			hash[hash_index] = value;
			return hash_index;
		}
	}
	cout << "hash_table overflow " << value << endl;
	return FAILURE;
}

template <size_t hash_table_size>
size_t hash_table<hash_table_size>::search(int value)
{//查找
	for (size_t i = 0; i != hash_table_size; ++i)
	{
		size_t hash_index = hash_fun(value, i);
		if (hash[hash_index] == value) return hash_index;
		else if (hash[hash_index] == NIL)
			return FAILURE;
	}
	return FAILURE;
}

template <size_t hash_table_size>
size_t hash_table<hash_table_size>::erase(int value)
{//删除
	for (size_t i = 0; i != hash_table_size; ++i)
	{
		size_t hash_index = hash_fun(value, i);
		if (hash[hash_index] == value)
		{
			hash[hash_index] = DELETE;
			return hash_index;
		}
	}
	return FAILURE;
}

const int TABLE_SIZE = 11;

size_t linearProbing(int value, size_t i)
{//线性探查法解决冲突
	size_t tmp = value % TABLE_SIZE;
	return (tmp + i) % TABLE_SIZE;
}

size_t quadraticProbing(int value, size_t i)
{//二次探查法解决冲突c1 = 1,c2 = 3
	size_t tmp = value % TABLE_SIZE;
	return (tmp + 1 * i + 3 * i * i) % TABLE_SIZE;
}

size_t doubleHashing(int value, size_t i)
{//双重散列法解决冲突
	size_t tmp1 = value % TABLE_SIZE,
		tmp2 = 1 + (value % (TABLE_SIZE - 1));
	return (tmp1 + i * tmp2) % TABLE_SIZE;
}

int main()
{
	vector<int> vec = {10,22,31,4,15,28,17,88,59};
	hash_table<TABLE_SIZE> hash1(linearProbing);
	for (size_t i = 0; i != vec.size(); ++i)
		hash1.insert(vec[i]);
	hash1.erase(10);
	hash1.erase(4);
	hash1.erase(88);
	cout << hash1.search(15) << endl;
	cout << hash1.search(28) << endl;
	cout << hash1.search(22) << endl;
	cout << hash1.search(4) << endl;
	hash1.print();
	cout << "Linear Probing--------------" << endl << endl;
	hash_table < TABLE_SIZE> hash2(quadraticProbing);
	for (size_t i = 0; i != vec.size(); ++i)
		hash2.insert(vec[i]);
	hash2.print();
	cout << "Quadratic Probing-----------" << endl << endl;
	hash_table<TABLE_SIZE> hash3(doubleHashing);
	for (size_t i = 0; i != vec.size(); ++i)
		hash3.insert(vec[i]);
	hash3.print();
	cout << "Double Hasing---------------" << endl << endl;
	getchar();
	return 0;
}