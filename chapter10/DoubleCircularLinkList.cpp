
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
using namespace std;

typedef int DataType;
const int dt = -1000000;

class Node;
class UseCount
{
	friend class  Node;
	size_t use;
	Node *next;
	Node *prev;
	UseCount():use(1),next(NULL),prev(NULL){}
};

class Node
{
private:
	DataType data;
	UseCount *pu;
public:
	Node(DataType e = dt):data(e),pu(new UseCount()){}
	Node(const Node & rhs):data(rhs.data),pu(rhs.pu){++pu->use;}
	Node& operator=(const Node &);
	DataType getData(){return data;}
	void setData(DataType e){data = e;}
	Node * getNext(){return pu->next;}
	void setNext(Node *next){pu->next = next;}
	Node * getPrev(){return pu->prev;}
	void setPrev(Node *prev){pu->prev = prev;}
	~Node(){if(--pu->use == 0) delete pu;}
};

Node & Node::operator=(const Node &rhs)
{
	++rhs.pu->use;
	if(--pu->use == 0)
		delete pu;
	data = rhs.data;
	pu = rhs.pu;
	return *this;
}

typedef class DoubleCircularLinkList
{//˫��ѭ������
private:
	Node *head;
	DoubleCircularLinkList(const DoubleCircularLinkList&);
	DoubleCircularLinkList & operator=(const DoubleCircularLinkList&);
public:
	DoubleCircularLinkList():head(new Node())
	{//���캯��
		head->setNext(head);
		head->setPrev(head);
	}
	void pushBack(DataType);//������β������Ԫ��
	Node * insert(size_t,DataType);//��ĳλ�ò���Ԫ��
	void create();//��������
	void create(DataType *,DataType *);//����ĳһ��ΧԪ�ؽ�������
	Node * find(DataType);//����ĳԪ�أ�����λ��
	Node * find(size_t);//��������ڼ���Ԫ��λ��
	void edit(DataType,DataType);//�޸�ĳԪ��
	void edit(size_t,DataType);//�޸ĵڼ���Ԫ��
	bool erase(DataType);//ɾ��ĳԪ��
	bool erase(size_t);//ɾ���ڼ���Ԫ��
	size_t size();//������
	bool empty();//�п�
	void clear();//�������
	void destroy();//��������
	DataType max();//�������ֵ
	void sequentialTravel();//�������
	void reverseTravel();//�������
	DataType  back();//������һ��Ԫ��
	~DoubleCircularLinkList();//��������
}DCLinkList;

void DCLinkList::pushBack(DataType e)
{
	Node *p = new Node(e);
	p->setPrev(head->getPrev());
	head->getPrev()->setNext(p);
	head->setPrev(p);
	p->setNext(head);
}

Node * DCLinkList::insert(size_t size,DataType e)
{
	Node *p = head;
	while(--size > 0)
	{
		p = p->getNext();
		if(p == head)
		{
			cerr << "Longer size!" << endl;
			return p;
		}
	}
	Node *q = new Node(e);
	q->setNext(p->getNext());
	p->getNext()->setPrev(q);
	p->setNext(q);
	q->setPrev(p);
	return q;
}

void DCLinkList::create()
{
	DataType e;
	size_t N;
	cout << "Enter the number of node: ";
	cin >> N;
	cout << "Please enter " << N << "element(s)" << endl;
	for(size_t i = 0;i < N;i++)
	{
		cin >> e;
		pushBack(e);
	}
}

void DCLinkList::create(DataType *beg,DataType *end)
{
	while(beg != end)
		pushBack(*beg++);
}

Node * DCLinkList::find(DataType e)
{
	Node *p = head->getNext();
	while(p != head && p->getData() != e)
		p = p->getNext();
	return p;
}

Node * DCLinkList::find(size_t size)
{
	Node *p = head;
	while(--size > 0 )
		p = p->getNext();
	return p->getNext();
}

void DCLinkList::edit(DataType e,DataType ee)
{
	Node *p = find(e);
	p->setData(ee);
}

void DCLinkList::edit(size_t size,DataType e)
{
	Node *p = find(size);
	p->setData(e);
}

DataType DCLinkList::max()
{
	Node *p = head->getNext();
	int max = dt;
	while(p != head)
	{
		if(p->getData() > max)
			max = p->getData();
		p = p->getNext();
	}
	return max;
}

void DCLinkList::sequentialTravel()
{
	Node *p = head->getNext();
	while(p != head)
	{
		cout << p->getData();
		if(p->getNext() != head)
			cout << ' ';
		p = p->getNext();
	}
}

void DCLinkList::reverseTravel()
{
	Node *p = head->getPrev();
	while(p != head)
	{
		cout << p->getData();
		if(p->getPrev() != head)
			cout << ' ';
		p = p->getPrev();
	}
}

DataType  DCLinkList::back()
{
	return head->getPrev()->getData();
}

bool DCLinkList::erase(DataType e)
{
	Node *p = find(e);
	if(p != head)
	{
		p->getNext()->setPrev(p->getPrev());
		p->getPrev()->setNext(p->getNext());
		delete p;
		return true;
	}
	return false;
}

bool DCLinkList::erase(size_t size)
{
	Node *p = find(size);
	if(p != head)
	{
		p->getNext()->setPrev(p->getPrev());
		p->getPrev()->setNext(p->getNext());
		delete p;
		return true;
	}
	return false;
}

size_t DCLinkList::size()
{
	Node *p = head->getNext();
	size_t count = 0;
	while(p != head)
	{
		++count;
		p = p->getNext();
	}
	return count;
}

bool DCLinkList::empty()
{
	return (head->getNext() == head) ? true:false;
}

void DCLinkList::clear()
{
	Node *p = head->getNext();
	while(p != head)
	{
		Node *q = p;
		p = p->getNext();
		head->setNext(p);
		p->setPrev(head);
		delete q;
	}
}

void DCLinkList::destroy()
{
	clear();
	delete head;
}

DCLinkList::~DoubleCircularLinkList()
{
	destroy();
}


