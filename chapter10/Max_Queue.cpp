
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

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
	vector<T> s;//栈，用数组实现
	int m_index;//存储最大值所在的索引
	vector<int> max_index;//max_index[i]处存储的是s[0...i - 1]的最大值的索引，即上一次的最大值索引
	int t;//栈顶
public:
	max_stack() :s(), max_index(), m_index(MIN_INDEX), t(MIN_INDEX){}
	bool empty()const { return t == MIN_INDEX; }
	T pop()
	{
		if (t == m_index)//如果出栈元素正好是目前的最大值
			m_index = max_index[m_index];//则更新最大值为上一次，即在此之前所有元素的最大值
		return s[t--];
	}
	T top()const { return s[t]; }
	T max()const
	{
		if (m_index > MIN_INDEX)//若存在元素
			return s[m_index];
		else return MIN_DATA;//否则
	}
	void push(const T & dt)
	{
		if (t == static_cast<int>(s.size()) - 1)
		{//如果栈满
			s.resize(2 * (t + 1) + 1);
			max_index.resize(2 * (t + 1) + 1);
		}
		s[++t] = dt;
		if (dt > max())
		{//如果新元素比最大值大
			max_index[t] = m_index;//则记下前一次最大值索引，这里类似于前驱子图
			m_index = t;//更新现在的最大值索引
		}
		else max_index[t] = MIN_INDEX;//否则
	}
	/*void print()const
	{
		for (int i = 0; i <= t; ++i)
			cout << s[i] << ' ';
		cout << endl;
		for (int i = 0; i <= t; ++i)
			cout << max_index[i] << ' ';
		cout << endl;
	}*/
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
	/*void print()const
	{
		A.print();
		B.print();
	}*/
};

int main()
{
	max_queue<int> ms;
	vector<int> vec = { 3, 5, 7, 78,1, 3, 5, 45 };
	for (size_t i = 0; i != vec.size(); ++i)
		ms.push(vec[i]);
	cout << boolalpha << ms.empty() << endl;
	while (!ms.empty())
	{
		cout << "max: " << ms.max() << endl;
		cout << "pop: " << ms.pop() << endl;
	}
	cout << boolalpha << ms.empty() << endl;
	system("pause");
	return 0;
}