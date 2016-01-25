
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

//算法导论 第31章 数论算法

/*
#include<iostream>
#include<cmath>

using namespace std;

void notTravialPow(int n)
{//判断一个数n是否是非平凡幂，即a^k = n,且k > 1,n > 1.
	bool yes = false;
	for (int a = 2; a <= sqrt(n); ++a)
	{
		int m = n,k = 0;
		while (m % a == 0)
		{
			m /= a;
			++k;
		}
		if (m == 1 && k > 1)
		{
			cout << a << '^' << k << " = " << n << endl;
			yes = true;
		}
	}
	if (!yes) cout << "travial pow" << endl;
}

int main()
{
	int n;
	while (cin >> n)
		notTravialPow(n);
	return 0;
}
*/
/*
#include<iostream>
#include<cassert>

using namespace std;
//求两个正整数的最大公约数
int euclid(int a, int b)
{//recursive euclid
	if (b == 0) return a;
	else if (a < b) return euclid(b, a);
	else return euclid(b, a % b);
}

int iter_euclid(int a, int b)
{//non-recursive euclid
	if (a < b) swap(a, b);
	while (b != 0)
	{
		int tmp = b;
		b = a % b;
		a = tmp;
	}
	return a;
}

int multiple_euclid(int a[],int size)
{//求任意个正整数的最大公约数
	assert(size >= 2);
	int tmp = euclid(a[size - 1], a[size - 2]);
	if (size == 2) return tmp;
	else
	{
		for (int i = size - 3; i >= 0; --i)
			tmp = euclid(a[i], tmp);
		return tmp;
	}
}

int multiple_lcm(int a[], int size)
{//求多个正整数的最小公倍数
	assert(size >= 2);
	int gcd = euclid(a[size - 1], a[size - 2]);
	int lcm = a[size - 1] * a[size - 2] / gcd;
	if (size == 2) return lcm;
	else
	{
		for (int i = size - 3; i >= 0; --i)
		{
			gcd = euclid(a[i], gcd);
			if (lcm % a[i] != 0) lcm = lcm * a[i] / gcd;
		}
		return lcm;
	}
}

int main()
{
	int num;
	while (cin >> num)
	{
		int *a = new int[num];
		for (int i = 0; i < num; ++i) cin >> a[i];
		cout << multiple_euclid(a, num) << endl;
		cout << multiple_lcm(a, num) << endl;
		delete[]a;
	}
	return 0;
}
*/

#include<iostream>

using namespace std;

struct triple
{//三元组
	int d, x, y;
	triple(int d_ = 0, int x_ = 0, int y_ = 0) :d(d_), x(x_), y(y_){}
};

triple extended_euclid(int a, int b)
{//扩展的欧几里得算法
	if (b == 0) return triple(a, 1, 0);
	else
	{
		triple t = extended_euclid(b, a % b);
		return triple(t.d, t.y, t.x - a / b * t.y);
	}
}

void modularLinearEquation(int a, int b, int n)
{//解模线性方程ax = b mod n
	bool a_greater_n = false;
	triple t;
	if (a > n)
	{//若a大于n
		a_greater_n = true;
		t = extended_euclid(a, n);//返回的是d = a * x + n * y
	}
	else t = extended_euclid(n, a);//返回的是d = n * x + a * y
	if (b % t.d == 0)
	{
		int x0;
		if (a_greater_n)
			x0 = t.x * b / t.d % n;
		else x0 = t.y * b / t.d % n;
		for (int i = 0; i != t.d; ++i)
			cout << (x0 + i * n / t.d) % n<< ' ';
		cout << endl;
		//printf("xi = (%d + %di) mod %d,i = 0,...,%d\n", x0, n / t.d, n, t.d - 1);
	}
	else cout << "No solution!" << endl;
}

int main()
{
	int a, b, n;
	while (cin >> a >> b >> n)
		modularLinearEquation(a, b, n);
	return 0;
}