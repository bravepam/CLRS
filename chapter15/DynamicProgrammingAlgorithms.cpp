//��̬�滮����1����װ��������----------------------------------------------------------------------------

/*#include<iostream>

using namespace std;
const int n = 6;

void fastestWay(int (*a)[n + 1],int (*t)[n + 1],int (*f)[n + 1],int (*line)[n + 1],
				int *e,int *x,int *fastest,int *last)
{
	f[1][1] = e[1] + a[1][1],f[2][1] = e[2] + a[2][1];
	for(int i = 2; i <= n;++i)
	{
		if((f[1][i - 1] + a[1][i]) <= (f[2][i - 1] + t[2][i - 1] + a[1][i]))
		{
			f[1][i] = f[1][i - 1] + a[1][i];
			line[1][i] = 1;
		}
		else
		{
			f[1][i] = f[2][i - 1] + t[2][i - 1] + a[1][i];
			line[1][i] = 2;
		}
		if((f[2][i - 1] + a[2][i]) <= (f[1][i - 1] + t[1][i - 1] + a[2][i]))
		{
			f[2][i] = f[2][i - 1] + a[2][i];
			line[2][i]= 2;
		}
		else
		{
			f[2][i] = f[1][i - 1] + t[1][i - 1] + a[2][i];
			line[2][i] = 1;
		}
		//cout << i << "\t" << f[1][i] << "\t" << f[1][i] << endl;
	}
	if(f[1][n] + x[1] <= f[2][n] + x[2])
	{
		*fastest = f[1][n] + x[1];
		*last = 1;
	}
	else
	{
		*fastest = f[2][n] + x[2];
		*last = 2;
	}
}

void printStation(int (*line)[n + 1],int last,int n)
{
	if(n != 1) 
	{//�ȴ�ӡǰ���·��
		if(last == 1)
			printStation(line,line[1][n],n - 1);
		else printStation(line,line[2][n],n - 1);
	}
	cout << "line: " << last << ",station: " << n << endl;//�ٴ�ӡ��ǰ
}

int main()
{
	int a[3][n + 1] = {{0},{0,7,9,3,4,8,4},{0,8,5,6,4,5,7}};
	int t[3][n + 1] = {{0},{0,2,3,1,3,4},{0,2,1,2,2,1}};
	int e[3] = {0,2,4},x[3] = {0,3,2};
	int f[3][n + 1],line[3][n + 1],FASTEST,LAST;//FASTEST��¼��С���ۣ�LAST��¼���һվѡ���������װ����
	fastestWay(a,t,f,line,e,x,&FASTEST,&LAST);
	cout << "The minimal cost is " << FASTEST << endl;
	printStation(line,LAST,n);
	return 0;
}
*/


//����2�����������˷�-------------------------------------------------------------------------------------
/*
#include<iostream>

using namespace std;
const int n = 6;
const int MAX = 0x7fffffff;

void matrixMultiplyOrder(int *d,int (*m)[n + 1],int (*s)[n + 1])
{//�Ե����ϵĶ�̬�滮�汾
	for(int length = 1;length <= n;++length)//length����������
	{
		for(int i = 1;i <= n - length + 1;++i)
		{
			int j = i + length - 1;
			m[i][j] = MAX;
			if(i == j) m[i][j] = 0;
			else
			{
				for(int k = i;k <= j - 1;++k)
				{
					int tmp = m[i][k] + m[k + 1][j] + d[i - 1] * d[k] * d[j];
					if(tmp < m[i][j])
					{
						m[i][j] = tmp;
						s[i][j] = k;
					}
				}
			}
		}
	}
}

int recursiveMatrixMultiplyOrder(int *d,int (*m)[n + 1],int (*s)[n + 1],int i,int j)
{//��ֱ�۵ĵݹ�汾���Զ�����
	if(i == j) return 0;
	m[i][j] = MAX;
	for(int k = i;k != j;k++)
	{
		int tmp = recursiveMatrixMultiplyOrder(d,m,s,i,k) + 
				  recursiveMatrixMultiplyOrder(d,m,s,k + 1,j) +
				  d[i - 1] * d[k] * d[j];
		if(tmp < m[i][j])
		{
			m[i][j] = tmp;
			s[i][j] = k;
		}
	}
	return m[i][j];
}

int checkMemo(int *d,int (*m)[n + 1],int (*s)[n + 1],int i,int j)
{//�Զ����µ�������¼�汾����ȻҲ�ǲ��ϵݹ���ã�������ݹ�汾����ͬ���ǣ�
 //ÿһ�μ�������ᱣ���Ѽ������´�ֱ�Ӳ�ѯ���ɣ��������㡣
	if(m[i][j] < MAX) return m[i][j];
	if(i == j)
	{
		m[i][j] = 0;
		return m[i][j];
	}
	for(int k = i; k != j;k++)
	{
		int tmp = checkMemo(d,m,s,i,k) + checkMemo(d,m,s,k + 1,j) + d[i - 1] * d[k] * d[j];
		if(tmp < m[i][j])
		{
			m[i][j] = tmp;
			s[i][j] = k;
		}
	}
	return m[i][j];
}

void memoizedMatrixMultiplyOrder(int *d,int (*m)[n + 1],int (*s)[n + 1])
{//��ʼ������Ҫ��������checkMemoʵ��
	for(int i = 0;i != n + 1;i++)
		for(int j = i;j != n + 1;j++)
			m[i][j] = MAX;
	checkMemo(d,m,s,1,n);
}

void worstMatrixMultiplyOrder(int *d,int (*m)[n + 1],int (*s)[n + 1])
{//�������˴���
	for(int length = 1;length <= n;++length)
	{
		for(int i = 1;i <= n - length + 1;++i)
		{
			int j = i + length - 1;
			if(i == j)m[i][j] = 0;
			else
			{
				m[i][j] = -1;
				for(int k = i;k != j;++k)
				{
					int tmp = m[i][k] + m[k + 1][j] + d[i - 1] * d[k] * d[j];
					if(tmp > m[i][j])
					{
						m[i][j] = tmp;
						s[i][j] = k;
					}
				}
			}
		}
	}
}

void printOrder(int (*s)[n + 1],int i,int j)
{//��ӡ��˴���
	if(i == j) cout << "A" << i;
	else
	{
		cout << "(";
		printOrder(s,i,s[i][j]);
		printOrder(s,s[i][j] + 1,j);
		cout << ")";
	}
}

int main()
{//30 35 15 5 10 20 25
	//d��¼����ά�ȣ�m[i][j]��¼������Ai...Aj��С��˴���,s[i][j]���¼�����ںδ��ѿ���ȡ����С��˴���
	int d[n + 1],m[n + 1][n + 1],s[n + 1][n + 1];
	printf("�����������(%d��)�������ά��(%d��)\n",n,n + 1);
	for(int i = 0; i <= n;++i)
		cin >> d[i];
	//recursiveMatrixMultiplyOrder(d,m,s,1,n);//�ݹ�汾
	//memoizedMatrixMultiplyOrder(d,m,s);//������¼�汾��ע����ݹ������
	matrixMultiplyOrder(d,m,s);//��̬�滮�汾
	//worstMatrixMultiplyOrder(d,m,s);//�����˴���汾
	cout << "���(��)��˴���";
	printOrder(s,1,n);
	cout << endl << "����ܴ���Ϊ��" << m[1][n] << endl;
	return 0;
}
*/

//����3���������������(The longest common sequence)-----------------------------------------------------
/*
#include<iostream>

using namespace std;
#define inc 0
#define up 1
#define left 2

const int m = 8;
const int n = 7;

template<typename T>
void LCSlength(T x[],T y[],int len[m][n],int d[m][n])
{//���ݹ�ʽ����
	for(int i = 0;i < m;++i)
		len[i][0] = 0;
	for(int i = 0;i < n;++i)
		len[0][i] = 0;
	for(int i = 1;i < m;++i)
	{
		for(int j = 1;j < n;++j)
		{
			if(x[i] == y[j])
			{//����ȣ�˵�����г����ֿ�������1��
				len[i][j] = len[i - 1][j - 1] + 1;
				d[i][j] = inc;
			}
			else if(len[i - 1][j] >= len[i][j - 1])
			{
				len[i][j] = len[i - 1][j];
				d[i][j] = up;
			}
			else
			{
				len[i][j] = len[i][j - 1];
				d[i][j] = left;
			}
		}
	}
}

template <typename T>
void printLCS(int d[m][n],T *x,int i,int j)
{
	if(i == 0 || j == 0) return ;
	if(d[i][j] == inc /*&& (i <= m && j <= n))
	{
		printLCS(d,x,i - 1,j - 1);
		cout << x[i] << ' ';
	}
	else if(d[i][j] == up)
		printLCS(d,x,i - 1,j);
	else
		printLCS(d,x,i,j - 1);
}

int main()
{//1 0 0 1 0 1 0 1,0 1 0 1 1 0 1 1 0
 //A B C B D A B,B D C A B A
	char x[m],y[n];
	int len[m][n],d[m][n];
	cout << "����������X" << endl;
	for(int i = 1;i < m;++i)
		cin >> x[i];
	cout << "����������Y" << endl;
	for(int i = 1;i < n;++i)
		cin >> y[i];
	LCSlength(x,y,len,d);
	cout << "The length of LCS is " << len[m - 1][n - 1] << endl;
	printLCS(d,x,m - 1,n - 1);
	cout << endl;
	return 0;
}
*/

//�����ġ��������������(The longest increasing sequence)-------------------------------------------------
//�ⷨһ��������s������õ�s'��Ȼ���ҵ����ߵ������������

//�ⷨ��,ʱ�临�Ӷ�O(n^2)����̬�滮
/*
#include<iostream>

using namespace std;

template <typename T>
void printLIS(T *a,int *pre,int pos)
{
	if(pre[pos] != pos)
		printLIS(a,pre,pre[pos]);
	cout << a[pos] << ' ';
}

template <typename T>
void LISlength(T a[],int len)
{
	int maxlen = 1,last = 0;//maxlen��¼����г���,last��¼��������һ��Ԫ���±�
	int *lislen = new int[len],//��¼��a[i]��β������г���
		*pre = new int[len];//ͬʱ��¼��ʱa[i]ǰ�������鼯
	for(int i = 0;i != len;++i)
	{
		pre[i] = i;
		lislen[i] = 1;
	}
	for(int i = 1;i != len;++i)
	{
		for(int j = 0;j != i;++j)
		{
			if(a[j] < a[i] && lislen[j] + 1 > lislen[i])//�����������Ǳ�Ҫ��
			{
				lislen[i] = lislen[j] + 1;
				pre[i] = j;
				if(lislen[i] > maxlen)
				{
					maxlen = lislen[i];
					last = i;
				}
			}
		}
	}
	cout << "The length of LIS is " << maxlen << endl;
	cout << "The LIS as follows" << endl;
	printLIS(a,pre,last);//˳����������
	delete []lislen;delete []pre;
}

int main()
{
	int N;
	cout << "Enter the number of the element(s): ";
	while(cin >> N)
	{
		char *a = new char[N];
		cout << "Enter " << N << " element(s)" << endl;
		for(int i = 0;i != N;++i)
			cin >> a[i];
		LISlength(a,N);
		delete []a;
		cout << endl << "Enter the number of the element(s): ";
	}
	return 0;
}
*/

//�ⷨ������ʱ�临�Ӷ�O(nlgn)

/*
void LISlengthEx(int a[],int len)
{
	int *lisIndex = new int[len],//lisIndex[i]��¼����Ϊi + 1�����п�����������ĩԪ����С��Ԫ�ص��±�
		*pre = new int[len];//pre[i]��¼������a[i]ǰ��
	int maxlen = 1;
	lisIndex[0] = 0;//��ʼ
	for(int i = 0;i != len;++i)
		pre[i] = i;
	for(int i = 1;i != len;++i)
	{
		int low = 0,high = maxlen - 1;
		while(high - low > 1)
		{//���ö��ֲ���
			int mid = (low + high) >> 1;
			if(a[ lisIndex[mid] ] > a[i])
				high = mid;
			else low = mid;
		}
		if(a[ lisIndex[low] ] < a[i])//��high - low == 1ʱ����a[ lisIndex[low] ]��Ȼ��С��a[i]
			low = high;//��high����low
		if(low == maxlen - 1)//��lowָ��lisIndex�����һ��Ԫ��
		{
			if(a[ lisIndex[low] ] < a[i])//lisIndex�����һ���±��ӦԪ�ر�a[i]С��
			{//��lisIndex������Ԫ�ض���a[i]С
				lisIndex[maxlen++] = i;
				pre[i] = lisIndex[low];
				continue;
			}
		}
		if(low == 0)//��a[i]������Ԫ�ض�С
		{
			if(a[ lisIndex[low] ] >= a[i])//�����˳���Ϊ1��������е����Ԫ�ظ�С
			{//�Ⱥ��Ǳ���ģ���Ϊ�����п��ܴ����ظ�����СԪ��.��ʵ�������Ҫ���Ǵ��ں�>
				lisIndex[low] = i;
				continue;
			}
		}
		//�������
		lisIndex[low] = i;
		pre[i] = lisIndex[low - 1];
	}
	cout << "The length of LIS is " << maxlen << endl;
	cout << "The LIS as follows" << endl;
	printLIS(a,pre,lisIndex[maxlen - 1]);//���ýⷨ���е�printLIS����
	delete []lisIndex;delete []pre;
}
*/

//�����塪�����Ŷ����������OptimalBST��--------------------------------------------------------
/*
#include<iostream>

using namespace std;
const int n = 5;
const int MAX = 0x7fffffff;

float computeTotalProbability(float *k,float *v,int i,int j)
{
	float sum = v[i - 1];
	for(int x = i;x <= j;++x)
		sum += k[x] + v[x];
	return sum;
}

void optimalBST(float *k,float *v,float (*cost)[n + 1],int (*root)[n + 1])
{
	float w[n + 2][ n + 1];//��[i][j]��¼����ki...kj��������ʺ�
	for(int i = 1;i <= n + 1;++i)
	{
		cost[i][i - 1] = v[i - 1];
		w[i][i - 1] = v[i - 1];
	}
	for(int length = 1;length <= n;++length)//lengthΪ�������ȣ��������ڵ���
	{
		for(int i = 1;i <= n - length + 1;++i)
		{
			int j = i + length - 1;
			w[i][j] = w[i][j - 1] + k[j] + v[j];
			//�ɵ���computeTotalProbability����w[i][j],����������������ϸ΢�޸�
			//w[i][j] = computeTotalProbability(k,v,i,j);
			cost[i][j] = MAX;
			for(int r = i;r <= j;++r)//�˴�������root[i - 1][j] <= root[i][j] <= root[i + 1][j]
				//�����ʵ�޸ĳ��򣬼��ٲ��Դ���
			{//һ�����ڵ�̽��
				float tmp = cost[i][r - 1] + cost[r + 1][j] + w[i][j];
				if(tmp < cost[i][j])
				{
					cost[i][j] = tmp;
					root[i][j] = r;//��¼ʹ����������С�ĸ�
				}
			}
		}
	}
}

void constructOptimalBST(int (*root)[n + 1],int i,int j)
{
	int r = root[i][j];
	if(i == 1 && j == n) cout << 'k' << r << "�Ǹ�" << endl;
	if(i == r)//�ȴ�ӡ������
		cout << 'd' << i - 1 << "��k" << r << "������" << endl;
	else
	{
		cout << 'k' << root[i][r - 1] << "��k" << r << "������" << endl;
		constructOptimalBST(root,i,r - 1);
	}
	if(r == j)//�ٴ�ӡ������
		cout << 'd' << j << "��k" << r << "���Һ���" << endl;
	else
	{
		cout << 'k' << root[r + 1][j] << "��k" << r << "���Һ���" << endl;
		constructOptimalBST(root,r + 1,j);
	}
}

int main()
{
	//float k[n + 1] = {0.0,0.04,0.06,0.08,0.02,0.10,0.12,0.14},v[n + 1] = {0.06,0.06,0.06,0.06,0.05,0.05,0.05,0.05},
						//cost[n + 2][n + 1];//cost[i][j]��¼����ki...kj���������ۣ���������ֻ����һ�������v
	float k[n + 1] = {0,0.15,0.10,0.05,0.10,0.20},v[n + 1] = {0.05,0.10,0.05,0.05,0.05,0.10},
						cost[n + 2][n + 1];
	int root[n + 2][n + 1];//��¼ÿ��ʹ��������������С�ĸ�
	optimalBST(k,v,cost,root);
	cout << "The expection cost of optimal BST is " << cost[1][n] << endl;
	cout << "The construction of optimal BST as follows:" << endl;
	constructOptimalBST(root,1,n);
	getchar();
	return 0;
}
*/
