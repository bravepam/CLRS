//�㷨����15-3 �༭����

#include<iostream>
#include<vector>
#include<string>

using namespace std;

//����ÿ�ֲ����Ĵ���
#define COPY			1
#define REPLACE		1
#define ERASE			1
#define INSERT		1
#define TWIDDLE		1
#define KILL			1
#define MAX			0x7fffffff

enum operation{copy,replace,erase,insert,twiddle,kill};//�������

void editDistance(const string &x,const string &y,
	vector<vector<int> > &distance,vector<vector<int> > &op,int &k)
{//����༭���룬ǧ��ע���±�
	for(int i = 0;i != distance[0].size();i++)
	{//���ַ���������Ϊ����i���ַ����ַ������༭����
		distance[0][i] = i * INSERT;
		op[0][i] = insert;
	}
	for(int i = 0;i != distance.size();++i)
	{//����i���ַ����ַ�����Ϊ���ַ����������༭����
		distance[i][0] = i * ERASE;
		op[i][0] = erase;
	}
	for(int i = 0;i != x.size();++i)
	{//ע��i��j�ֱ��ʾ�ַ���x��y���±꣬����distance���±�
		for(int j = 0;j != y.size();++j)
		{
			distance[i + 1][j + 1] = MAX;//�����x[i]��y[j]ʱ����ʾ�ü����1...i+1��1...j+1�ı༭������
			if(x[i] == y[j])
			{//��ȣ�����Ҫcopy����
				distance[i + 1][j + 1] = distance[i][j] + COPY;
				op[i + 1][j + 1] = operation::copy;
			}
			if(x[i] != y[j] && (distance[i][j] + REPLACE) < distance[i + 1][j + 1])
			{//���ȣ�replace����
				distance[i + 1][j + 1] = distance[i][j] + REPLACE;
				op[i + 1][j + 1] = operation::replace;
			}
			if((i >= 1) && (j >= 1) && (x[i] == y[j - 1]) && (x[i - 1] == y[j]) && 
				(distance[i - 1][j - 1] + TWIDDLE < distance[i + 1][j + 1]))
			{//��������
				distance[i + 1][j + 1] = distance[i - 1][j - 1] + TWIDDLE;
				op[i + 1][j + 1] = operation::twiddle;
			}
			if(distance[i][j + 1] + ERASE < distance[i + 1][j + 1])
			{//ɾ������
				distance[i + 1][j + 1] = distance[i][j + 1] + ERASE;
				op[i + 1][j + 1] = operation::erase;
			}
			if(distance[i + 1][j] + INSERT < distance[i + 1][j + 1])
			{//�������
				distance[i + 1][j + 1] = distance[i + 1][j] + INSERT;
				op[i + 1][j + 1] = operation::insert;
			}
		}
	}
	size_t x_len = x.size(),y_len = y.size();
	for(int i = 0;i != x_len;++i)
	{//���kill����
		if(distance[i + 1][y_len] + KILL < distance[x_len][y_len])
		{
			distance[x_len][y_len] = distance[i + 1][y_len] + KILL;
			op[x_len][y_len] = operation::kill;
			k = i + 1;//����kill�ĵط������һ��
		}
	}
}

void printOperation(int oper)
{//��ӡ����
	if(oper == operation::copy)
		cout << "copy" << endl;
	else if(oper == operation::replace)
		cout << "replace" << endl;
	else if(oper == operation::erase)
		cout << "erase" << endl;
	else if(oper == operation::insert)
		cout << "insert" << endl;
	else if(oper == operation::twiddle)
		cout << "twiddle" << endl;
	else cout << "kill" << endl;
}

void print(vector<vector<int> > &op,int k,int i,int j)
{//�ݹ�������ȴ�ӡǰ��ģ��ٴ�ӡ�����
	if(i == 0 || j == 0) return ;
	if(op[i][j] == operation::copy || op[i][j] == operation::replace)
		print(op,k,i - 1,j - 1);
	else if(op[i][j] == operation::twiddle)
		print(op,k,i - 2,j - 2);
	else if(op[i][j] == operation::erase)
		print(op,k,i - 1,j);
	else if(op[i][j] == operation::insert)
		print(op,k,i,j - 1);
	else if(op[i][j] == operation::kill)
		print(op,k,k,j);
	printOperation(op[i][j]);
}

int main()
{
	string x,y;
	while(cin >> x >> y)
	{
		size_t x_len = x.size(),y_len = y.size();
		vector<vector<int> > dis(x_len + 1),op(x_len + 1);
		for(int i = 0;i != x_len + 1;++i)
		{
			dis[i].resize(y_len + 1);
			op[i].resize(y_len + 1);
		}
		int k = 0;
		editDistance(x,y,dis,op,k);
		for(int i = 1;i != dis.size();++i)
		{
			for(int j = 1;j != dis[i].size();j++)
				cout << dis[i][j] << ' ';
			cout << endl;
		}
		cout << "The minimum cost is " << dis[x_len][y_len] << endl;
		cout << endl << "operation sequence" << endl;
		print(op,k,x_len,y_len);
	}
	return 0;
}