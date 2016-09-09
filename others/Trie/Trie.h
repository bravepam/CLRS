#ifndef TRIE_H
#define TRIE_H

#include<string>
#include<unordered_map>
#include<vector>
#include<utility>

using namespace std;

class Trie
{//前缀树/字典树
private:
	struct node
	{
		int count; //从根到该节点的路径所构成的单词出现的次数，不包括该节点
		unordered_map<char, node*> children; //孩子列表，它们拥有同一前缀
		node() :count(0), children(){}
	};
private:
	node *root;

	void destroy(node*);
	void countingFrequency(node*, string&, vector<pair<string, int>>&)const;
public:
	Trie() :root(new node()){}
	void create(const vector<string>&);
	void insert(const string&);
	int search(const string&)const;
	//统计所有出现的单词的频数
	vector<pair<string, int>> countingFrequency(int, int = 10)const;
	void clear()
	{
		destroy(root);
		root->children.clear();
	}
	~Trie()
	{
		clear();
		delete root;
	}
};

void Trie::insert(const string &s)
{
	node *p = root;
	for (int i = 0; i != s.size(); ++i)
	{
		if (p->children.find(s[i]) == p->children.end())
		{//如果该节点的孩子列表不存在该字符，则创建一个节点，该字符为键值
			p->children.emplace(s[i], new node());
		}
		p = p->children[s[i]];
	}
	p->count++; //该单词出现次数加1
}

void Trie::create(const vector<string> &ss)
{
	for (int i = 0; i != ss.size(); ++i)
		insert(ss[i]);
}

int Trie::search(const string &s)const
{
	node *p = root;
	for (int i = 0; i != s.size(); ++i)
	{
		//孩子列表没有改字符，则说明该单词不存在
		if (p->children.find(s[i]) == p->children.end())
			return 0;
		p = p->children[s[i]];
	}
	return p->count;
}

void Trie::destroy(node *r)
{
	if (r->children.empty())
	{
		delete r;
		return;
	}
	for (auto iter = r->children.begin(); iter != r->children.end();
		++iter)
		destroy(iter->second); //先删除孩子
	if (r != root)
		delete r; //再删除自己
}

vector<pair<string, int>> Trie::countingFrequency(int wordcnt, int word_avg_len)const
{
	vector<pair<string, int>> countings;
	if (root->children.empty())
		return move(countings);
	countings.reserve(wordcnt);
	string prefix;
	prefix.reserve(word_avg_len);
	countingFrequency(root, prefix, countings);
	return move(countings);
}

void Trie::countingFrequency(node *r, string &prefix,
	vector<pair<string, int>> &countings)const
{
	if (r->children.empty() || r->count > 0)
	{//1、为叶子节点；2、到目前为止，prefix所构成的单词出现过
		countings.emplace_back(make_pair(prefix, r->count));
		if (r->children.empty())
			return;
	}
	for (auto iter = r->children.begin(); iter != r->children.end(); ++iter)
	{
		prefix.push_back(iter->first);
		countingFrequency(iter->second, prefix, countings);
		prefix.pop_back();
	}
}

#endif