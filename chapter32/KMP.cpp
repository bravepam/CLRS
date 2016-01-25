
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

void compute_next(const string &P,vector<int> &next)
{//计算next数组，next记录的是一旦当前P索引k + 1处匹配不成功，应当回退到next[k]索引处继续下一个的匹配
	size_t length = P.size();
	next.resize(length);
	next[0] = -1;//初始，索引0处匹配不成功
	int k = -1;//初始k值,k记录的是k以及k之前的所有字符均已匹配
	for (size_t q = 1; q != length; ++q)
	{//从第二字符，也就是索引1处开始循环
		while (k >= 0 && P[k + 1] != P[q])//索引k之前已经匹配，接着检查k+1所引处
			k = next[k];//若k+1处不匹配，则回退到next[k]处接着检查下一个，直到匹配或者已经没有匹配字符了
		if (P[k + 1] == P[q])//若匹配，同时它导致上述while循环终止
			++k;
		next[q] = k;//设置索引q处可回退的索引号
	}
}

void KMP(const string &T, const string &P)
{//KMP算法，类似于字符串匹配自动机算法
	size_t t_length = T.size(), p_length = P.size();
	vector<int> next;
	compute_next(P, next);//计算模式P的next数组
	int matched_char_index = -1;//当前已经匹配字符的最大索引，即P的最长前缀
	for (size_t i = 0; i != t_length; ++i)
	{//从0开始依次检查每个字符
		//以下while和if语句非常类似于compute_next函数
		//也类似于字符串匹配自动机算法中的第四行
		while (matched_char_index >= 0 && P[matched_char_index + 1] != T[i])
			matched_char_index = next[matched_char_index];
		if (P[matched_char_index + 1] == T[i])
			++matched_char_index;
		if (matched_char_index + 1 == p_length)
		{//若当前已经匹配的字符数等于模式P的长度
			cout << "Pattern occurs with shift " << i - matched_char_index << endl;//则说明已经出现了一个匹配
			matched_char_index = next[matched_char_index];//回退，以查找下一个匹配
		}
	}
}

int main()
{
	string T = "abcabaabcabac", P = "abaa";
	KMP(T, P);
	system("pause");
	return 0;
}