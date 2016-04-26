
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<utility>

#define EXTREMUM 0x7fffffff //��ֵ��ȡ����ֵ����С�ѣ�ȡ��Сֵ������
using namespace std;

template < typename K, typename V>
struct binomial_heap_node
{//�������ڵ�
	K key;//��
	V value;//ֵ
	size_t degree = 0;//��
	binomial_heap_node *parent = nullptr;
	binomial_heap_node *leftchild = nullptr;
	binomial_heap_node *sibling = nullptr;
	binomial_heap_node(const K &k, const V &v) :key(k), value(v){}
};

template <typename K, typename V, typename Compare = less<K> >
class binomial_heap
{//����ѣ��������ֵܷ�ʽ�洢
public:
	typedef binomial_heap_node<K, V>		node;
	typedef binomial_heap					Bheap;
private:
	node *head;
	Compare compare;//���Ƚ�����Ĭ��С�ڣ�Ϊ��С��
	void heapLink(node *lhs, node *rhs)
	{//���ö�����������
		lhs->parent = rhs;
		lhs->sibling = rhs->leftchild;
		rhs->leftchild = lhs;
		++rhs->degree;
	}
	void linkAtTail(node *&tail,node *curr)
	{//β�巨��������
		if (head == nullptr)
		{
			head = curr;
			tail = curr;
		}
		else
		{
			tail->sibling = curr;
			tail = tail->sibling;
		}
	}
	node* findPre(node *curr)const
	{//����curr��ǰ��
		node *pre = nullptr;
		if (curr->parent == nullptr) pre = head;
		else if (curr->parent->leftchild == curr) return pre;
		else pre = curr->parent->leftchild;
		while (pre->sibling != curr)
			pre = pre->sibling;
		return pre;
	}
	void heapMerge(binomial_heap&);
	void postTraversal(node *)const;
	void destroy(node*);
	void reverse();

	binomial_heap& operator=(const binomial_heap&);
	binomial_heap(const binomial_heap&);
public:
	binomial_heap(node *h, Compare c = Compare()) :head(h), compare(c){}
	binomial_heap(Compare c = Compare()) :head(nullptr), compare(c){}
	node* insert(const K&, const V&);
	node* minimum()const;
	pair<K, V> extractMin();
	void BheapUnion(binomial_heap&);
	void decreaseKey(node*, const K&);
	void erase(node*);
	bool empty()const { return head == nullptr; }
	void print()const { postTraversal(head); }
	~binomial_heap(){ destroy(head); }
};

template <typename K,typename V,typename Compare>
binomial_heap_node<K, V>* binomial_heap<K, V, Compare>::insert(const K &k, const V &v)
{//����Ԫ��
	node *curr = new node(k, v);
	if (head == nullptr) head = curr;//��Ϊ��һ���ڵ�
	else
	{//����
		binomial_heap heap(curr,compare);
		BheapUnion(heap);
	}
	return curr;
}

template <typename K,typename V,typename Compare>
binomial_heap_node<K, V>* binomial_heap<K, V, Compare>::minimum()const
{//Ѱ����С/��Ԫ�أ�����ָ��
	node *p_min = nullptr, *curr = head;
	K min = EXTREMUM;
	while (curr != nullptr)
	{
		if (compare(curr->key,min))
		{
			min = curr->key;
			p_min = curr;
		}
		curr = curr->sibling;
	}
	return p_min;
}

template <typename K,typename V,typename Compare>
pair<K, V> binomial_heap<K, V, Compare>::extractMin()
{//�����С/��ֵ������pair<K,V>������ֵ��
	node  *p_min = nullptr, *pre = nullptr, *curr = head;
	K min = head->key;//��ʼʱhead��Ϊ��ֵ
	while (curr->sibling != nullptr)
	{//������Ѱ����ֵ������ǰ��
		if (compare(curr->sibling->key,min))
		{
			min = curr->sibling->key;
			pre = curr;
		}
		curr = curr->sibling;
	}
	if (pre == nullptr)
	{//��ֻ��һ��Ԫ�ػ��ߵ�һ��Ԫ�ؼ�Ϊ����
		p_min = head;
		head = p_min->sibling;
	}
	else
	{//����
		p_min = pre->sibling;
		pre->sibling = p_min->sibling;
	}
	binomial_heap heap(p_min->leftchild,compare);//������ʱ�����
	heap.reverse();//������
	BheapUnion(heap);//�ٺ�ԭ���ĶѺϲ�
	pair<K, V> return_value = pair<K, V>(p_min->key, p_min->value);//���췵��ֵ
	delete p_min;//�ͷŽڵ��ڴ�
	return return_value;
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::BheapUnion(binomial_heap &rhs)
{//����ѵĺϲ�
	heapMerge(rhs);//�Ƚ��������ϲ������ȵķǵݼ�˳��
	if (head == nullptr) return;
	node *prev = nullptr, *curr = head, *next = head->sibling;
	while (next != nullptr)
	{//����ÿһ���������ĸ�
		if ((curr->degree != next->degree) || (next->sibling != nullptr && next->sibling->degree
			== curr->degree))
		{//����ǰ������һ�����Ȳ��ȣ����������Ŷ������ĸ��Ķ����
			prev = curr;
			curr = next;
		}//����ǰ���������������
		else if (compare(curr->key,next->key))
		{//����ǰ���ĸ���key��С
			curr->sibling = next->sibling;
			heapLink(next, curr);//����һ�ö�������Ϊ������
		}
		else
		{//�����෴
			if (prev == nullptr) head = next;//����ǰ�������ǰ���������Ķ���ȣ����޸�head
			else prev->sibling = curr->sibling;
			heapLink(curr, next);
			curr = next;
		}
		next = curr->sibling;
	}
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::decreaseKey(node *p, const K &k)
{//��Сĳһ�ڵ��key����������,����õ������ѣ���ú�����ʵ����increaseKey
	if (!compare(k,p->key))
	{//����ֵ�ϴ�
		cout << "Error : greater/less key" << endl;
		return;
	}
	p->key = k;
	node *par = p->parent,*p_sib = p->sibling;
	while (par != nullptr && compare(p->key,par->key))
	{//�Ե����ϵ���
		//�޸�p��par�ĺ�̣������ֵ�
		p->sibling = par->sibling;
		par->sibling = p_sib;
		//���p��par����ǰ�������޸�����ǰ�������ֵ�
		node *p_pre, *par_pre;
		if ((p_pre = findPre(p)) != nullptr)
			p_pre->sibling = par;
		if ((par_pre = findPre(par)) != nullptr)
			par_pre->sibling = p;

		if (par->parent != nullptr && par->parent->leftchild == par)
			par->parent->leftchild = p;
		
		node *p_child = p->leftchild;
		//����p�ĺ���
		if (p->parent->leftchild == p)//���p���丸��par�����ӣ�p��������ʵ����par
			p->leftchild = par;
		else
		{//����
			p->leftchild = par->leftchild;
			par->leftchild->parent = p;
		}
		p->parent = par->parent;//����p������
		par->parent = p;//����par������

		//����par�ĺ���
		par->leftchild = p_child;
		if (p_child != nullptr)
			p_child->parent = par;
		std::swap(p->degree, par->degree);//������
		par = p->parent;
	}
}

template <typename K,typename V,typename Compare>
void binomial_heap<K,V,Compare>::erase(node *p)
{//ɾ��ָ���ڵ�
	node *p_min = minimum();
	decreaseKey(p, p_min->key - 1);
	extractMin();
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::heapMerge(binomial_heap &rhs)
{//���ȷǵݼ�˳��ϲ�������
	if (rhs.empty()) return;//��������һ�ѿ�
	if (empty())
	{
		head = rhs.head;
		rhs.head = nullptr;
		return;
	}
	node *curr1 = head, *curr2 = rhs.head,*tail = nullptr;
	head = nullptr, rhs.head = nullptr;
	while (curr1 != nullptr && curr2 != nullptr)
	{//�������ӣ�ֱ����һ��Ϊ��
		if (curr1->degree <= curr2->degree)
		{
			linkAtTail(tail,curr1);//β������
			curr1 = curr1->sibling;
		}
		else
		{
			linkAtTail(tail,curr2);
			curr2 = curr2->sibling;
		}
	}
	if (curr1 != nullptr) tail->sibling = curr1;//�����ѻ���ʣ��
	else if (curr2 != nullptr) tail->sibling = curr2;//����һ�ѻ���ʣ��
	else tail->sibling = nullptr;//����û��ʣ��
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::destroy(node *H)
{//���ٶ�
	node *curr = H;
	while (curr != nullptr)
	{//����ÿһ�ö�����
		destroy(curr->leftchild);//�ݹ�����ÿһ���Ӷ�
		node *p = curr;
		curr = curr->sibling;
		delete p;//�ͷŸ�����
	}
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::reverse()
{//����
	node *curr = head,*r;
	head = nullptr;
	while (curr != nullptr)
	{
		curr->parent = nullptr; r = curr;
		curr = curr->sibling;
		if (head == nullptr)
		{
			head = r;
			r->sibling = nullptr;
		}
		else
		{
			r->sibling = head;
			head = r;
		}
	}
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::postTraversal(node *H)const
{//������������
	node *curr = H;
	while (curr != nullptr)
	{
		postTraversal(curr->leftchild);//�ݹ�
		printf("key: %-6d value: %-6d degree: %-6d\n", curr->key, curr->value,curr->degree);
		curr = curr->sibling;
	}
}

#include<vector>

int main()
{
	binomial_heap<int, int> bh;
	vector<binomial_heap_node<int, int>*> ptr(10);
	for (int i = 0; i != 10; i++)
		ptr[i] = bh.insert(i, 2 * i);
	for (size_t i = 0; i != ptr.size(); ++i)
		cout << ptr[i]->key << ' ';
	cout << endl;
	//binomial_heap_node<int, int> *p = bh.minimum();
	for (size_t i = 0; i != ptr.size(); i += 2)
		bh.decreaseKey(ptr[i], ptr[i]->key - 2);
	for (size_t i = 0; i != ptr.size(); ++i)
		cout << ptr[i]->key << ' ';
	cout << endl;
	bh.print();
	cout << endl;
	while (!bh.empty())
	{
		cout << bh.minimum()->key << endl;
		bh.extractMin();
	}
	getchar();
	return 0;
}