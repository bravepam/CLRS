#include<iostream>
#include<vector>
#include<cmath>
#include<assert.h>

using namespace std;

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
//----------------Miller-Rabin素数测试法---------------------------------
int random(int seed,int upper)
{//获得随机数，用于基a，范围[1,n - 1],upper = n -1
	srand(unsigned int(seed));
	return rand() % upper + 1;
}

void power_presentation(int n, int &t, int &u)
{//将n转换为u * 2^t表示，其中u是奇数
	t = 0; u = n;
	while (u % 2 == 0)
	{
		++t;
		u /= 2;
	}
}

bool witness(int a,int n)
{//测试在基a下，n是否为合数。//根据费马定理和推论31.35
	int t = 0, u = 0;
	power_presentation(n - 1, t, u);
	int y = 0,x = modularExponentiation(a, u, n);//先计算出a^u mod n
	for (int i = 1; i <= t; ++i)
	{//再连续平方t次
		y = x * x % n;
		if (y == 1 && x != 1 && x != n - 1)//若此次结果为1，而上次结果是1的非平凡平方根
			return true;//则根据推论31.35可得n为合数
		x = y;
	}
	if (x != 1) return true;//若最后的到的模不是1，则根据费马定理可知肯定不是素数
	return false;
}

bool miller_rabin(int s, int n)
{//素数测试
	assert(n > 1);
	if (n == 2) return true;
	for (int i = 0; i != s; ++i)
	{//随机选取基，测试s次
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