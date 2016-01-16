
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<vector>

using namespace std;

template <typename T>
struct hash_table_list_node
{//链表节点
	T data;
	hash_table_list_node *next;
	hash_table_list_node(T d) :data(d), next(nullptr){}
};

template <typename T>
struct hash_table_list
{//链表
	hash_table_list_node<T> *head;
	hash_table_list_node<T>* locatePre(const T&);
	//各接口
	hash_table_list() :head(nullptr){}
	void insert(const T&);
	void erase(const T&);
	hash_table_list_node<T>* search(const T&);
	void print();//打印链表
	void destroy();//销毁链表
};

template <typename T>
void hash_table_list<T>::insert(const T &value)
{
	hash_table_list_node<T> *p = new hash_table_list_node<T>(value);
	p->next = head;
	head = p;
}

template <typename T>
hash_table_list_node<T>* hash_table_list<T>::search(const T &value)
{
	hash_table_list_node<T> *curr = head;
	while (curr != nullptr && curr->data != value)
		curr = curr->next;
	return curr;
}

template <typename T>
hash_table_list_node<T>* hash_table_list<T>::locatePre(const T &value)
{//查找value的前驱
	hash_table_list_node<T> *pre = head;
	while (pre->next->data != value)
		pre = pre->next;
	return pre;
}

template <typename T>
void hash_table_list<T>::erase(const T &value)
{
	hash_table_list_node<T> *curr = search(value);
	if (curr == nullptr) return;
	if (curr == head)
	{//删除的是头结点
		head = curr->next;
		delete curr;
	}
	else
	{//否则
		hash_table_list_node<T> *pre = locatePre(value);
		pre->next = curr->next;
		delete curr;
	}
}

template <typename T>
void hash_table_list<T>::destroy()
{
	hash_table_list_node<T> *p;
	while (head != nullptr)
	{
		p = head;
		head = head->next;
		delete p;
	}
}

template <typename T>
void hash_table_list<T>::print()
{
	hash_table_list_node<T> *p = head;
	while (p != nullptr)
	{
		cout << p->data;
		if (p->next == nullptr) cout << endl;
		else cout << ' ';
		p = p->next;
	}
}


template <typename T,size_t hash_table_size>//存储数据类型和表长
class hash_table
{//哈希表，链接法解决冲突
public:
	typedef size_t (*hash_function)(const T&);//哈希函数形式
	typedef hash_table_list<T>				list;
	typedef hash_table_list_node<T>			node;
private:
	vector<list> hash;//哈希表
	hash_function hash_fun;//所用哈希函数
public:
	//所有具体操作转给链表
	hash_table(hash_function h) :hash_fun(h), hash(hash_table_size){}
	void insert(const T&);
	bool search(const T&);
	void erase(const T&);
	void print();
	~hash_table()
	{
		for (size_t i = 0; i != hash.size(); ++i)
			hash[i].destroy();
	}
};

template <typename T,size_t hash_table_size>
void hash_table<T,hash_table_size>::insert(const T &value)
{
	size_t hash_index = hash_fun(value);
	hash[hash_index].insert(value);
}

template <typename T,size_t hash_table_size>
bool hash_table<T, hash_table_size>::search(const T &value)
{
	size_t hash_index = hash_fun(value);
	node *curr = hash[hash_index].search(value);
	if (curr == nullptr) return false;
	return true;
}

template <typename T,size_t hash_table_size>
void hash_table<T, hash_table_size>::erase(const T &value)
{
	size_t hash_index = hash_fun(value);
	hash[hash_index].erase(value);
}

template <typename T,size_t hash_table_size>
void hash_table<T, hash_table_size>::print()
{
	for (size_t i = 0; i != hash.size(); ++i)
	{
		cout << "Slot " << i << " ----> ";
		if (hash[i].head == nullptr) cout << "null" << endl;
		else hash[i].print();
	}
}

const int TABLE_SIZE = 9;

template <typename T>
size_t compute_index(const T &value)
{
	return value % TABLE_SIZE;
}

int main()
{
	hash_table<int, TABLE_SIZE> hash(compute_index);
	vector<int> vec = { 5, 28, 19, 15, 20, 33, 12, 17, 10 };
	for (size_t i = 0; i != vec.size();++i)
		hash.insert(vec[i]);
	hash.print();
	getchar();
	return 0;
}