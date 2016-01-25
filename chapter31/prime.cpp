#include<iostream>
#include<vector>
#include<cmath>
#include<assert.h>

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

bool prime_trial_divide(int n)
{
	assert(n > 1);
	for (int i = 2; i <= (int)sqrt(n); ++i)
	{
		if (i > 2 && i % 2 == 0)continue;
		if (n % i == 0) return false;
	}
	return true;
}

bool posudo_prime(int n)
{
	assert(n > 1);
	if (n == 2) return true;
	/*******************************************
	* Author: bravepam
	* E-mail:1372120340@qq.com
	* Blog:http://blog.csdn.net/zilingxiyue
	*******************************************
	*/


	if (modularExponentiation(2, n - 1, n) != 1)
		return false;
	return true;
}
//----------------Miller-Rabin�������Է�---------------------------------
int random(int seed,int upper)
{//�������������ڻ�a����Χ[1,n - 1],upper = n -1
	srand(unsigned int(seed));
	return rand() % upper + 1;
}

void power_presentation(int n, int &t, int &u)
{//��nת��Ϊu * 2^t��ʾ������u������
	t = 0; u = n;
	while (u % 2 == 0)
	{
		++t;
		u /= 2;
	}
}

bool witness(int a,int n)
{//�����ڻ�a�£�n�Ƿ�Ϊ������//���ݷ����������31.35
	int t = 0, u = 0;
	power_presentation(n - 1, t, u);
	int y = 0,x = modularExponentiation(a, u, n);//�ȼ����a^u mod n
	for (int i = 1; i <= t; ++i)
	{//������ƽ��t��
		y = x * x % n;
		if (y == 1 && x != 1 && x != n - 1)//���˴ν��Ϊ1�����ϴν����1�ķ�ƽ��ƽ����
			return true;//���������31.35�ɵ�nΪ����
		x = y;
	}
	if (x != 1) return true;//�����ĵ���ģ����1������ݷ������֪�϶���������
	return false;
}

bool miller_rabin(int s, int n)
{//��������
	assert(n > 1);
	if (n == 2) return true;
	for (int i = 0; i != s; ++i)
	{//���ѡȡ��������s��
		int a = random(i,n - 1);
		if (witness(a, n))
			return false;
	}
	return true;
}

int main()
{
	int n;
	while (cin >> n)
		cout << miller_rabin(50,n) << endl;
	return 0;
}