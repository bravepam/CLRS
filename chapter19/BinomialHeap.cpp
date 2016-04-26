
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<utility>

#define EXTREMUM 0x7fffffff //极值，取极大值则最小堆，取极小值则最大堆
using namespace std;

template < typename K, typename V>
struct binomial_heap_node
{//二项树节点
	K key;//键
	V value;//值
	size_t degree = 0;//度
	binomial_heap_node *parent = nullptr;
	binomial_heap_node *leftchild = nullptr;
	binomial_heap_node *sibling = nullptr;
	binomial_heap_node(const K &k, const V &v) :key(k), value(v){}
};

template <typename K, typename V, typename Compare = less<K> >
class binomial_heap
{//二项堆，左孩子右兄弟方式存储
public:
	typedef binomial_heap_node<K, V>		node;
	typedef binomial_heap					Bheap;
private:
	node *head;
	Compare compare;//键比较器，默认小于，为最小堆
	void heapLink(node *lhs, node *rhs)
	{//两棵二项树的链接
		lhs->parent = rhs;
		lhs->sibling = rhs->leftchild;
		rhs->leftchild = lhs;
		++rhs->degree;
	}
	void linkAtTail(node *&tail,node *curr)
	{//尾插法链二项树
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
	{//查找curr的前驱
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
{//插入元素
	node *curr = new node(k, v);
	if (head == nullptr) head = curr;//若为第一个节点
	else
	{//否则
		binomial_heap heap(curr,compare);
		BheapUnion(heap);
	}
	return curr;
}

template <typename K,typename V,typename Compare>
binomial_heap_node<K, V>* binomial_heap<K, V, Compare>::minimum()const
{//寻找最小/大元素，返回指针
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
{//获得最小/大值，返回pair<K,V>，即键值对
	node  *p_min = nullptr, *pre = nullptr, *curr = head;
	K min = head->key;//初始时head即为最值
	while (curr->sibling != nullptr)
	{//迭代，寻得最值，及其前驱
		if (compare(curr->sibling->key,min))
		{
			min = curr->sibling->key;
			pre = curr;
		}
		curr = curr->sibling;
	}
	if (pre == nullptr)
	{//若只有一个元素或者第一个元素即为所求
		p_min = head;
		head = p_min->sibling;
	}
	else
	{//否则
		p_min = pre->sibling;
		pre->sibling = p_min->sibling;
	}
	binomial_heap heap(p_min->leftchild,compare);//创建临时二项堆
	heap.reverse();//先逆置
	BheapUnion(heap);//再和原来的堆合并
	pair<K, V> return_value = pair<K, V>(p_min->key, p_min->value);//构造返回值
	delete p_min;//释放节点内存
	return return_value;
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::BheapUnion(binomial_heap &rhs)
{//二项堆的合并
	heapMerge(rhs);//先将两条链合并，按度的非递减顺序
	if (head == nullptr) return;
	node *prev = nullptr, *curr = head, *next = head->sibling;
	while (next != nullptr)
	{//遍历每一个二项树的根
		if ((curr->degree != next->degree) || (next->sibling != nullptr && next->sibling->degree
			== curr->degree))
		{//若当前树和下一棵树度不等，或者有三颗二项树的根的度相等
			prev = curr;
			curr = next;
		}//否则当前仅有两棵树度相等
		else if (compare(curr->key,next->key))
		{//若当前树的根的key较小
			curr->sibling = next->sibling;
			heapLink(next, curr);//则将下一棵二项树链为其左孩子
		}
		else
		{//否则，相反
			if (prev == nullptr) head = next;//若当前二项堆最前面两棵树的度相等，则修改head
			else prev->sibling = curr->sibling;
			heapLink(curr, next);
			curr = next;
		}
		next = curr->sibling;
	}
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::decreaseKey(node *p, const K &k)
{//减小某一节点的key，并调整堆,如果用的是最大堆，则该函数其实就是increaseKey
	if (!compare(k,p->key))
	{//若新值较大
		cout << "Error : greater/less key" << endl;
		return;
	}
	p->key = k;
	node *par = p->parent,*p_sib = p->sibling;
	while (par != nullptr && compare(p->key,par->key))
	{//自底向上调整
		//修改p和par的后继，即右兄弟
		p->sibling = par->sibling;
		par->sibling = p_sib;
		//如果p和par存在前驱，则修改它们前驱的右兄弟
		node *p_pre, *par_pre;
		if ((p_pre = findPre(p)) != nullptr)
			p_pre->sibling = par;
		if ((par_pre = findPre(par)) != nullptr)
			par_pre->sibling = p;

		if (par->parent != nullptr && par->parent->leftchild == par)
			par->parent->leftchild = p;
		
		node *p_child = p->leftchild;
		//设置p的孩子
		if (p->parent->leftchild == p)//如果p是其父亲par的左孩子，p的祖先其实就是par
			p->leftchild = par;
		else
		{//否则
			p->leftchild = par->leftchild;
			par->leftchild->parent = p;
		}
		p->parent = par->parent;//设置p的祖先
		par->parent = p;//设置par的祖先

		//设置par的孩子
		par->leftchild = p_child;
		if (p_child != nullptr)
			p_child->parent = par;
		std::swap(p->degree, par->degree);//交换度
		par = p->parent;
	}
}

template <typename K,typename V,typename Compare>
void binomial_heap<K,V,Compare>::erase(node *p)
{//删除指定节点
	node *p_min = minimum();
	decreaseKey(p, p_min->key - 1);
	extractMin();
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::heapMerge(binomial_heap &rhs)
{//按度非递减顺序合并两个堆
	if (rhs.empty()) return;//若至少有一堆空
	if (empty())
	{
		head = rhs.head;
		rhs.head = nullptr;
		return;
	}
	node *curr1 = head, *curr2 = rhs.head,*tail = nullptr;
	head = nullptr, rhs.head = nullptr;
	while (curr1 != nullptr && curr2 != nullptr)
	{//不断链接，直到有一堆为空
		if (curr1->degree <= curr2->degree)
		{
			linkAtTail(tail,curr1);//尾端链入
			curr1 = curr1->sibling;
		}
		else
		{
			linkAtTail(tail,curr2);
			curr2 = curr2->sibling;
		}
	}
	if (curr1 != nullptr) tail->sibling = curr1;//若本堆还有剩下
	else if (curr2 != nullptr) tail->sibling = curr2;//若另一堆还有剩下
	else tail->sibling = nullptr;//若均没有剩下
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::destroy(node *H)
{//销毁堆
	node *curr = H;
	while (curr != nullptr)
	{//销毁每一棵二项树
		destroy(curr->leftchild);//递归销毁每一个子堆
		node *p = curr;
		curr = curr->sibling;
		delete p;//释放该树根
	}
}

template <typename K,typename V,typename Compare>
void binomial_heap<K, V, Compare>::reverse()
{//逆置
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
{//后序遍历二项堆
	node *curr = H;
	while (curr != nullptr)
	{
		postTraversal(curr->leftchild);//递归
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