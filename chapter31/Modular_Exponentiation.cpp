
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

int modularExponentiation2(int a, int b, int n)
{//模取幂2，反向遍历二进制b
	vector<char> bit;
	binary(b, bit);
	int t = a, d = 1;
	for (int i = 0; i != bit.size(); ++i)
	{//因而从第一个位开始遍历
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