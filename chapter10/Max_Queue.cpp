/*
 *ʵ��һ������һ�����У�
 *1�����Ӳ���pop(),ɾ������Ԫ�أ����ҷ��أ�
 *2������push()��
 *3��ȡ���������ֵԪ�ط���max();
 *Ҫ���ٶȾ����ܵĿ졣
 */

/*
 *��������ջʵ�֣����ջҲ��һЩ���⣺
 *1������������pop��push������
 *2��ȡջ�����ֵԪ�ز���max().
 */

#include<iostream>
#include<vector>

using namespace std;

#define MIN_DATA 0x80000000
#define MIN_INDEX -1

template <typename T>
class max_stack
{//���ֵջ
private:
	vector<T> s;//ֵջ
	int t;//ջ��
	vector<int> max_index;//����ջ���洢�������ֵ����
	int m_index_t;//����ջջ��
public:
	max_stack() :s(), max_index(), m_index_t(MIN_INDEX), t(MIN_INDEX){}
	bool empty()const { return t == MIN_INDEX; }
	T pop()
	{
		if (s[t] == s[max_index[m_index_t]])//�����ջԪ��������Ŀǰ�����ֵ
			--m_index_t;
		return s[t--];
	}
	T top()const { return s[t]; }
	T max()const
	{
		if (m_index_t > MIN_INDEX)//������Ԫ��
			return s[max_index[m_index_t]];
		else return MIN_DATA;//����
	}
	void push(const T &dt)
	{
		if (t == static_cast<int>(s.size()) - 1)
		{//���ֵջ��
			s.resize(2 * (t + 1) + 1);
		}
		s[++t] = dt;
		if (dt > max())
		{//�����Ԫ�ر����ֵ��
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
{//���ֵ����
	//����Լ�ȡ���ֵ������ΪO(1)ʱ��
	//����ƽ̯������֪���Ӳ���Ҳ��O(1)ʱ��
private://�����������ֵջʵ��
	max_stack<T> A;//������ݴ洢ջ
	max_stack<T> B;//����ʱ��B����A������ȫ��ѹ��B

	T maxOfTwo(const T &x, const T& y)const { return x > y ? x : y; }
public:
	max_queue() = default;
	bool empty()const { return A.empty() && B.empty(); }
	T max()const { return maxOfTwo(A.max(), B.max()); }
	T pop()
	{//����
		if (B.empty())
		{//��B�գ������Ԫ��Ӧ����A��ջ��
			while (!A.empty())//��A��Ԫ��ȫ��ѹ��B
				B.push(A.pop());
		}
		return B.pop();//��Bջ
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