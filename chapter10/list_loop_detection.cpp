
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<assert.h>

using namespace std;

template <typename T>
struct node
{
	T value;
	node *next;
	node(const T &v) :value(v), next(nullptr){}
};

template <typename T>
class list
{
private:
	node<T> *head;
public:
	list() :head(new node(T())){}
	bool empty()const { return head->next == nullptr; }
	node* isLoop()const;
	size_t loopSize(node<T>*)const;
	node* loopEntry(node<T> *)const;
};

template <typename T>
node<T>* list<T>::isLoop()const
{
	assert(!empty());
	node<T> *slow = head->next, *fast = head->next;
	while (slow != nullptr && fast->next != nullptr)
	{
		slow = slow->next;
		fast = fast->next->next;
		if (slow == fast)
			return fast;
	}
	return nullptr;
}

template <typename T>
size_t list<T>::loopSize(node<T> *meet)const
{
	assert(meet != nullptr);
	node<T> *slow = meet, *fast = meet;
	size_t cnt = 0;
	while (slow != nullptr && fast->next != nullptr)
	{
		slow = slow->next;
		fast = fast->next->next;
		++cnt;
		if (slow == fast)
			break;
	}
	return cnt;
}

template <typename T>
node<T>* list<T>::loopEntry(node<T> *meet)const
{
	assert(meet != nullptr);
	node<T> *h = head->next;
	while (true)
	{
		meet = meet->next;
		h = h->next;
		if (h == meet)
			break;
	}
	return meet;
}