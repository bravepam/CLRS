
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<string>
#include<iostream>
#include<vector>
#include<set>
#include<map>

using namespace std;

class automaton
{//字符串匹配自动机
private:
	string P;//模式串
	string T;//匹配串
	map<char, int> m;//字符和数字编号的映射
	vector<vector<int>> tran;//变迁函数
	set<char> s;//字符表

	void compute_transition();
	int get_number(char c)const
	{//获得字符c所对应的数字编号
		auto beg = m.begin(), end = m.end();
		while (beg != end && beg->first != c)
			++beg;
		return beg->second;
	}
public:
	automaton(const string &t, const string &p, const set<char> &ss) :P(move(p)), T(move(t)), s(move(ss))
	{
		size_t num = 0;
		auto beg = s.begin();
		while (beg != s.end())//生成映射
			m.insert(map<char, int>::value_type(*beg++, num++));
		P.insert(P.begin(), '\0');//为了便于实现，在模式串首部插入一个空字符
	}
	void matcher();
};

void automaton::compute_transition()
{//计算变迁函数
	size_t m = P.size();
	tran.resize(m);
	for (size_t i = 0; i != tran.size(); ++i)
		tran[i].resize(s.size());//设置变迁表大小
	for (size_t state = 0; state < m; ++state)
	{//从状态0开始计算
		auto beg = s.begin(),end = s.end();
		while (beg != end)
		{//依次迭代字符表中的每一个字符
			size_t k = m < state + 2 ? m : state + 2;//p中包含的字符数，包括首部的空字符
			string p = P.substr(0, k),t = P.substr(0,state + 1);//构造子模式串和子匹配串
			t.push_back(*beg);//字匹配串接受当前字符表字符
			while (k > 0)
			{
				size_t i, j;
				//将两个子串逐一比对，p的长度是不会超过t的
				for (i = k, j = t.size() - 1; i > 0 && p[i] == t[j]; --i, --j);
				if (i == 0) break;//当子模式串已经到比较到0，说明1~k之间的字符全部匹配
				else --k;//否则回退一字符，继续比对
			}
			tran[state][get_number(*beg)] = k;//在状态state时，若接受*beg字符，状态应迁往何状态
			++beg;//接受下一个字符
		}
	}
}

void automaton::matcher()
{//匹配
	compute_transition();//计算变迁函数
	size_t n = T.size(),state = 0,m = P.size() - 1;
	for (size_t i = 0; i != n; ++i)
	{//从匹配串0索引处开始比对
		state = tran[state][get_number(T[i])];//在当前状态下，若接受T[i]字符，状态更新
		if (state == m)//若更新后的状态等于m，即为接收状态，则说明已经发现一个匹配
			cout << "Pattern occurs with shift " << i - m + 1 << endl;//输出该匹配相对起始位置的位移
	}
}

int main()
{
	string T = "abcabaabcabac", P = "c";
	set<char> s = { 'a', 'b', 'c' };
	automaton a(T, P, s);
	a.matcher();
	system("pause");
	return 0;
}