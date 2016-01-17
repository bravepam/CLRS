
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

/*//(a)
void changeCoin(int n, vector<int> &coin, vector<int> &select)
{
	for (size_t i = 0; n != 0 && i != coin.size(); ++i)
	{
		select.push_back(n / coin[i]);
		n %= coin[i];
	}
	cout << "The solution of change-coin" << endl;
	for (size_t i = 0; i != select.size(); ++i)
		if (select[i] != 0)
			cout << coin[i] << " cent(s) : " << select[i] << endl;
}

int main()
{
	int n;
	cout << "Enter the money,> 0: ";
	while (cin >> n)
	{//����coin = {25,10,1}����{5,4,1}�ȵȿ��ܲ��ܵõ����Ž��
		vector<int> coin = { 25, 10, 5, 1 }, select;
		changeCoin(n, coin, select);
	}
	return 0;
}
*/

//(d)�ö�̬�滮���ɽ����һ����
/*sln[i]��ʾ�һ�i��Ǯʱ����СӲ������coin[j]��ʾ��j��Ӳ�ҵ���ֵ������j = 0,1,2,3...k
 *Ӳ����ֵ�������У�ʱ��O(nk)
 *			 0,							i = 0
 *sln[i] = {
 *			 min{sln[i - s[j]]} + 1,		i > 1
 *			 j >= 1 && s[j] <= i
 */
void changeCoin(int n, vector<int> &coin, vector<int> &sln,vector<int> &select)
{//�����һ�Ӳ��
	for (int i = 1; i <= n; ++i)
	{//��1�ֿ�ʼ
		for (int j = 0; j != coin.size() && coin[j] <= i; ++j)
		{//�ӵ�0��Ӳ�ҿ�ʼ
			if (sln[i] == 0 || sln[i - coin[j]] + 1 < sln[i])
			{//����������
				sln[i] = sln[i - coin[j]] + 1;//����״̬ת�Ʒ���
				select[i] = coin[j];//���µ�ʱ��ѡ��
			}
		}
	}
}

void printSolution(vector<int> &select, int n)
{//��ӡӲ��ѡ�񷽰����ݹ�
	if (n > 0)
	{
		cout << "select: " << select[n] << endl;
		printSolution(select, n - select[n]);
	}
}

int main()
{
	int n,value;
	cout << "Enter the money: ";
	while (cin >> n)
	{
		cin.clear();
		vector<int> coin, select(n + 1), sln(n + 1);
		cout << "Enter the value of coins" << endl;
		while (cin >> value)
			coin.push_back(value);
		cin.clear();
		sort(coin.begin(), coin.end());
		changeCoin(n, coin, sln, select);
		printSolution(select, n);
		for (size_t i = 0; i != coin.size(); ++i)
			cout << coin[i] << ' ';
		cout << endl;
		for (size_t i = 0; i != sln.size(); ++i)
			cout << sln[i] << ' ';
		cout << endl;
		for (size_t i = 0; i != select.size(); ++i)
			cout << select[i] << ' ';
		cout << endl;
	}
	return 0;
}