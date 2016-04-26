#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include<functional>
#include<cassert>
#include<random>

using namespace std;

//跳跃链表
template<typename Key, typename Value, class Comparator = less<Key>>
class skiplist
{
private:
	struct node
	{//节点类
		const Key key_;
		Value value_;

		explicit node(const Key& k, const Value& v) :key_(k), value_(v){}
		node* next(int n)const
		{//获取第n层的后继
			assert(n >= 0);
			return next_[n];
		}
		void setNext(int n, node* x)
		{
			assert(n >= 0);
			next_[n] = x;
		}
	private:
		node* next_[1];//实现变长数组，长度随机指定，next[i]存储第i层的后继
	};
public:
	struct iterator
	{//迭代器类
		explicit iterator(const skiplist* const sl) :sl_(sl), curr(nullptr){}
		bool valid()const
		{
			return curr != nullptr;
		}
		const Key& key()const
		{
			assert(valid());
			return curr->key_;
		}
		const Value& value()const
		{
			assert(valid());
			return curr->value_;
		}
		void next()
		{//此处next只用来获取最底层后继
			assert(valid());
			curr = curr->next(0);
		}
		void prev()
		{//同理，获取最底层前驱
			assert(valid());
			curr = sl_->findLessThan(curr);

		}
		void seek(const Key& key)
		{//定位到键key处
			curr = sl_->findGreaterOrEqual(key, nullptr);
			if (!curr || !sl_->equal(curr->key_, key))
				curr = nullptr;
		}
		void first()
		{
			curr = sl_->head_->next(0);
		}
		void last()
		{
			curr = sl_->findLast();
		}
	private:
		const skiplist* const sl_;
		node* curr;
	};
private:
	node* const head_; //头结点
	enum{ MaxHeight = 12 }; //跳表最大高度
	int curr_max_height_; //当前最大高度
	//采用线性同余获得随机数，X(i+1) = {X(i) * A + C} mod B,此处A = 16807,C = 0,B = 2147483647(2^31 - 1)
	using lce = linear_congruential_engine < unsigned long, 16807, 0, 2147483647 > ;
	static lce rnd_; 
	static Comparator compare_; //比较器，默认less比较，递增序列

private:
	static int randomHeight()
	{//获得随机高度
		static const unsigned int K = 4;
		int h = 1;
		while (h < MaxHeight && ((rnd_() % K) == 0))
			++h; //只有当随机数能够被K整除时，才递增高度，避免高度波动太大
		return h;
	}
	bool equal(const Key& lhs, const Key& rhs)const
	{
		return !compare_(lhs, rhs) && !compare_(rhs, lhs);
	};
	bool after(const Key& lhs, const Key& rhs)const
	{//即 >=
		return !compare_(lhs,rhs);
	}

	//构造高度为height的节点
	node* newNode(const Key& key, const Value& value, int height)const;

	//返回不小于key的第一个节点，prev存储查找过程中得到的key的各层前驱
	node* findGreaterOrEqual(const Key& key, node** prev)const;
	//返回小于key的最后一个节点
	node* findLessThan(const Key& key)const;
	//跳到最后一个节点
	node* findLast()const;

	//避免复制构造或者赋值
	skiplist& operator=(const skiplist&);
	skiplist(const skiplist&);
public:
	skiplist() :head_(newNode(Key(),Value(), MaxHeight)),
		curr_max_height_(1)
	{
		for (int i = 0; i != MaxHeight; ++i)
			head_->setNext(i, nullptr); //初始头结点任何层都没有后继
	}
	bool empty()const
	{
		return head_->next(0) == nullptr;
	}
	void insert(const Key& key, const Value& value);
	void erase(const Key& key);
	void edit(const Key& key, const Value& new_value);
	bool contain(const Key& key)const;
	void clear();
	~skiplist()
	{
		clear();
		delete head_;
	}
};

//定义compare_
template <typename Key, typename Value, class Comparator = less<Key>>
Comparator skiplist<Key, Value, Comparator>::compare_ = Comparator();

//定义随机数生成器rnd_
template <typename Key, typename Value, class Comparator = less<Key>>
typename skiplist<Key, Value, Comparator>::lce skiplist<Key, Value, Comparator>::rnd_ = lce(0xdeadbeef);

template <typename Key, typename Value, class Comparator = less<Key>>
typename skiplist<Key, Value, Comparator>::node*
skiplist<Key, Value, Comparator>::newNode(const Key& key, const Value& value, int height)const
{
	//node中含有一个next[0]
	char* p = new char[sizeof(node) + sizeof(node*) * (height - 1)];
	return new (p)node(key, value); //定位new表达式构造节点
}

template <typename Key, typename Value, class Comparator = less<Key>>
typename skiplist<Key, Value, Comparator>::node*
skiplist<Key, Value, Comparator>::findGreaterOrEqual(const Key& key, node** prev)const
{
	int level = curr_max_height_ - 1;
	node* curr = head_, *pnext = nullptr;
	while (true)
	{//整个过程就跟在单链表中搜寻一样，只不过有多层单链表
		pnext = curr->next(level); //level层上，curr的后继节点
		if (!pnext || after(pnext->key_,key))
		{//若pnext无效（curr是level层尾节点）或者key小于它
			if (prev) prev[level] = curr; //则得到一个前驱
			if (level == 0) //若已为最底层，则返回
				return pnext;
			else --level;
		}
		else curr = pnext;
	}
}

template <typename Key, typename Value, class Comparator = less<Key>>
typename skiplist<Key, Value, Comparator>::node*
skiplist<Key, Value, Comparator>::findLessThan(const Key& key)const
{
	node* curr = head_, *pnext = nullptr;
	int level = curr_max_height_ - 1;
	while (true)
	{
		pnext = curr->next(level); //level层上，curr的后继节点
		if (pnext && after(pnext->key_,key))
		{//若pnext有效，且比key大
			if (level == 0) return curr; //则curr为该层key的前驱，若是最底层，则返回
			else --level;
		}
		else curr = pnext;
	}
}

template <typename Key, typename Value, class Comparator = less<Key>>
typename skiplist<Key, Value, Comparator>::node*
skiplist<Key, Value, Comparator>::findLast()const
{
	node* curr = head_, *pnext = nullptr;
	int level = curr_max_height_ - 1;
	while (true)
	{
		pnext = curr->next(level); //不断获得level层的后继
		if (!pnext)
		{//若pnext无效
			if (level == 0) return curr; //且已到达最底层，则返回
			else --level;
		}
		else curr = pnext;
	}
}

template <typename Key, typename Value, class Comparator = less<Key>>
void skiplist<Key, Value, Comparator>::insert(const Key& key, const Value& value)
{
	const int h = randomHeight(); //获得随机节点高度
	node* pn = newNode(key, value, h);
	node* prev[MaxHeight];
	findGreaterOrEqual(key, prev); //获得各层前驱，跟单链表类似
	if (h > curr_max_height_)
	{//如果该节点的高度目前最高
		for (int i = curr_max_height_; i != h; ++i)
			prev[i] = head_; //则高出来的部分的前驱必然是头结点
		curr_max_height_ = h; //更新当前最高
	}
	for (int i = 0; i != h; ++i)
	{
		//设置pn的每层的后继和前驱
		pn->setNext(i, prev[i]->next(i));
		prev[i]->setNext(i, pn);
	}
}

template <typename Key, typename Value, class Comparator = less<Key>>
void skiplist<Key, Value, Comparator>::erase(const Key& key)
{
	node* prev[MaxHeight] = { nullptr };
	node* p = findGreaterOrEqual(key, prev); //获得各层前驱
	assert(p && equal(key, p->key_));
	//if (!p || !equal(key, p->key_)) return;
	for (int i = 0; i != MaxHeight && prev[i] && prev[i]->next(i) && equal(prev[i]->next(i)->key_, key); ++i)
	{//删除该节点，则需要更新其前驱的后继。首先，前驱必须存在（比该节点高的层没有）；其次，
		//prev[i]中存储是其紧邻的节点，因为findGreaterOrEqual得到的prev可能不是紧邻的
		prev[i]->setNext(i, p->next(i));
	}
	delete p;
}

template <typename Key, typename Value, class Comparator = less<Key>>
void skiplist<Key, Value, Comparator>::edit(const Key& key, const Value& new_value)
{
	node* p = findGreaterOrEqual(key, nullptr);
	assert(p && equal(key, p->key_));
	//if (p && equal(key, p->key_))
		p->value_ = new_value;
}

template <typename Key, typename Value, class Comparator = less<Key>>
bool skiplist<Key, Value, Comparator>::contain(const Key& key)const
{
	node* p = findGreaterOrEqual(key, nullptr);
	return p && equal(key, p->key_);
}

template <typename Key, typename Value, class Comparator = less<Key>>
void skiplist<Key, Value, Comparator>::clear()
{
	node* curr = head_->next(0), *p = nullptr;
	while (curr)
	{
		p = curr;
		curr = curr->next(0);
		delete p;
	}
	for (int i = 0; i != curr_max_height_; ++i)
		head_->setNext(i, nullptr);
	curr_max_height_ = 1;
}

#endif