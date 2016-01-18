
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>

using namespace std;

long long fibonacci_recursive(int n)
{//���صĵݹ���쳲���������ʱ��n����
	if (n == 0) return 0;
	if (n == 1) return 1;
	return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

long long fibonacci_memory(int n)
{//���䷨��쳲���������ʱ�临�Ӷ�O(n)
	if (n == 0) return 0;
	if (n == 1) return 1;
	long long x = 0, y = 1,z;
	for (long long i = 2; i <= n; ++i)
	{
		z = x + y;
		x = y;
		y = z;
	}
	return y;
}

//���پ��������쳲�������-------------------------------------------
void multiply(long long a[][2], long long b[][2], long long c[][2])
{//2*2�������
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			c[i][j] = 0;
			for (int k = 0; k < 2; ++k)
				c[i][j] += a[i][k] * b[k][j];
		}
	}
}

void copy(long long a[][2], long long c[][2])
{//���ƾ���
	c[0][0] = a[0][0];
	c[0][1] = a[0][1];
	c[1][0] = a[1][0];
	c[1][1] = a[1][1];
}

void quickPower(long long a[][2],long long c[][2],int n)
{//���پ�����
	if (n == 1)
	{
		copy(a, c);
		return;
	}
	long long tmp[2][2] = { { 0 }, {0} };
	quickPower(a, tmp, n / 2);
	multiply(tmp, tmp, c);
	if (n % 2 == 1)
	{
		copy(c, tmp);
		multiply(a, tmp, c);
	}
}

long long fibonacci_mutrix_power(int n)
{//���þ����ݿ��ٽ��쳲�����
	if (n <= 32)//n��Сʱ���ü��䷨
		return fibonacci_memory(n);
	long long a[2][2] = { { 0, 1 }, { 1, 1 } };
	long long c[2][2] = { {0}, {0} };
	quickPower(a, c, n - 1);
	return c[1][1];
}

int main()
{
	int n;
	while (cin >> n)
		cout << fibonacci_mutrix_power(n) << endl;
	return 0;
}