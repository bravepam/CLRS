
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<vector>

using namespace std;

struct triple
{//��Ԫ��
	int d, x, y;
	triple(int d_ = 0, int x_ = 0, int y_ = 0) :d(d_), x(x_), y(y_){}
};

triple extended_euclid(int a, int b)
{//��չ��ŷ������㷨
	if (b == 0) return triple(a, 1, 0);
	else
	{
		triple t = extended_euclid(b, a % b);
		return triple(t.d, t.y, t.x - a / b * t.y);
	}
}

int multiplicativeInverse(int a, int n)
{//��ax = 1 mod n,��a��ģn�˷���Ԫ
	bool a_greater_n = false;
	triple t;
	if (a > n)
	{//��a����n
		a_greater_n = true;
		t = extended_euclid(a, n);//���ص���d = a * x + n * y
	}
	else t = extended_euclid(n, a);//���ص���d = n * x + a * y
	int x0;
	if (a_greater_n) x0 = t.x / t.d % n;
	else x0 = t.y / t.d % n;
	while (x0 < 0) x0 += n;
	return x0;
}

#include<iostream>

using namespace std;

int main()
{
	int a, n;
	while (cin >> a >> n)
		cout << multiplicativeInverse(a, n) << endl;
	return 0;
}

//void chineseRemainderTheorem(vector<int> &a, vector<int> &n)
//{//�й���������
//	vector<int> m(a.size(), 1), c(a.size());
//	int x = 0,n_p = 1;
//	for (int i = 0; i != n.size(); ++i)
//	{
//		n_p *= n[i];//����ni�Ļ�
//		for (int j = 0; j != m.size(); ++j)//����ÿ��mj = n1 * n2 *...*nj-1 * nj+1 *...*nn
//			if (i != j)
//				m[j] *= n[i];
//	}
//	for (int i = 0; i != m.size(); ++i)
//	{//����ÿ��ci
//		c[i] = m[i] * multiplicativeInverse(m[i], n[i]);
//		x += a[i] * c[i];
//	}
//	printf("x = %d mod %d\n", x % n_p, n_p);
//}
//
//int main()
//{
//	while (true)
//	{
//		int aa,nn;
//		vector<int> a, n;
//		while (cin >> aa >> nn)
//		{
//			a.push_back(aa);
//			n.push_back(nn);
//		}
//		cin.clear();
//		chineseRemainderTheorem(a, n);
//	}
//	return 0;
//}