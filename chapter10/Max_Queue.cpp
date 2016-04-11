/*
 *实现一个这样一个队列：
 *1、出队操作pop(),删除队首元素，并且返回；
 *2、进队push()；
 *3、取队列中最大值元素返回max();
 *要求：速度尽可能的快。
 */

/*
 *采用两个栈实现，这个栈也有一些特殊：
 *1、包括常见的pop和push操作；
 *2、取栈中最大值元素操作max().
 */

#include<iostream>
#include<vector>

using namespace std;

#define MIN_DATA 0x80000000
#define MIN_INDEX -1

template <typename T>
class max_stack
{//最大值栈
private:
	vector<T> s;//值栈
	int t;//栈顶
	vector<int> max_index;//索引栈，存储历次最大值索引
	int m_index_t;//索引栈栈顶
public:
	max_stack() :s(), max_index(), m_index_t(MIN_INDEX), t(MIN_INDEX){}
	bool empty()const { return t == MIN_INDEX; }
	T pop()
	{
		if (s[t] == s[max_index[m_index_t]])//如果出栈元素正好是目前的最大值
			--m_index_t;
		return s[t--];
	}
	T top()const { return s[t]; }
	T max()const
	{
		if (m_index_t > MIN_INDEX)//若存在元素
			return s[max_index[m_index_t]];
		else return MIN_DATA;//否则
	}
	void push(const T &dt)
	{
		if (t == static_cast<int>(s.size()) - 1)
		{//如果值栈满
			s.resize(2 * (t + 1) + 1);
		}
		s[++t] = dt;
		if (dt > max())
		{//如果新元素比最大值大
			if (m_index_t == static_cast<int>(max_index.size()) - 1)
				max_index.resize(2 * (m_index_t + 1) + 1);
			max_index[++m_index_t] = t;
		}
	}
	void print()const
	{
		for (int i = 0; i <= t; ++i)
			cout << s[i] << ' ';
		cout << endl;
		for (int i = 0; i <= m_index_t; ++i)
			cout << s[max_index[i]] << ' ';
		cout << endl;
	}
};

template <typename T>
class max_queue
{//最大值队列
	//入队以及取最大值操作均为O(1)时间
	//采用平摊分析可知出队操作也是O(1)时间
private://采用两个最大值栈实现
	max_stack<T> A;//入队数据存储栈
	max_stack<T> B;//出队时若B空则将A中数据全部压入B

	T maxOfTwo(const T &x, const T& y)const { return x > y ? x : y; }
public:
	max_queue() = default;
	bool empty()const { return A.empty() && B.empty(); }
	T max()const { return maxOfTwo(A.max(), B.max()); }
	T pop()
	{//出队
		if (B.empty())
		{//若B空，则队首元素应该在A的栈底
			while (!A.empty())//将A中元素全部压入B
				B.push(A.pop());
		}
		return B.pop();//出B栈
	}
	void push(const T & val) { A.push(val); }
	void print()const
	{
		A.print();
		B.print();
	}
};

int main()
{
	/*max_queue<int> mq;
	vector<int> vec = { 3, 5, 7, 78,1, 3, 5, 45 };
	for (size_t i = 0; i != vec.size(); ++i)
		mq.push(vec[i]);
	cout << boolalpha << mq.empty() << endl;
	while (!mq.empty())
	{
		cout << "max: " << mq.max() << endl;
		cout << "pop: " << mq.pop() << endl;
	}
	cout << boolalpha << mq.empty() << endl;
	system("pause");*/

	max_queue<int> mq;
	vector<int> vec1 = { 3, 7, 2, 6, 10, 78 };
	for (int i = 0; i != vec1.size(); ++i)
		mq.push(vec1[i]);
	cout << mq.pop() << endl;
	mq.print();

	vector<int> vec2 = { 45, 3, 5, 1, 79, 34 };
	for (int i = 0; i != vec2.size(); ++i)
		mq.push(vec2[i]);
	mq.print();

	while (!mq.empty())
	{
		cout << "---------------" << endl;
		cout << mq.pop() << endl;
		mq.print();
	}
	system("pause");
	return 0;
}