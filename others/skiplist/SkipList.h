#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include<functional>
#include<cassert>
#include<random>

using namespace std;

//��Ծ����
template<typename Key, typename Value, class Comparator = less<Key>>
class skiplist
{
private:
	struct node
	{//�ڵ���
		const Key key_;
		Value value_;

		explicit node(const Key& k, const Value& v) :key_(k), value_(v){}
		node* next(int n)const
		{//��ȡ��n��ĺ��
			assert(n >= 0);
			return next_[n];
		}
		void setNext(int n, node* x)
		{
			assert(n >= 0);
			next_[n] = x;
		}
	private:
		node* next_[1];//ʵ�ֱ䳤���飬�������ָ����next[i]�洢��i��ĺ��
	};
public:
	struct iterator
	{//��������
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
		{//�˴�nextֻ������ȡ��ײ���
			assert(valid());
			curr = curr->next(0);
		}
		void prev()
		{//ͬ����ȡ��ײ�ǰ��
			assert(valid());
			curr = sl_->findLessThan(curr);

		}
		void seek(const Key& key)
		{//��λ����key��
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
	node* const head_; //ͷ���
	enum{ MaxHeight = 12 }; //�������߶�
	int curr_max_height_; //��ǰ���߶�
	//��������ͬ�����������X(i+1) = {X(i) * A + C} mod B,�˴�A = 16807,C = 0,B = 2147483647(2^31 - 1)
	using lce = linear_congruential_engine < unsigned long, 16807, 0, 2147483647 > ;
	static lce rnd_; 
	static Comparator compare_; //�Ƚ�����Ĭ��less�Ƚϣ���������

private:
	static int randomHeight()
	{//�������߶�
		static const unsigned int K = 4;
		int h = 1;
		while (h < MaxHeight && ((rnd_() % K) == 0))
			++h; //ֻ�е�������ܹ���K����ʱ���ŵ����߶ȣ�����߶Ȳ���̫��
		return h;
	}
	bool equal(const Key& lhs, const Key& rhs)const
	{
		return !compare_(lhs, rhs) && !compare_(rhs, lhs);
	};
	bool after(const Key& lhs, const Key& rhs)const
	{//�� >=
		return !compare_(lhs,rhs);
	}

	//����߶�Ϊheight�Ľڵ�
	node* newNode(const Key& key, const Value& value, int height)const;

	//���ز�С��key�ĵ�һ���ڵ㣬prev�洢���ҹ����еõ���key�ĸ���ǰ��
	node* findGreaterOrEqual(const Key& key, node** prev)const;
	//����С��key�����һ���ڵ�
	node* findLessThan(const Key& key)const;
	//�������һ���ڵ�
	node* findLast()const;

	//���⸴�ƹ�����߸�ֵ
	skiplist& operator=(const skiplist&);
	skiplist(const skiplist&);
public:
	skiplist() :head_(newNode(Key(),Value(), MaxHeight)),
		curr_max_height_(1)
	{
		for (int i = 0; i != MaxHeight; ++i)
			head_->setNext(i, nullptr); //��ʼͷ����κβ㶼û�к��
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

//����compare_
template <typename Key, typename Value, class Comparator = less<Key>>
Comparator skiplist<Key, Value, Comparator>::compare_ = Comparator();

//���������������rnd_
template <typename Key, typename Value, class Comparator = less<Key>>
typename skiplist<Key, Value, Comparator>::lce skiplist<Key, Value, Comparator>::rnd_ = lce(0xdeadbeef);

template <typename Key, typename Value, class Comparator = less<Key>>
typename skiplist<Key, Value, Comparator>::node*
skiplist<Key, Value, Comparator>::newNode(const Key& key, const Value& value, int height)const
{
	//node�к���һ��next[0]
	char* p = new char[sizeof(node) + sizeof(node*) * (height - 1)];
	return new (p)node(key, value); //��λnew���ʽ����ڵ�
}

template <typename Key, typename Value, class Comparator = less<Key>>
typename skiplist<Key, Value, Comparator>::node*
skiplist<Key, Value, Comparator>::findGreaterOrEqual(const Key& key, node** prev)const
{
	int level = curr_max_height_ - 1;
	node* curr = head_, *pnext = nullptr;
	while (true)
	{//�������̾͸��ڵ���������Ѱһ����ֻ�����ж�㵥����
		pnext = curr->next(level); //level���ϣ�curr�ĺ�̽ڵ�
		if (!pnext || after(pnext->key_,key))
		{//��pnext��Ч��curr��level��β�ڵ㣩����keyС����
			if (prev) prev[level] = curr; //��õ�һ��ǰ��
			if (level == 0) //����Ϊ��ײ㣬�򷵻�
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
		pnext = curr->next(level); //level���ϣ�curr�ĺ�̽ڵ�
		if (pnext && after(pnext->key_,key))
		{//��pnext��Ч���ұ�key��
			if (level == 0) return curr; //��currΪ�ò�key��ǰ����������ײ㣬�򷵻�
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
		pnext = curr->next(level); //���ϻ��level��ĺ��
		if (!pnext)
		{//��pnext��Ч
			if (level == 0) return curr; //���ѵ�����ײ㣬�򷵻�
			else --level;
		}
		else curr = pnext;
	}
}

template <typename Key, typename Value, class Comparator = less<Key>>
void skiplist<Key, Value, Comparator>::insert(const Key& key, const Value& value)
{
	const int h = randomHeight(); //�������ڵ�߶�
	node* pn = newNode(key, value, h);
	node* prev[MaxHeight];
	findGreaterOrEqual(key, prev); //��ø���ǰ����������������
	if (h > curr_max_height_)
	{//����ýڵ�ĸ߶�Ŀǰ���
		for (int i = curr_max_height_; i != h; ++i)
			prev[i] = head_; //��߳����Ĳ��ֵ�ǰ����Ȼ��ͷ���
		curr_max_height_ = h; //���µ�ǰ���
	}
	for (int i = 0; i != h; ++i)
	{
		//����pn��ÿ��ĺ�̺�ǰ��
		pn->setNext(i, prev[i]->next(i));
		prev[i]->setNext(i, pn);
	}
}

template <typename Key, typename Value, class Comparator = less<Key>>
void skiplist<Key, Value, Comparator>::erase(const Key& key)
{
	node* prev[MaxHeight] = { nullptr };
	node* p = findGreaterOrEqual(key, prev); //��ø���ǰ��
	assert(p && equal(key, p->key_));
	//if (!p || !equal(key, p->key_)) return;
	for (int i = 0; i != MaxHeight && prev[i] && prev[i]->next(i) && equal(prev[i]->next(i)->key_, key); ++i)
	{//ɾ���ýڵ㣬����Ҫ������ǰ���ĺ�̡����ȣ�ǰ��������ڣ��ȸýڵ�ߵĲ�û�У�����Σ�
		//prev[i]�д洢������ڵĽڵ㣬��ΪfindGreaterOrEqual�õ���prev���ܲ��ǽ��ڵ�
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