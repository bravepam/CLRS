
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

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
	vector<T> s;//ջ��������ʵ��
	int m_index;//�洢���ֵ���ڵ�����
	vector<int> max_index;//max_index[i]���洢����s[0...i - 1]�����ֵ������������һ�ε����ֵ����
	int t;//ջ��
public:
	max_stack() :s(), max_index(), m_index(MIN_INDEX), t(MIN_INDEX){}
	bool empty()const { return t == MIN_INDEX; }
	T pop()
	{
		if (t == m_index)//�����ջԪ��������Ŀǰ�����ֵ
			m_index = max_index[m_index];//��������ֵΪ��һ�Σ����ڴ�֮ǰ����Ԫ�ص����ֵ
		return s[t--];
	}
	T top()const { return s[t]; }
	T max()const
	{
		if (m_index > MIN_INDEX)//������Ԫ��
			return s[m_index];
		else return MIN_DATA;//����
	}
	void push(const T & dt)
	{
		if (t == static_cast<int>(s.size()) - 1)
		{//���ջ��
			s.resize(2 * (t + 1) + 1);
			max_index.resize(2 * (t + 1) + 1);
		}
		s[++t] = dt;
		if (dt > max())
		{//�����Ԫ�ر����ֵ��
			max_index[t] = m_index;//�����ǰһ�����ֵ����������������ǰ����ͼ
			m_index = t;//�������ڵ����ֵ����
		}
		else max_index[t] = MIN_INDEX;//����
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