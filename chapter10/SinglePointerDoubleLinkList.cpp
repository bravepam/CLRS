
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>

using namespace std;
//������Ϊȫ�ֱ���������PREV�ڸ���������������Զָ��
//��ǰ�ڵ��ǰ����FIRST��ָ��PREV��ǰ����NEXT��ָ��ǰ�ڵ�ĺ��
unsigned long FIRST = 0, NEXT = 0, PREV = 0;

template <typename T>
struct node
{//�ڵ�ģ����
	T data;
	unsigned long np;
	node(const T &d = T()) :data(d), np(0){}
};

template <typename T>
class list
{//����ģ����
private:
	node<T> *head;//ͷβָ��
	node<T> *tail;
public:
	list() :head(NULL), tail(NULL){}
	void insert(const T&);
	void create();
	void erase(const T&);
	node<T>* locate(const T&);
	void seTraversal();//�������
	void reTraversal();//�������
	void edit(const T&, const T&);
	size_t size();
	bool empty() { return head == NULL; }
};

template <typename T>
void list<T>::insert(const T &t)
{//����β�巨
	NEXT = 0;
	node<T> *p = new node<T>(t);
	PREV = reinterpret_cast<unsigned long>(tail);
	if (head == NULL)
	{//����ǵ�һ���ڵ�
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
	return p;//���ص�pҪô��Ҫ���ҵĽڵ㣬ҪôΪ�ձ�ʾû�д˽ڵ�
}

template <typename T>
void list<T>::erase(const T &t)
{
	node<T> *p = locate(t), *r;
	if (p == NULL)
	{//û�д˽ڵ�
		cout << t << " isn't exist!" << endl;
		return;
	}
	r = p;//���±�ɾ�ڵ�
	NEXT = PREV ^ p->np;//PREV���ڲ���������Ϊ��ǰ�ڵ��ǰ��
	unsigned long ulp = reinterpret_cast<unsigned long>(p);
	if (p == head && p == tail) head = tail = NULL;//������ֻ��һ���ڵ�
	else if (p == head)
	{//��Ϊͷ���
		p = reinterpret_cast<node<T>* >(NEXT);
		p->np = PREV ^ ulp ^ p->np;
		head = p;
	}
	else if (p == tail)
	{//��Ϊβ�ڵ�
		p = reinterpret_cast<node<T>* >(PREV);
		p->np = p->np ^ ulp ^ NEXT;
		tail = p;
	}
	else
	{//һ�����
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