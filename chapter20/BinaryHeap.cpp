
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<vector>
#include<functional>

using namespace std;

//#define MINIMUM_HEAP_H

#ifdef MINIMUM_HEAP_H	//如果定义了该值
const static int EXTREAM = 0x7fffffff;//则表明实现的是最小堆
#else
const static int EXTREAM = 0x80000000;
#endif

template <typename Key, typename Handle = void*>
struct node
{//堆结点
	Key key;//堆关键字，通常是应用对象的优先权
	Handle obj_handle;//与该关键字关联的句柄，指向外部应用对象，可能是指针,不可变
	node(const Key &k, const Handle &h_) :key(k), obj_handle(h_){}
	node() :key(), obj_handle(){}
	Handle getHandle()const { return obj_handle; }
};

template <typename Handle>
struct doNothing :public binary_function < Handle, int, void >
{//默认的用于在堆中修改外部对象的函数对象句柄
	void operator()(const Handle &hd, int h)
	{//不作任何事
		return;
	}
};

template <typename Key,typename Handle = void*,class EditObjIndex = doNothing<Handle>,class CompareKey = greater<Key>>
class heap//类型参数分别为关键字类型，句柄类型，编辑堆结点句柄所对应的外部对象的句柄，比较器
{//二叉堆，默认最大堆
private:
	typedef node<Key, Handle>				node;
	vector<node> h;
	int size;//堆大小
	EditObjIndex editIndex;
	CompareKey compare;
public:
	//四种不同的构造函数
	heap() :h(), editIndex(), compare(),size(0){}
	heap(const EditObjIndex &eh) :editIndex(eh), compare(), h(), size(0){}
	heap(const CompareKey &comp) :compare(comp), editIndex(), h(),size(0){}
	heap(const EditObjIndex &eh, const CompareKey &comp) :editIndex(eh), compare(comp), h(), size(0){}
	bool empty()const { return size == 0; }
	size_t length()const { return size; }
	Key maximum()const { return h[0].key; }//如果是最小堆，则返回的是最小关键字
	void insert(const Key&,const Handle& = Handle());
	Key extractMax();
	void increaseKey(int, const Key&);
};

template <typename K,typename H,class EOH,class CK>
K heap<K, H, EOH, CK>::extractMax()
{//抽取最大值，并调整堆
	K k = h[0].key;
	h[0] = h[--size];//将最后一个元素放到堆根部
	editIndex(h[size].obj_handle, 0);//修改该元素关联的外部对象在堆中的句柄
	int left = 1, right = 2, i = 0, largest = i;
	while (left < size)
	{//从上至下开始调整堆
		if (left < size && compare(h[left].key, h[largest].key))
			largest = left;
		if (right < size && compare(h[right].key, h[largest].key))
			largest = right;//确定最大值
		if (largest == i) break;//如果本身最大，则退出，不再调整
		else 
		{//否则
			std::swap(h[largest], h[i]);//交换两元素
			//并修改这两个元素关联的外部对象的堆中句柄
			editIndex(h[largest].obj_handle, largest);
			editIndex(h[i].obj_handle, i);
		}
		left = 2 * largest + 1, right = 2 * largest + 2,i = largest;//修改，以继续迭代
	}
	return k;//返回最大值
}

template <typename K,typename H,class EOH,class CK>
void heap<K, H, EOH, CK>::increaseKey(int index, const K &k)
{//增加指定元素的关键字值，如果是最小堆，该函数即表示减小关键字值
	if (!compare(k, h[index].key))
	{//若新关键字不适合
		#ifdef MINIMUM_HEAP_H
			cout << "Error:greater key" << endl;
		#else
			cout << "Error:less key" << endl;
		#endif
		return;
	}
	h[index].key = k;
	int parent = (index - 1) / 2;
	while (parent >= 0 && compare(k,h[parent].key))//若比父节点大
	{//自下而上调整堆
		std::swap(h[index], h[parent]);
		editIndex(h[parent].obj_handle, parent);
		editIndex(h[index].obj_handle, index);
		if (parent == 0) break;//若已到根部
		else
		{//否则继续迭代
			index = parent;
			parent = (index - 1) / 2;
		}
	}
}

template <typename K,typename H,class EOH,class CK>
void heap<K, H, EOH, CK>::insert(const K &k,const H &hd = H())
{//插入元素，参数分别为外部对象的权值和唯一性标识
	if (size == h.size()) h.resize(2 * size + 1);//如果vector已满
	++size;
	editIndex(hd, size - 1);
	if (size == 1)//若是第一个元素
		h[size - 1] = node(k, hd);
	else
	{//否则
		h[size - 1] = node(EXTREAM, hd);//若是
		increaseKey(size - 1, k);
	}
}

//----------------------------------------测试---------------------------------------

template <typename Value>
struct object
{//测试对象
	int node_index;//在堆中的的索引
	int priority;//优先权
	Value v;//值
	object(int pri, const Value& vv) :node_index(0), v(vv), priority(pri){}//初始，索引默认为0
};

template <typename Value>
struct editObjIdx :public binary_function < object<Value>*, int , void >
{//自定义的修改对象的堆中索引函数对象
	void operator()(object<Value> *p, int hh)
	{
		p->node_index = hh;
	}
};

int main()
{
	heap<int, object<int>*, editObjIdx<int>> h;
	vector<object<int>*> vec;//存储这些obj
	for (int i = 0; i <= 10; i += 2)
	{//obj的权值为2 * i，值为i * i
		object<int> *p = new object<int>(2 * i, i * i);
		vec.push_back(p);
		h.insert(2 * i, p);
	}
	for (int i = 1; i <= 10; i += 2)
	{
		object<int> *p = new object<int>(2 * i, i * i);
		vec.push_back(p);
		h.insert(2 * i, p);
	}
	/*while (!h.empty())
		cout << h.extractMax() << endl;
	cout << endl;*/
	for (size_t i = 0; i <= h.length(); i += 3)
	{//增加某些对象的权值
		vec[i]->priority += 100;
		h.increaseKey(vec[i]->node_index, vec[i]->priority);
	}
	while (!h.empty())
		cout << h.extractMax() << endl;
	for (int i = 0; i != vec.size(); ++i)
		delete vec[i];

	/*heap<int> h;//只有关键字的堆，此时Handle和EditObjIndex等均为默认值，没有所谓的句柄
	for (int i = 0; i != 10; ++i)
		h.insert(i);
	while (!h.empty())
		cout << h.extractMax() << endl;*/
	getchar();
	return 0;
}