
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>

#define NOEXIST 0x7fffffff
using namespace std;
template <typename T> class list;
template <typename T> ostream& operator<<(ostream&, const list<T> &);

template <typename T>
class list
{
private:
	size_t *next;
	T *key;
	size_t *prev;
	size_t free;//管理自由节点链表，单链表即可
	size_t head;//管理已用结点链表，双链表
	size_t list_size;//链表总大小，包括自由链表
	friend ostream& operator<< <T>(ostream&, const list &);
	size_t getFree();//获得自由节点
	void addFree(size_t index)
	{//添加空闲空间到自由链表
		next[index] = free;
		free = index;
	}
	void newList(size_t n)
	{//分配空间
		key = new T[n];
		next = new size_t[n];
		prev = new size_t[n];
	}
	void deleteList(size_t *p, T *k, size_t *n)
	{//收回空间
		delete[] p;
		delete[] k;
		delete[] n;
	}
	void copyList(size_t *p, T *k, size_t *n, size_t s)
	{//复制链表
		copy(p, p + s, prev);
		copy(n, n + s, next);
		copy(k, k + s, key);
	}
	void setFree(size_t start_index)
	{//设置自由链表
		next[list_size - 1] = NOEXIST;
		for (size_t i = start_index; i != list_size - 1; ++i)
			next[i] = i + 1;
		free = start_index;
	}
public:
	list() :free(NOEXIST), head(NOEXIST), list_size(8)//-1表示空，最初链表有8个空间
	{//初始化，设置free链表
		newList(list_size);
		setFree(0);
	}
	list(T *beg, T *end) :list() { insert(beg, end); }
	void setData(size_t index, const T &t) { key[index] = t; }
	T getData(size_t index)const { return key[index]; }
	void insert(const T&);
	void insert(T*, T*);
	size_t locate(const T&);
	void erase(const T&);
	void erase(size_t);
	void edit(const T&, const T&);
	bool empty() { return head == NOEXIST; }//链表是否为空
	~list()
	{
		deleteList(prev, key, next);
	}
	//bool full() { return free == -1; }
};

template <typename T>
inline size_t list<T>::getFree()
{
	if (free == NOEXIST)
	{//若自由链表已空，则需补充空间
		size_t *old_next = next, *old_prev = prev;
		T *old_key = key;
		size_t old_list_size = list_size;
		list_size *= 2;
		newList(list_size);
		copyList(old_prev, old_key, old_next, old_list_size);
		deleteList(old_prev,old_key,old_next);
		setFree(old_list_size);
	}
	size_t index = free;
	free = next[index];
	return index;
}

template <typename T>
void list<T>::insert(const T &t)
{
	size_t index = getFree();
	key[index] = t;
	if (head == NOEXIST)
	{//插入第一个节点
		next[index] = NOEXIST;
		prev[index] = NOEXIST;
		head = index;
	}
	else
	{
		next[index] = head;
		prev[head] = index;
		prev[index] = NOEXIST;
		head = index;
	}
}

template <typename T>
void list<T>::insert(T *beg, T *end)
{
	for (; beg != end; ++beg)
		insert(*beg);
}

template <typename T>
size_t list<T>::locate(const T &t)
{
	size_t p = head;
	while (p != NOEXIST && key[p] != t)
		p = next[p];
	return p;
}

template <typename T>
void list<T>::erase(size_t index)
{
	if (index == head)
		head = next[index];
	else
	{
		next[prev[index]] = next[index];
		if (next[index] != NOEXIST)
			prev[next[index]] = prev[index];
	}
	addFree(index);
}
template <typename T>
void list<T>::erase(const T &t)
{
	size_t index = locate(t);
	if (index != NOEXIST)
		erase(index);
}


template <typename T>
void list<T>::edit(const T &old_key, const T &new_key)
{
	size_t index = locate(old_key);
	if (index == NOEXIST)
	{
		cout << old_key << " isn't exist!" << endl;
		return;
	}
	key[index] = new_key;
}

template <typename T>
ostream& operator<<(ostream &out, const list<T> &lst)
{
	size_t p = lst.head;
	while (p != NOEXIST)
	{
		out << lst.key[p];
		if (lst.next[p] != NOEXIST) out << ' ';
		p = lst.next[p];
	}
	return out;
}

int main()
{
	int a[] = { 1, 2, 3, 4, 5, 6 };
	list<int> lst(a,a + 6);
	cout << lst << endl << endl;
	for (int i = 10; i != 20; ++i)
		lst.insert(i);
	cout << lst << endl << endl;
	for (int i = 1; i != 1000; ++i)
	{
		size_t f = lst.locate(i);
		if (f != NOEXIST)
			lst.erase(f);
		else
			lst.insert(-i);
	}
	cout << lst << endl;
	getchar();
	return 0;
}
