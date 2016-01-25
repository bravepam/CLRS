
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<string>
#include<vector>

using namespace std;

class rabin_karp_matcher
{//rabin-karp�ַ���ƥ���㷨
public:
	string T;//ƥ�䴮
	string P;//ģʽ��
	int d;//��������Ҳ���ַ����ַ�����
	int q;//ȡģ����
	char pivot;//��׼�ַ�

	void binary(int b, vector<char> &bit)
	{//ʮ����ת��Ϊ������
		while (b != 0)
		{
			bit.push_back(b % 2);
			b /= 2;
		}
	}
	int modularExponentiation(int a, int b, int n)
	{//ģȡ�ݣ�a^b mod n = �����������������b
		vector<char> bit;
		binary(b, bit);//��b��ʾ�ɶ�������ʽ����bit���Ƿ���洢��
		int d = 1;
		for (int i = bit.size() - 1; i >= 0; --i)
		{//��������һ��λ��ʼ����
			d = d * d % n;
			if (bit[i] == 1)
				d = d * a % n;
		}
		return d;
	}
	int char_to_num(char c)const { return static_cast<int>(c - pivot); }//�������ַ�ת��Ϊd����������֮һ
public:
	rabin_karp_matcher(const string &t, const string &p, int dd, int qq, char piv) :
		T(move(t)), P(move(p)), d(dd), q(qq), pivot(piv){}
	void rabin_karp();
};

void rabin_karp_matcher::rabin_karp()
{
	int n = T.size(), m = P.size();
	int h = modularExponentiation(d, m - 1, q);//����d^m-1 mod q
	int p = 0, t = 0;
	for (int i = 0; i != m; ++i)
	{//Ԥ��������ģʽ������ƥ�䴮��ֵ
		p = (p * d + char_to_num(P[i])) % q;
		t = (t * d + char_to_num(T[i])) % q;
	}
	for (int s = 0; s <= n - m; ++s)
	{//��λ��ƥ��
		if (p == t)
		{//��ֵ��ȣ���˵��ƥ���Ѿ�����
			string Tstr = T.substr(s, m);
			if (P == Tstr)
				cout << "Pattern occurs with shift " << s << endl;
		}
		if (s < n - m)//������һ����ƥ�䴮��ֵ
		{
			t = (d * (t - char_to_num(T[s]) * h) + char_to_num(T[s + m])) % q;
			if (t < 0) t += q;
		}
	}
}

int main()
{
	//string T = "abcabaabcabac", P = "abaa";
	//rabin_karp_matcher rkm(T, P, 26, 6151,'a');
	string T = "000010001010001", P = "0001";
	rabin_karp_matcher rkm(T, P, 2, 6151, '0');
	rkm.rabin_karp();
	system("pause");
	return 0;
}