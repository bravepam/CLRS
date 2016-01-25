
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
{//�ַ���ƥ���Զ���
private:
	string P;//ģʽ��
	string T;//ƥ�䴮
	map<char, int> m;//�ַ������ֱ�ŵ�ӳ��
	vector<vector<int>> tran;//��Ǩ����
	set<char> s;//�ַ���

	void compute_transition();
	int get_number(char c)const
	{//����ַ�c����Ӧ�����ֱ��
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
		while (beg != s.end())//����ӳ��
			m.insert(map<char, int>::value_type(*beg++, num++));
		P.insert(P.begin(), '\0');//Ϊ�˱���ʵ�֣���ģʽ���ײ�����һ�����ַ�
	}
	void matcher();
};

void automaton::compute_transition()
{//�����Ǩ����
	size_t m = P.size();
	tran.resize(m);
	for (size_t i = 0; i != tran.size(); ++i)
		tran[i].resize(s.size());//���ñ�Ǩ���С
	for (size_t state = 0; state < m; ++state)
	{//��״̬0��ʼ����
		auto beg = s.begin(),end = s.end();
		while (beg != end)
		{//���ε����ַ����е�ÿһ���ַ�
			size_t k = m < state + 2 ? m : state + 2;//p�а������ַ����������ײ��Ŀ��ַ�
			string p = P.substr(0, k),t = P.substr(0,state + 1);//������ģʽ������ƥ�䴮
			t.push_back(*beg);//��ƥ�䴮���ܵ�ǰ�ַ����ַ�
			while (k > 0)
			{
				size_t i, j;
				//�������Ӵ���һ�ȶԣ�p�ĳ����ǲ��ᳬ��t��
				for (i = k, j = t.size() - 1; i > 0 && p[i] == t[j]; --i, --j);
				if (i == 0) break;//����ģʽ���Ѿ����Ƚϵ�0��˵��1~k֮����ַ�ȫ��ƥ��
				else --k;//�������һ�ַ��������ȶ�
			}
			tran[state][get_number(*beg)] = k;//��״̬stateʱ��������*beg�ַ���״̬ӦǨ����״̬
			++beg;//������һ���ַ�
		}
	}
}

void automaton::matcher()
{//ƥ��
	compute_transition();//�����Ǩ����
	size_t n = T.size(),state = 0,m = P.size() - 1;
	for (size_t i = 0; i != n; ++i)
	{//��ƥ�䴮0��������ʼ�ȶ�
		state = tran[state][get_number(T[i])];//�ڵ�ǰ״̬�£�������T[i]�ַ���״̬����
		if (state == m)//�����º��״̬����m����Ϊ����״̬����˵���Ѿ�����һ��ƥ��
			cout << "Pattern occurs with shift " << i - m + 1 << endl;//�����ƥ�������ʼλ�õ�λ��
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