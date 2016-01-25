
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<vector>

using namespace std;

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

int modularExponentiation2(int a, int b, int n)
{//ģȡ��2���������������b
	vector<char> bit;
	binary(b, bit);
	int t = a, d = 1;
	for (int i = 0; i != bit.size(); ++i)
	{//����ӵ�һ��λ��ʼ����
		if (bit[i] == 1)
			d = (d * t) % n;
		t = t * t % n;
	}
	return d;
}

int main()
{
	int a, b, n;
	while (cin >> a >> b >> n)
		cout << modularExponentiation(a, b, n) << endl;
	return 0;
}