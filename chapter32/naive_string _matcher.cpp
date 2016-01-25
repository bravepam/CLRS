
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//算法导论第32章 字符串匹配算法

#include<iostream>
#include<string>

using namespace std;

//void naive_string_macther(const string &T, const string &P)
//{//朴素的字符串匹配算法
//	size_t n = T.size(), m = P.size();
//	for (size_t s = 0; s <= n - m; ++s)
//	{//s为匹配开始匹配的位移，最多只能到n-m
//		size_t i;
//		for (i = 0; i != m; ++i)
//		{
//			if (P[i] != T[s + i])//若当前字符不匹配，则跳出
//				break;
//		}
//		if (i == m)//若i一直自增到m，说明该s是一个匹配点
//			cout << "Pattern occurs with shift " << s << endl;
//	}
//}

size_t naive_string_macther(const string &T, const string &P)
{//修改一下朴素的字符串匹配算法，用于实现32.1-4
	size_t n = T.size(), m = P.size(), s;
	for (s = 0; s <= n - m; ++s)
	{//s为匹配开始匹配的位移，最多只能到n-m
		size_t i;
		for (i = 0; i != m; ++i)
		{
			if (P[i] != T[s + i])//若当前字符不匹配，则跳出
				break;
		}
		if (i == m)//若i一直自增到m，说明该s是一个匹配点
			return s;
	}
	return s;
}

//bool exercise32_1_4(string &T, string &P)
//{//递归实现32.1-4
//	size_t i = 0;
//	while (i < P.size() && P[i] != '*') ++i;//找到字符*
//	string Pstr = P.substr(0, i);//以*之前的字符构造模式串
//	size_t s = naive_string_macther(T, Pstr);
//	if (s == T.size() - Pstr.size() + 1) return false;//若没有匹配，则失败
//	if (i == P.size()) return true;//若原模式串已经匹配完毕，则成功
//	return exercise32_1_4(T.substr(s + Pstr.size()), P.substr(i + 1));//重新修改T和P,进行递归
//}

bool exercise32_1_4(string &T, string &P)
{//非递归实现31.1-4
	do
	{
		size_t i = 0;
		while (i < P.size() && P[i] != '*') ++i;
		string Pstr = P.substr(0, i);
		size_t s = naive_string_macther(T, Pstr);
		if (s == T.size() - Pstr.size() + 1) return false;
		T = T.substr(s + Pstr.size());
		P = P.substr(i + 1);
	} while (!P.empty());
	return true;
}

int main()
{
	string T = "cabccbacbacab", P = "**a**";
	//while (cin >> T >> P)
	cout << exercise32_1_4(T, P) << endl;
	system("pause");
	return 0;
}