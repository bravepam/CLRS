//动态规划问题1——装配线问题----------------------------------------------------------------------------

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
	{//先打印前面的路线
		if(last == 1)
			printStation(line,line[1][n],n - 1);
		else printStation(line,line[2][n],n - 1);
	}
	cout << "line: " << last << ",station: " << n << endl;//再打印当前
}

int main()
{
	int a[3][n + 1] = {{0},{0,7,9,3,4,8,4},{0,8,5,6,4,5,7}};
	int t[3][n + 1] = {{0},{0,2,3,1,3,4},{0,2,1,2,2,1}};
	int e[3] = {0,2,4},x[3] = {0,3,2};
	int f[3][n + 1],line[3][n + 1],FASTEST,LAST;//FASTEST记录最小代价，LAST记录最后一站选择的是哪条装配线
	fastestWay(a,t,f,line,e,x,&FASTEST,&LAST);
	cout << "The minimal cost is " << FASTEST << endl;
	printStation(line,LAST,n);
	return 0;
}
*/


//问题2——矩阵链乘法-------------------------------------------------------------------------------------
/*
#include<iostream>

using namespace std;
const int n = 6;
const int MAX = 0x7fffffff;

void matrixMultiplyOrder(int *d,int (*m)[n + 1],int (*s)[n + 1])
{//自底向上的动态规划版本
	for(int length = 1;length <= n;++length)//length矩阵链长度
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
{//简单直观的递归版本，自顶向下
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
{//自顶向下的做备忘录版本，虽然也是不断递归调用，但是与递归版本所不同的是，
 //每一次计算后，它会保存已计算的项，下次直接查询即可，无需再算。
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
{//初始化，主要工作调用checkMemo实现
	for(int i = 0;i != n + 1;i++)
		for(int j = i;j != n + 1;j++)
			m[i][j] = MAX;
	checkMemo(d,m,s,1,n);
}

void worstMatrixMultiplyOrder(int *d,int (*m)[n + 1],int (*s)[n + 1])
{//求最大相乘次数
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
{//打印相乘次序
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
	//d记录矩阵维度；m[i][j]记录矩阵链Ai...Aj最小相乘次数,s[i][j]则记录此链在何处裂开可取得最小相乘次数
	int d[n + 1],m[n + 1][n + 1],s[n + 1][n + 1];
	printf("请输入矩阵链(%d个)中诸矩阵维数(%d个)\n",n,n + 1);
	for(int i = 0; i <= n;++i)
		cin >> d[i];
	//recursiveMatrixMultiplyOrder(d,m,s,1,n);//递归版本
	//memoizedMatrixMultiplyOrder(d,m,s);//做备忘录版本，注意与递归的区别
	matrixMultiplyOrder(d,m,s);//动态规划版本
	//worstMatrixMultiplyOrder(d,m,s);//最差相乘次序版本
	cout << "最佳(差)相乘次序：";
	printOrder(s,1,n);
	cout << endl << "相乘总次数为：" << m[1][n] << endl;
	return 0;
}
*/

//问题3——最长公共子序列(The longest common sequence)-----------------------------------------------------
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
{//理解递归式即可
	for(int i = 0;i < m;++i)
		len[i][0] = 0;
	for(int i = 0;i < n;++i)
		len[0][i] = 0;
	for(int i = 1;i < m;++i)
	{
		for(int j = 1;j < n;++j)
		{
			if(x[i] == y[j])
			{//若相等，说明序列长度又可以增加1了
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
	cout << "请输入序列X" << endl;
	for(int i = 1;i < m;++i)
		cin >> x[i];
	cout << "请输入序列Y" << endl;
	for(int i = 1;i < n;++i)
		cin >> y[i];
	LCSlength(x,y,len,d);
	cout << "The length of LCS is " << len[m - 1][n - 1] << endl;
	printLCS(d,x,m - 1,n - 1);
	cout << endl;
	return 0;
}
*/

//问题四——最长递增子序列(The longest increasing sequence)-------------------------------------------------
//解法一，对序列s先排序得到s'，然后找到两者的最长公共子序列

//解法二,时间复杂度O(n^2)，动态规划
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
	int maxlen = 1,last = 0;//maxlen记录最长序列长度,last记录最长序列最后一个元素下标
	int *lislen = new int[len],//记录以a[i]结尾的最长序列长度
		*pre = new int[len];//同时记录此时a[i]前驱，并查集
	for(int i = 0;i != len;++i)
	{
		pre[i] = i;
		lislen[i] = 1;
	}
	for(int i = 1;i != len;++i)
	{
		for(int j = 0;j != i;++j)
		{
			if(a[j] < a[i] && lislen[j] + 1 > lislen[i])//这两个条件是必要的
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
	printLIS(a,pre,last);//顺序输出最长序列
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

//解法三——时间复杂度O(nlgn)

/*
void LISlengthEx(int a[],int len)
{
	int *lisIndex = new int[len],//lisIndex[i]记录长度为i + 1的所有可能序列中最末元素最小的元素的下标
		*pre = new int[len];//pre[i]记录序列中a[i]前驱
	int maxlen = 1;
	lisIndex[0] = 0;//初始
	for(int i = 0;i != len;++i)
		pre[i] = i;
	for(int i = 1;i != len;++i)
	{
		int low = 0,high = maxlen - 1;
		while(high - low > 1)
		{//采用二分查找
			int mid = (low + high) >> 1;
			if(a[ lisIndex[mid] ] > a[i])
				high = mid;
			else low = mid;
		}
		if(a[ lisIndex[low] ] < a[i])//当high - low == 1时，若a[ lisIndex[low] ]依然还小于a[i]
			low = high;//则将high赋给low
		if(low == maxlen - 1)//当low指向lisIndex中最后一个元素
		{
			if(a[ lisIndex[low] ] < a[i])//lisIndex中最后一个下表对应元素比a[i]小，
			{//即lisIndex中所有元素都比a[i]小
				lisIndex[maxlen++] = i;
				pre[i] = lisIndex[low];
				continue;
			}
		}
		if(low == 0)//若a[i]比所有元素都小
		{
			if(a[ lisIndex[low] ] >= a[i])//发现了长度为1的最长子序列的最后元素更小
			{//等号是必须的，因为序列中可能存在重复的最小元素.其实这里最不必要的是大于号>
				lisIndex[low] = i;
				continue;
			}
		}
		//正常情况
		lisIndex[low] = i;
		pre[i] = lisIndex[low - 1];
	}
	cout << "The length of LIS is " << maxlen << endl;
	cout << "The LIS as follows" << endl;
	printLIS(a,pre,lisIndex[maxlen - 1]);//调用解法二中的printLIS函数
	delete []lisIndex;delete []pre;
}
*/

//问题五——最优二叉查找树（OptimalBST）--------------------------------------------------------
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
	float w[n + 2][ n + 1];//我[i][j]记录子树ki...kj的总体概率和
	for(int i = 1;i <= n + 1;++i)
	{
		cost[i][i - 1] = v[i - 1];
		w[i][i - 1] = v[i - 1];
	}
	for(int length = 1;length <= n;++length)//length为子树长度，即所含节点数
	{
		for(int i = 1;i <= n - length + 1;++i)
		{
			int j = i + length - 1;
			w[i][j] = w[i][j - 1] + k[j] + v[j];
			//可调用computeTotalProbability计算w[i][j],程序其他部分需做细微修改
			//w[i][j] = computeTotalProbability(k,v,i,j);
			cost[i][j] = MAX;
			for(int r = i;r <= j;++r)//此处可利用root[i - 1][j] <= root[i][j] <= root[i + 1][j]
				//这个事实修改程序，减少测试次数
			{//一个个节点探测
				float tmp = cost[i][r - 1] + cost[r + 1][j] + w[i][j];
				if(tmp < cost[i][j])
				{
					cost[i][j] = tmp;
					root[i][j] = r;//记录使期望代价最小的根
				}
			}
		}
	}
}

void constructOptimalBST(int (*root)[n + 1],int i,int j)
{
	int r = root[i][j];
	if(i == 1 && j == n) cout << 'k' << r << "是根" << endl;
	if(i == r)//先打印左子树
		cout << 'd' << i - 1 << "是k" << r << "的左孩子" << endl;
	else
	{
		cout << 'k' << root[i][r - 1] << "是k" << r << "的左孩子" << endl;
		constructOptimalBST(root,i,r - 1);
	}
	if(r == j)//再打印右子树
		cout << 'd' << j << "是k" << r << "的右孩子" << endl;
	else
	{
		cout << 'k' << root[r + 1][j] << "是k" << r << "的右孩子" << endl;
		constructOptimalBST(root,r + 1,j);
	}
}

int main()
{
	//float k[n + 1] = {0.0,0.04,0.06,0.08,0.02,0.10,0.12,0.14},v[n + 1] = {0.06,0.06,0.06,0.06,0.05,0.05,0.05,0.05},
						//cost[n + 2][n + 1];//cost[i][j]记录子树ki...kj的期望代价，子树可以只包含一份虚拟键v
	float k[n + 1] = {0,0.15,0.10,0.05,0.10,0.20},v[n + 1] = {0.05,0.10,0.05,0.05,0.05,0.10},
						cost[n + 2][n + 1];
	int root[n + 2][n + 1];//记录每次使子树期望代价最小的根
	optimalBST(k,v,cost,root);
	cout << "The expection cost of optimal BST is " << cost[1][n] << endl;
	cout << "The construction of optimal BST as follows:" << endl;
	constructOptimalBST(root,1,n);
	getchar();
	return 0;
}
*/
