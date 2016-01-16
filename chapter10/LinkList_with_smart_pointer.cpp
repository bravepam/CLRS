
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>

using namespace std;
typedef int DataType;
const DataType dt = -100000;//表示空值
class Node;
class NodeUseCount
{//Node类的智能指针类
private:
	friend class Node;//友元，让Node能够直接访问其私有成员
	size_t use;//计数，表明有几个指针指向此对象
	Node *next;
	NodeUseCount(Node *p) :next(p), use(1){}
};
class Node
{//节点类
private:
	DataType data;
	NodeUseCount *pn;//指向此节点的指针类对象
public:
	Node(DataType e = dt) :pn(new NodeUseCount(NULL)), data(e){}
	Node(const Node& rhs) :data(rhs.data), pn(rhs.pn) { ++pn->use; }//复制构造函数
	Node& operator=(const Node&);//声明赋值操作符
	DataType getData()const { return data; }
	Node* getNext() { return pn->next; }
	void setData(DataType e){ data = e; }
	void setNext(Node* next) { pn->next = next; }
	~Node(){ if (--pn->use == 0)delete pn; }//析构函数
};
Node& Node::operator=(const Node& rhs)
{//定义赋值操作符
	++rhs.pn->use;
	if (--pn->use == 0)
		delete pn;
	data = rhs.data;
	pn = rhs.pn;
	return *this;
}
typedef class LinkList
{//链表类
private:
	Node *head;//链表头指针，含有首节点的
	//设为私有成员且不定义，以禁止复制构造和赋值
	LinkList(const LinkList &);
	LinkList& operator=(const LinkList&);
public:
	LinkList() :head(new Node()){}//构造空链表
	Node* getHead()const { return head; }
	void create();//建立链表
	void insert(DataType);//插入一个结点
	void del(DataType e);//删除值为e的节点
	void edit(DataType e, DataType ee);//将值为e的节点的值改为ee
	Node* find(DataType e)const;//查找值为e的节点，并返回此节点
	void print()const;//打印链表
	//清空链表保留头结点head，而销毁则删除所有节点，释放此链表的所有内存
	void clear();//清空链表
	void destroy();//销毁链表
	~LinkList();
}list;

void LinkList::create()
{
	DataType dt;
	cout << "Enter some characters(Ctrl+Z to end):" << endl;
	while (cin >> dt)
		insert(dt);
	cin.clear();
}
void LinkList::insert(DataType e)
{//插入一个结点
	Node *p = new Node(e);
	p->setNext(head->getNext());
	head->setNext(p);
}
void LinkList::del(DataType e)
{//删除值为e的节点
	Node *p = find(e);
	if (p->getNext() == NULL)
		cerr << "No the data,delete failure!" << endl;
	else
	{
		Node *q = p->getNext();
		p->setNext(q->getNext());
		delete q;
	}
}
void LinkList::edit(DataType e, DataType ee)
{//将值为e的节点的值改为ee
	Node *p = find(e);
	if (p->getNext() == NULL)
		cerr << "No the data,edit failure!" << endl;
	else
		p->getNext()->setData(ee);
}
Node* LinkList::find(DataType e)const
{//查找值为e的节点，并返回此节点
	Node *p = head;
	while (p->getNext() != NULL && p->getNext()->getData() != e)
		p = p->getNext();
	return p;
}
void LinkList::print()const
{//打印链表
	Node *p = head->getNext();
	while (p != NULL)
	{
		cout << p->getData() << ' ';
		p = p->getNext();
	}
	cout << endl;
}
void LinkList::clear()
{//清空链表
	Node *H = head->getNext();
	head->setNext(NULL);
	while (H->getNext() != NULL)
	{
		Node *p = H;
		H = H->getNext();
		delete p;
	}
	delete H;
}
void LinkList::destroy()
{//销毁链表
	clear();
	delete head;
}
LinkList::~LinkList(){ destroy(); }

int main()
{
	list li;
	char op;
	DataType dchar;
	cout << "Enter your choice:" << endl
		<< "c->Create a->Insert d->delete f->find  p->print e->clear" << endl;
	cin >> op;
	do
	{
		switch (op)
		{
		case 'c':
			li.create();
			if (li.getHead()->getNext() != NULL)
				cout << "Successfully!" << endl;
			else
				cout << "Unsuccessfully!" << endl;
			break;
		case 'a':
			cout << "Enter the data" << endl;
			cin >> dchar;
			li.insert(dchar);
			if (li.getHead()->getNext()->getData() == dchar)
				cout << "Successfully!" << endl;
			else
				cout << "Unsuccessfully!" << endl;
			break;
		case 'd':
			cout << "Enter the data" << endl;
			cin >> dchar;
			li.del(dchar);
			break;
		case 'f':
			cout << "Enter the data" << endl;
			cin >> dchar;
			if ((li.find(dchar)->getNext()) != NULL)
				cout << "Exist!" << endl;
			else
				cout << "No the data!" << endl;
			break;
		case 'p':
			if (li.getHead()->getNext() == NULL)
				cout << "No any data!" << endl;
			else
				li.print();
			break;
		case 'e':
			if (li.getHead()->getNext() == NULL)
				cout << "No any data!" << endl;
			else
				li.clear();
			break;
		case 's':
			li.destroy();
		default:
			cout << "Wrong choice!" << endl;
		}
		cout << "Enter your choice:" << endl
			<< "c->Create a->Insert d->delete f->find  p->print e->clear q->quit  s->destroy" << endl;
		cin >> op;
	} while (op != 'q');
	return 0;
}
