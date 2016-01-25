
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
{//����next���飬next��¼����һ����ǰP����k + 1��ƥ�䲻�ɹ���Ӧ�����˵�next[k]������������һ����ƥ��
	size_t length = P.size();
	next.resize(length);
	next[0] = -1;//��ʼ������0��ƥ�䲻�ɹ�
	int k = -1;//��ʼkֵ,k��¼����k�Լ�k֮ǰ�������ַ�����ƥ��
	for (size_t q = 1; q != length; ++q)
	{//�ӵڶ��ַ���Ҳ��������1����ʼѭ��
		while (k >= 0 && P[k + 1] != P[q])//����k֮ǰ�Ѿ�ƥ�䣬���ż��k+1������
			k = next[k];//��k+1����ƥ�䣬����˵�next[k]�����ż����һ����ֱ��ƥ������Ѿ�û��ƥ���ַ���
		if (P[k + 1] == P[q])//��ƥ�䣬ͬʱ����������whileѭ����ֹ
			++k;
		next[q] = k;//��������q���ɻ��˵�������
	}
}

void KMP(const string &T, const string &P)
{//KMP�㷨���������ַ���ƥ���Զ����㷨
	size_t t_length = T.size(), p_length = P.size();
	vector<int> next;
	compute_next(P, next);//����ģʽP��next����
	int matched_char_index = -1;//��ǰ�Ѿ�ƥ���ַ��������������P���ǰ׺
	for (size_t i = 0; i != t_length; ++i)
	{//��0��ʼ���μ��ÿ���ַ�
		//����while��if���ǳ�������compute_next����
		//Ҳ�������ַ���ƥ���Զ����㷨�еĵ�����
		while (matched_char_index >= 0 && P[matched_char_index + 1] != T[i])
			matched_char_index = next[matched_char_index];
		if (P[matched_char_index + 1] == T[i])
			++matched_char_index;
		if (matched_char_index + 1 == p_length)
		{//����ǰ�Ѿ�ƥ����ַ�������ģʽP�ĳ���
			cout << "Pattern occurs with shift " << i - matched_char_index << endl;//��˵���Ѿ�������һ��ƥ��
			matched_char_index = next[matched_char_index];//���ˣ��Բ�����һ��ƥ��
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