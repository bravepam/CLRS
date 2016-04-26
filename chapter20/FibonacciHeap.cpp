
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//쳲������ѣ�Ĭ����С��

#include<iostream>
#include<cmath>
#include<vector>

using namespace std;

template <typename Key, typename Value>
struct fibonacci_heap_node
{//쳲������ѽڵ�����
	Key key;
	Value value;
	bool mark = false;//��־�Ƿ�����ʧȥ��һ�����ӣ���ʼʱ��Ϊfalse
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
{//쳲�������
public:
	typedef fibonacci_heap_node<Key, Value>						node;
	typedef Key													key_type;
	typedef Value												value_type;
private:
	node *head;//��Զָ����Сֵ�ڵ�
	Compare compare;//�Ƚ���
	size_t n = 0;//�ڵ�����
private:
	void linkNode(node *&lhs, node *&rhs)
	{//��lhs��ָ�ڵ�����Ϊrhs��ָ�ڵ��ǰ��
		if (rhs == nullptr)
		{//��rhsΪ��
			rhs = lhs;
			lhs->next = lhs;
			lhs->prev = lhs;
		}
		else
		{//����
			lhs->next = rhs;
			rhs->prev->next = lhs;
			lhs->prev = rhs->prev;
			rhs->prev = lhs;
		}
	}
	void removeNode(node *p)
	{//���ڵ�p��������˫�������Ƴ���
		if (p->next == p)
		{//����˫�������һ���ڵ�
			p->next = nullptr;
			p->prev = nullptr;
		}
		else
		{//����
			p->prev->next = p->next;
			p->next->prev = p->prev;
		}
	}
	void heapLink(node *big, node *small)
	{//�����ڵ�ؼ��ֽϴ������Ϊ���ؼ��ֽ�С�����ĺ���
		removeNode(big);//�����Ƴ�big
		linkNode(big, small->child);//����Ϊ�亢��
		++small->degree;
		big->parent = small;
		big->mark = false;//1����Ϊһ���ڵ�ĺ���ʱ��������λ
	}
	void prune(node *p, node *par)
	{//��֦��������Ϊp�������Ӹ��ڵ�par����
		if (par->degree == 1) par->child = nullptr;//������Ҫ�������ú���
		else if (par->child == p) par->child = p->next;
		removeNode(p);
		linkNode(p, head);//����������
		p->parent = nullptr;
		p->mark = false;//2����Ϊһ������ʱ��������λ
		--par->degree;
	}
	void removeChildsToRoot(node *p)
	{//������p�µĺ�����ȫ���Ƶ�������
		if (p->child != nullptr)
		{
			node *first = p->child, *last = p->child->prev;
			while (true)
			{//��������p��ÿ����������������
				node *curr = first;
				first = first->next;
				removeNode(curr);
				linkNode(curr, head);
				curr->parent = nullptr;
				if (curr == last) break;//���Ѿ����������һ��������ѭ��
			}
		}
	}
	void cascadingPrune(node*);//������֦
	void consolidate();//�ϲ�����
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
{//����һ���ڵ�
	node *p = new node(k, v);
	linkNode(p, head);//ֱ�Ӳ嵽������
	if (compare(p->key, head->key))
		head = p;
	++n;
	return p;
}

template <typename Key, typename Value, typename Compare>
inline void fibonacci_heap<Key, Value, Compare>::FibHeapUnion(fibonacci_heap &rhs)
{//�ϲ�����쳲�������
	if (rhs.empty())return;//�����ϲ��ѿ�
	if (empty())
	{//�����ѿ�
		swap(head, rhs.head);
		swap(n, rhs.n);
		swap(compare, rhs.compare);
		return;
	}
	//��������˫����
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
{//��ȡ����Сֵ
	node *p = head;//������Сֵ�ڵ�
	removeChildsToRoot(head);
	head->child = nullptr;
	removeNode(head);//�Ӹ������Ƴ�head
	if (head->next == nullptr) head = nullptr;
	else//��������head
	{//��ժ������Сֵ�ڵ�󣬶Ը����е������кϲ�
		head = head->next;
		consolidate();
	}
	--n;
	pair<Key, Value> tmp = pair<Key, Value>(p->key, p->value);//����ֵ
	delete p;
	return tmp;
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::consolidate()
{//�ϲ����У��������У�������ͬ����
	size_t max_degree = static_cast<size_t>(log(n) / log(2));
	vector<node*> temp(max_degree + 1);//�洢�����������ĸ�
	node *first = head;
	while (true)
	{//���ϵ�����ɨ��ÿһ����
		node *small = first;
		first = first->next;
		if (small->degree > max_degree)
		{//max_degree��ָ�ںϲ�����е������������������ںϲ�֮ǰ�����ܳ���ĳ���Ķ��������������
			//������Ϊ����һϵ�м�֦����û�м���ĳ�����ĺ��ӣ�����ȡ��Ҳû�г�ȡ�����Ľڵ㣩������
			//�ڵ���Ŀ���٣�ʹ�ü����������������֮ǰС������ʱ�����Ķ���Ȼά����֮ǰ��ˮƽ����ʱ
			//�ͻᷢ��������������ڵ�23��ʵ��prim�㷨ʱ���ֹ���������ϵ�ʱ�Ķѽṹͼ������ڴ���Ҫ
			//�������к����Ƶ������У����ٸ����Ķȡ�
			removeChildsToRoot(small);
			small->degree = 0;
		}
		size_t d = small->degree;
		if (small == temp[d]) break;//��ǰ��Ҫ��������Ѵ�����temp�У���˵���Ѿ��ϲ���ϣ��˳�
		while (temp[d] != nullptr)
		{//��temp��������һ�ú͵�ǰ���Ķ�����ͬ����
			node *big = temp[d];
			if (!compare(small->key, big->key))//����ǰ���ĸ��ؼ��ֽϴ�(��С��ʱ)
				swap(small, big);//�򽻻�
			//�����������˺þã���Ҫ���ϲ�����ǡ����first��ָ��ģ�����Ҫ����swap֮�󣬵�һ���ų�
			//�������swapǰ�棬������MST�㷨�У��ֳ���Ӧ��Ҫ���������Ϊbig��small���ܻᷢ������
			if (big == first) first = first->next;
			heapLink(big, small);//��big����Ϊsmall�ĺ���
			temp[d] = nullptr;
			++d;//�����˶���1�����������ϲ�
		}
		temp[d] = small;//�������ڣ�������temp����Ӧ��λ
	}
	head = nullptr;
	for (size_t i = 0; i != temp.size(); ++i)
	{//ɨ��temp
		if (temp[i] != nullptr && (head == nullptr || compare(temp[i]->key, head->key)))
			head = temp[i];//�����µ�head������Сֵ�ڵ�
	}
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::decreaseKey(node *p, const Key &k)
{//��Сĳһ�ڵ�ؼ���
	if (!compare(k, p->key))
	{//���¹ؼ��ֽϴ�
		cerr << "greater key" << endl;
		return;
	}
	p->key = k;
	node *par = p->parent;
	if (par != nullptr && compare(p->key, par->key))
	{//���¹ؼ��ֱȸ��ڵ�ؼ���С
		prune(p, par);//�������pΪ��������ʹ���Ϊ������һԱ
		cascadingPrune(par);//��������֦���ڵ�
	}
	if (compare(p->key, head->key))//�����Ƿ���Ҫ��������head
		head = p;
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::cascadingPrune(node *p)
{//������֦
	node *par = p->parent;
	if (par != nullptr)
	{//��p�ĸ��ڵ����
		//���ڴ�֮ǰpû��ʧȥ���ӣ�����֮����p���ϴ������Ǻ����ڽ�ʧȥ��һ������
		if (p->mark == false) p->mark = true;//�����Ϊtrue������ʧȥһ������
		else
		{//������ʧȥ��ʹ��ڶ�������
			prune(p, par);//�����֦
			p->mark = false;//3��������λ
			cascadingPrune(par);
		}
	}
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::erase(node *p)
{//ɾ��ĳһ�ڵ�
	node *p_min = minimum();
	decreaseKey(p, p_min->key - 1);
	extractMin();
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::print_aux(node *p)const
{//�ݹ��ӡ��
	if (p == nullptr) return;
	node *first = p->next;
	while (true)
	{
		node *curr = first;
		first = first->next;
		print_aux(curr->child);
		curr->print();
		if (curr == p) break;//�����Ѿ��Ƶ���һȦ���ý�����
	}
}

template <typename Key, typename Value, typename Compare>
void fibonacci_heap<Key, Value, Compare>::destroy(node *p)
{//���ٶ�
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