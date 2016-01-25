
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
{//rabin-karp字符串匹配算法
public:
	string T;//匹配串
	string P;//模式串
	int d;//进制数，也即字符表字符总数
	int q;//取模素数
	char pivot;//基准字符

	void binary(int b, vector<char> &bit)
	{//十进制转换为二进制
		while (b != 0)
		{
			bit.push_back(b % 2);
			b /= 2;
		}
	}
	int modularExponentiation(int a, int b, int n)
	{//模取幂，a^b mod n = ？，正向遍历二进制b
		vector<char> bit;
		binary(b, bit);//将b表示成二进制形式，在bit中是反向存储的
		int d = 1;
		for (int i = bit.size() - 1; i >= 0; --i)
		{//因而从最后一个位开始遍历
			d = d * d % n;
			if (bit[i] == 1)
				d = d * a % n;
		}
		return d;
	}
	int char_to_num(char c)const { return static_cast<int>(c - pivot); }//将各个字符转换为d进制数其中之一
public:
	rabin_karp_matcher(const string &t, const string &p, int dd, int qq, char piv) :
		T(move(t)), P(move(p)), d(dd), q(qq), pivot(piv){}
	void rabin_karp();
};

void rabin_karp_matcher::rabin_karp()
{
	int n = T.size(), m = P.size();
	int h = modularExponentiation(d, m - 1, q);//计算d^m-1 mod q
	int p = 0, t = 0;
	for (int i = 0; i != m; ++i)
	{//预处理，计算模式串和子匹配串数值
		p = (p * d + char_to_num(P[i])) % q;
		t = (t * d + char_to_num(T[i])) % q;
	}
	for (int s = 0; s <= n - m; ++s)
	{//逐位移匹配
		if (p == t)
		{//若值相等，则说明匹配已经出现
			string Tstr = T.substr(s, m);
			if (P == Tstr)
				cout << "Pattern occurs with shift " << s << endl;
		}
		if (s < n - m)//计算下一个子匹配串的值
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