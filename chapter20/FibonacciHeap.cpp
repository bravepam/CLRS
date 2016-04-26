
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//斐波那契堆，默认最小堆

#include<iostream>
#include<cmath>
#include<vector>

using namespace std;

template <typename Key, typename Value>
struct fibonacci_heap_node
{//斐波那契堆节点类型
	Key key;
	Value value;
	bool mark = false;//标志是否曾经失去过一个孩子，初始时均为false
	size_t degree = 0;
	fibonacci_heap_node *parent = nullptr;
	fibonacci_heap_node *prev = nullptr;
	fibonacci_heap_node *next = nullptr;
	fibonacci_heap_node *child = nullptr;
	fibonacci_heap_node(const Key &k, const Value &v) :key(k), value(v){}
	void print(){ printf("key: %-6d value: %-6d degree: %-6d\n", key, value, degree); }
};

template <typename Key, typename Value, typename Compare = less<Key>>
class fibonacci_heap
{//斐波那契堆
public:
	typedef fibonacci_heap_node<Key, Value>						node;
	typedef Key													key_type;
	typedef Value												value_type;
private:
	node *head;//永远指向最小值节点
	Compare compare;//比较器
	size_t n = 0;//节点总数
private:
	void linkNode(node *&lhs, node *&rhs)
	{//将lhs所指节点链接为rhs所指节点的前驱
		if (rhs == nullptr)
		{//若rhs为空
			rhs = lhs;
			lhs->next = lhs;
			lhs->prev = lhs;
		}
		else
		{//否则
			lhs->next = rhs;
			rhs->prev->next = lhs;
			lhs->prev = rhs->prev;
			rhs->prev = lhs;
		}
	}
	void removeNode(node *p)
	{//将节点p从所属的双链表中移除掉
		if (p->next == p)
		{//若该双链表仅有一个节点
			p->next = nullptr;
			p->prev = nullptr;
		}
		else
		{//否则
			p->prev->next = p->next;
			p->next->prev = p->prev;
		}
	}
	void heapLink(node *big, node *small)
	{//将根节点关键字较大的树链为根关键字较小的树的孩子
		removeNode(big);//首先移除big
		linkNode(big, small->child);//再链为其孩子
		++small->degree;
		big->parent = small;
		big->mark = false;//1、成为一个节点的孩子时，清除标记位
	}
	void prune(node *p, node *par)
	{//剪枝，将树根为p的子树从父节点par剪掉
		if (par->degree == 1) par->child = nullptr;//可能需要重新设置孩子
		else if (par->child == p) par->child = p->next;
		removeNode(p);
		linkNode(p, head);//链到根表中
		p->parent = nullptr;
		p->mark = false;//2、成为一棵新树时，清除标记位
		--par->degree;
	}
	void removeChildsToRoot(node *p)
	{//将树根p下的孩子们全部移到根表中
		if (p->child != nullptr)
		{
			node *first = p->child, *last = p->child->prev;
			while (true)
			{//迭代，将p的每个孩子链到根表中
				node *curr = first;
				first = first->next;
				removeNode(curr);
				linkNode(curr, head);
				curr->parent = nullptr;
				if (curr == last) break;//若已经处理完最后一个，跳出循环
			}
		}
	}
	void cascadingPrune(node*);//级联剪枝
	void consolidate();//合并根表
	void print_aux(node*)const;
	void destroy(node*);

	fibonacci_heap& operator=(const fibonacci_heap&);
	fibonacci_heap(const fibonacci_heap&);
public:
	fibonacci_heap(node *h = nullptr, const Compare &c = Compare()) :head(h), compare(c){}
	fibonacci_heap(const Compare &c) :head(nullptr), compare(c){}
	node* insert(const Key&, const Value&);
	node* minimum()const { return head; }
	void FibHeapUnion(fibonacci_heap&);
	pair<Key, Value> extractMin();
	void decreaseKey(node*, const Key&);
	void erase(node*);
	bool empty()const { return head == nullptr; }
	size_t size()const { return n; }
	void print()const { print_aux(head); }
	~fibonacci_heap(){ destroy(head); }
};

template <typename Key, typename Value, typename Compare>
inline fibonacci_heap_node<Key, Value>*
fibonacci_heap<Key, Value, Compare>::insert(const Key &k, const Value &v)
{//插入一个节点
	node *p = new node(k, v);
	linkNode(p, head);//直接插到根表中
	if (compare(p->key, head->key))
		head = p;
	++n;
	return p;
}

template <typename Key, typename Value, typename Compare>
inline void fibonacci_heap<Key, Value, Compare>::FibHeapUnion(fibonacci_heap &rhs)
{//合并两个斐波那契堆
	if (rhs.empty())return;//若被合并堆空
	if (empty())
	{//若本堆空
		swap(head, rhs.head);
		swap(n, rhs.n);
		swap(compare, rhs.compare);
		return;
	}
	//链接两个双链表
	node *head_prev = head->prev;
	head_prev->next = rhs.head->prev;
	rhs.head->prev->prev->next = head;
	head->prev = rhs.head->prev->prev;
	rhs.head->prev->prev = head_prev;
	if (compare(rhs.head->key, head->key))
		head = rhs.head;
	n += rhs.n;
	rhs.head = nullptr; rhs.n = 0;
}

template <typename Key, typename Value, typename Compare>
pair<Key, Value> fibonacci_heap<Key, Value, Compare>::extractMin()
{//抽取堆最小值
	node *p = head;//记下最小值节点
	removeChildsToRoot(head);
	head->child = nullptr;
	removeNode(head);//从根表中移除head
	if (head->next == nullptr) head = nullptr;
	else//重新设置head
	{//且摘除该最小值节点后，对根表中的树进行合并
		head = head->next;
		consolidate();
	}
	--n;
	pair<Key, Value> tmp = pair<Key, Value>(p->key, p->value);//返回值
	delete p;
	return tmp;
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::consolidate()
{//合并堆中，即根表中，度数相同的树
	size_t max_degree = static_cast<size_t>(log(n) / log(2));
	vector<node*> temp(max_degree + 1);//存储各度数的树的根
	node *first = head;
	while (true)
	{//不断迭代，扫描每一棵树
		node *small = first;
		first = first->next;
		if (small->degree > max_degree)
		{//max_degree是指在合并后堆中的树的最大度数。但是在合并之前，可能出现某树的度数超过的情况。
			//这是因为发生一系列剪枝（但没有剪掉某树根的孩子），抽取（也没有抽取该树的节点）操作后，
			//节点数目减少，使得计算出来的最大度数较之前小，而此时该树的度依然维持在之前的水平，此时
			//就会发生这种情况，这在第23章实现prim算法时出现过，代码后附上当时的堆结构图。因而在此需要
			//将其所有孩子移到根表中，减少该树的度。
			removeChildsToRoot(small);
			small->degree = 0;
		}
		size_t d = small->degree;
		if (small == temp[d]) break;//当前将要处理的树已存在于temp中，则说明已经合并完毕，退出
		while (temp[d] != nullptr)
		{//若temp中有另外一棵和当前树的度数相同的树
			node *big = temp[d];
			if (!compare(small->key, big->key))//若当前树的根关键字较大(最小堆时)
				swap(small, big);//则交换
			//这个错误调试了好久，将要被合并的树恰巧是first所指向的，而且要放在swap之后，第一次排除
			//错误放在swap前面，后来在MST算法中，又出错，应该要放在这里，因为big与small可能会发生交换
			if (big == first) first = first->next;
			heapLink(big, small);//将big树链为small的孩子
			temp[d] = nullptr;
			++d;//生成了度增1的树，继续合并
		}
		temp[d] = small;//若不存在，则设置temp的相应槽位
	}
	head = nullptr;
	for (size_t i = 0; i != temp.size(); ++i)
	{//扫描temp
		if (temp[i] != nullptr && (head == nullptr || compare(temp[i]->key, head->key)))
			head = temp[i];//设置新的head，即最小值节点
	}
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::decreaseKey(node *p, const Key &k)
{//减小某一节点关键字
	if (!compare(k, p->key))
	{//若新关键字较大
		cerr << "greater key" << endl;
		return;
	}
	p->key = k;
	node *par = p->parent;
	if (par != nullptr && compare(p->key, par->key))
	{//若新关键字比父节点关键字小
		prune(p, par);//则剪掉以p为根的树，使其成为根表中一员
		cascadingPrune(par);//并级联剪枝父节点
	}
	if (compare(p->key, head->key))//测试是否需要重新设置head
		head = p;
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::cascadingPrune(node *p)
{//级联剪枝
	node *par = p->parent;
	if (par != nullptr)
	{//若p的父节点存在
		//若在此之前p没有失去孩子，言下之意是p从上次清除标记后到现在仅失去过一个孩子
		if (p->mark == false) p->mark = true;//则将其标为true，表明失去一个孩子
		else
		{//若现在失去的使其第二个孩子
			prune(p, par);//则将其剪枝
			p->mark = false;//3、清除标记位
			cascadingPrune(par);
		}
	}
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::erase(node *p)
{//删除某一节点
	node *p_min = minimum();
	decreaseKey(p, p_min->key - 1);
	extractMin();
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::print_aux(node *p)const
{//递归打印堆
	if (p == nullptr) return;
	node *first = p->next;
	while (true)
	{
		node *curr = first;
		first = first->next;
		print_aux(curr->child);
		curr->print();
		if (curr == p) break;//表明已经绕地球一圈，该结束了
	}
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::destroy(node *p)
{//销毁堆
	if (p == nullptr) return;
	node *first = p->next;
	while (true)
	{
		node *curr = first;
		first = first->next;
		destroy(curr->child);
		--n;
		if (curr == p)
		{
			delete curr;
			break;
		}
		else delete curr;
	}
}

int main()
{
	fibonacci_heap<int, int> fh1,fh2;
	for (int i = 0; i != 10; ++i)
	{
		if (i % 2 == 0) fh1.insert(i, 2 * i);
		else fh2.insert(i, 2 * i);
	}
	cout << "fh1" << endl;
	fh1.print(); cout << fh1.size() << endl;
	cout << "fh2" << endl;
	fh2.print(); cout << fh2.size() << endl;
	fh1.FibHeapUnion(fh2);
	cout << "union" << endl;
	fh1.print(); cout << fh1.size() << endl;
	while (!fh1.empty())
	{
		cout << "-------------" << endl;
		fh1.minimum()->print();
		cout << endl;
		fh1.extractMin();
		fh1.print();
	}
	getchar();
	return 0;
}