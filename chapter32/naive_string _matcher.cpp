
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//�㷨���۵�32�� �ַ���ƥ���㷨

#include<iostream>
#include<string>

using namespace std;

//void naive_string_macther(const string &T, const string &P)
//{//���ص��ַ���ƥ���㷨
//	size_t n = T.size(), m = P.size();
//	for (size_t s = 0; s <= n - m; ++s)
//	{//sΪƥ�俪ʼƥ���λ�ƣ����ֻ�ܵ�n-m
//		size_t i;
//		for (i = 0; i != m; ++i)
//		{
//			if (P[i] != T[s + i])//����ǰ�ַ���ƥ�䣬������
//				break;
//		}
//		if (i == m)//��iһֱ������m��˵����s��һ��ƥ���
//			cout << "Pattern occurs with shift " << s << endl;
//	}
//}

size_t naive_string_macther(const string &T, const string &P)
{//�޸�һ�����ص��ַ���ƥ���㷨������ʵ��32.1-4
	size_t n = T.size(), m = P.size(), s;
	for (s = 0; s <= n - m; ++s)
	{//sΪƥ�俪ʼƥ���λ�ƣ����ֻ�ܵ�n-m
		size_t i;
		for (i = 0; i != m; ++i)
		{
			if (P[i] != T[s + i])//����ǰ�ַ���ƥ�䣬������
				break;
		}
		if (i == m)//��iһֱ������m��˵����s��һ��ƥ���
			return s;
	}
	return s;
}

//bool exercise32_1_4(string &T, string &P)
//{//�ݹ�ʵ��32.1-4
//	size_t i = 0;
//	while (i < P.size() && P[i] != '*') ++i;//�ҵ��ַ�*
//	string Pstr = P.substr(0, i);//��*֮ǰ���ַ�����ģʽ��
//	size_t s = naive_string_macther(T, Pstr);
//	if (s == T.size() - Pstr.size() + 1) return false;//��û��ƥ�䣬��ʧ��
//	if (i == P.size()) return true;//��ԭģʽ���Ѿ�ƥ����ϣ���ɹ�
//	return exercise32_1_4(T.substr(s + Pstr.size()), P.substr(i + 1));//�����޸�T��P,���еݹ�
//}

bool exercise32_1_4(string &T, string &P)
{//�ǵݹ�ʵ��31.1-4
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