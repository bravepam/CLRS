
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>

using namespace std;
typedef int DataType;
const DataType dt = -100000;//��ʾ��ֵ
class Node;
class NodeUseCount
{//Node�������ָ����
private:
	friend class Node;//��Ԫ����Node�ܹ�ֱ�ӷ�����˽�г�Ա
	size_t use;//�����������м���ָ��ָ��˶���
	Node *next;
	NodeUseCount(Node *p) :next(p), use(1){}
};
class Node
{//�ڵ���
private:
	DataType data;
	NodeUseCount *pn;//ָ��˽ڵ��ָ�������
public:
	Node(DataType e = dt) :pn(new NodeUseCount(NULL)), data(e){}
	Node(const Node& rhs) :data(rhs.data), pn(rhs.pn) { ++pn->use; }//���ƹ��캯��
	Node& operator=(const Node&);//������ֵ������
	DataType getData()const { return data; }
	Node* getNext() { return pn->next; }
	void setData(DataType e){ data = e; }
	void setNext(Node* next) { pn->next = next; }
	~Node(){ if (--pn->use == 0)delete pn; }//��������
};
Node& Node::operator=(const Node& rhs)
{//���帳ֵ������
	++rhs.pn->use;
	if (--pn->use == 0)
		delete pn;
	data = rhs.data;
	pn = rhs.pn;
	return *this;
}
typedef class LinkList
{//������
private:
	Node *head;//����ͷָ�룬�����׽ڵ��
	//��Ϊ˽�г�Ա�Ҳ����壬�Խ�ֹ���ƹ���͸�ֵ
	LinkList(const LinkList &);
	LinkList& operator=(const LinkList&);
public:
	LinkList() :head(new Node()){}//���������
	Node* getHead()const { return head; }
	void create();//��������
	void insert(DataType);//����һ�����
	void del(DataType e);//ɾ��ֵΪe�Ľڵ�
	void edit(DataType e, DataType ee);//��ֵΪe�Ľڵ��ֵ��Ϊee
	Node* find(DataType e)const;//����ֵΪe�Ľڵ㣬�����ش˽ڵ�
	void print()const;//��ӡ����
	//���������ͷ���head����������ɾ�����нڵ㣬�ͷŴ�����������ڴ�
	void clear();//�������
	void destroy();//��������
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
{//����һ�����
	Node *p = new Node(e);
	p->setNext(head->getNext());
	head->setNext(p);
}
void LinkList::del(DataType e)
{//ɾ��ֵΪe�Ľڵ�
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
{//��ֵΪe�Ľڵ��ֵ��Ϊee
	Node *p = find(e);
	if (p->getNext() == NULL)
		cerr << "No the data,edit failure!" << endl;
	else
		p->getNext()->setData(ee);
}
Node* LinkList::find(DataType e)const
{//����ֵΪe�Ľڵ㣬�����ش˽ڵ�
	Node *p = head;
	while (p->getNext() != NULL && p->getNext()->getData() != e)
		p = p->getNext();
	return p;
}
void LinkList::print()const
{//��ӡ����
	Node *p = head->getNext();
	while (p != NULL)
	{
		cout << p->getData() << ' ';
		p = p->getNext();
	}
	cout << endl;
}
void LinkList::clear()
{//�������
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
{//��������
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
