
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>

using namespace std;
//三个均为全局变量，其中PREV在各个函数过程中永远指向
//当前节点的前驱，FIRST则指向PREV的前驱，NEXT则指向当前节点的后继
unsigned long FIRST = 0, NEXT = 0, PREV = 0;

template <typename T>
struct node
{//节点模板类
	T data;
	unsigned long np;
	node(const T &d = T()) :data(d), np(0){}
};

template <typename T>
class list
{//链表模板类
private:
	node<T> *head;//头尾指针
	node<T> *tail;
public:
	list() :head(NULL), tail(NULL){}
	void insert(const T&);
	void create();
	void erase(const T&);
	node<T>* locate(const T&);
	void seTraversal();//正向遍历
	void reTraversal();//反向遍历
	void edit(const T&, const T&);
	size_t size();
	bool empty() { return head == NULL; }
};

template <typename T>
void list<T>::insert(const T &t)
{//采用尾插法
	NEXT = 0;
	node<T> *p = new node<T>(t);
	PREV = reinterpret_cast<unsigned long>(tail);
	if (head == NULL)
	{//如果是第一个节点
		head = p;
		tail = p;
		p->np = PREV ^ NEXT;
	}
	else
	{
		tail = p;
		p->np = PREV ^ NEXT;
		node<T> *q = reinterpret_cast<node<T> *>(PREV);
		q->np = q->np ^ reinterpret_cast<unsigned long>(p);
	}
}

template <typename T>
void list<T>::create()
{
	size_t N;
	T t;
	cout << "Enter the number of nodes: ";
	cin >> N;
	cout << "Enter " << N << " element(s)" << endl;
	for (int i = 0; i != N; ++i)
	{
		cin >> t;
		insert(t);
	}
}

template <typename T>
node<T>* list<T>::locate(const T &t)
{
	PREV = 0;
	node<T> *p = head, *r = head;
	while (p != NULL && p->data != t)
	{
		p = reinterpret_cast<node<T>* >(p->np ^ PREV);
		PREV = reinterpret_cast<unsigned long>(r);
		r = p;
	}
	return p;//返回的p要么是要查找的节点，要么为空表示没有此节点
}

template <typename T>
void list<T>::erase(const T &t)
{
	node<T> *p = locate(t), *r;
	if (p == NULL)
	{//没有此节点
		cout << t << " isn't exist!" << endl;
		return;
	}
	r = p;//记下被删节点
	NEXT = PREV ^ p->np;//PREV已在查找中设置为当前节点的前驱
	unsigned long ulp = reinterpret_cast<unsigned long>(p);
	if (p == head && p == tail) head = tail = NULL;//若链表只有一个节点
	else if (p == head)
	{//若为头结点
		p = reinterpret_cast<node<T>* >(NEXT);
		p->np = PREV ^ ulp ^ p->np;
		head = p;
	}
	else if (p == tail)
	{//若为尾节点
		p = reinterpret_cast<node<T>* >(PREV);
		p->np = p->np ^ ulp ^ NEXT;
		tail = p;
	}
	else
	{//一般情况
		p = reinterpret_cast<node<T>* >(PREV);
		p->np = p->np ^ ulp ^ NEXT;
		p = reinterpret_cast<node<T>* >(NEXT);
		p->np = PREV ^ ulp ^ p->np;
	}
	delete r;
}

template <typename T>
void list<T>::edit(const T &old, const T &New)
{
	node<T> *p = locate(old);
	if (p == NULL)
		cout << t << " isn't exist!" << endl;
	else p->data = New;
}

template <typename T>
size_t list<T>::size()
{
	size_t count = 0;
	PREV = 0;
	node<T> *p = head, *r = head;
	while (p != NULL)
	{
		++count;
		p = reinterpret_cast<node<T>* >(p->np ^ PREV);
		PREV = reinterpret_cast<unsigned long>(r);
		r = p;
	}
	return count;
}

template <typename T>
void list<T>::seTraversal()
{
	PREV = 0;
	node<T> *p = head, *r = head;
	while (p != NULL)
	{
		cout << p->data;
		p = reinterpret_cast<node<T>* >(p->np ^ PREV);
		PREV = reinterpret_cast<unsigned long>(r);
		r = p;
		if (p != NULL) cout << ' ';
		else cout << endl;
	}
}

template <typename T>
void list<T>::reTraversal()
{
	NEXT = 0;
	node<T> *p = tail, *r = tail;
	while (p != NULL)
	{
		cout << p->data;
		p = reinterpret_cast<node<T>* >(p->np ^ NEXT);
		NEXT = reinterpret_cast<unsigned long>(r);
		r = p;
		if (p != NULL) cout << ' ';
		else cout << endl;
	}
}

int main()
{
	list<int> lst;
	cout << "lst.create();" << endl;
	lst.create();
	cout << "lst.erase();" << endl;
	lst.erase(3);
	lst.locate(1);
	lst.insert(100);
	cout << "lst.seTraversal();" << endl;
	lst.seTraversal();
	cout << "lst.reTraversal();" << endl;
	lst.reTraversal();
	getchar();
	getchar();
	return 0;
}