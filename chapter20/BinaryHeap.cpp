
/*******************************************
* Author: bravepam
* E-mail:1372120340@qq.com
* Blog:http://blog.csdn.net/zilingxiyue
*******************************************
*/

#include<iostream>
#include<vector>
#include<functional>

using namespace std;

//#define MINIMUM_HEAP_H

#ifdef MINIMUM_HEAP_H	//��������˸�ֵ
const static int EXTREAM = 0x7fffffff;//�����ʵ�ֵ�����С��
#else
const static int EXTREAM = 0x80000000;
#endif

template <typename Key, typename Handle = void*>
struct node
{//�ѽ��
	Key key;//�ѹؼ��֣�ͨ����Ӧ�ö��������Ȩ
	Handle obj_handle;//��ùؼ��ֹ����ľ����ָ���ⲿӦ�ö��󣬿�����ָ��,���ɱ�
	node(const Key &k, const Handle &h_) :key(k), obj_handle(h_){}
	node() :key(), obj_handle(){}
	Handle getHandle()const { return obj_handle; }
};

template <typename Handle>
struct doNothing :public binary_function < Handle, int, void >
{//Ĭ�ϵ������ڶ����޸��ⲿ����ĺ���������
	void operator()(const Handle &hd, int h)
	{//�����κ���
		return;
	}
};

template <typename Key,typename Handle = void*,class EditObjIndex = doNothing<Handle>,class CompareKey = greater<Key>>
class heap//���Ͳ����ֱ�Ϊ�ؼ������ͣ�������ͣ��༭�ѽ��������Ӧ���ⲿ����ľ�����Ƚ���
{//����ѣ�Ĭ������
private:
	typedef node<Key, Handle>				node;
	vector<node> h;
	int size;//�Ѵ�С
	EditObjIndex editIndex;
	CompareKey compare;
public:
	//���ֲ�ͬ�Ĺ��캯��
	heap() :h(), editIndex(), compare(),size(0){}
	heap(const EditObjIndex &eh) :editIndex(eh), compare(), h(), size(0){}
	heap(const CompareKey &comp) :compare(comp), editIndex(), h(),size(0){}
	heap(const EditObjIndex &eh, const CompareKey &comp) :editIndex(eh), compare(comp), h(), size(0){}
	bool empty()const { return size == 0; }
	size_t length()const { return size; }
	Key maximum()const { return h[0].key; }//�������С�ѣ��򷵻ص�����С�ؼ���
	void insert(const Key&,const Handle& = Handle());
	Key extractMax();
	void increaseKey(int, const Key&);
};

template <typename K,typename H,class EOH,class CK>
K heap<K, H, EOH, CK>::extractMax()
{//��ȡ���ֵ����������
	K k = h[0].key;
	h[0] = h[--size];//�����һ��Ԫ�طŵ��Ѹ���
	editIndex(h[size].obj_handle, 0);//�޸ĸ�Ԫ�ع������ⲿ�����ڶ��еľ��
	int left = 1, right = 2, i = 0, largest = i;
	while (left < size)
	{//�������¿�ʼ������
		if (left < size && compare(h[left].key, h[largest].key))
			largest = left;
		if (right < size && compare(h[right].key, h[largest].key))
			largest = right;//ȷ�����ֵ
		if (largest == i) break;//�������������˳������ٵ���
		else 
		{//����
			std::swap(h[largest], h[i]);//������Ԫ��
			//���޸�������Ԫ�ع������ⲿ����Ķ��о��
			editIndex(h[largest].obj_handle, largest);
			editIndex(h[i].obj_handle, i);
		}
		left = 2 * largest + 1, right = 2 * largest + 2,i = largest;//�޸ģ��Լ�������
	}
	return k;//�������ֵ
}

template <typename K,typename H,class EOH,class CK>
void heap<K, H, EOH, CK>::increaseKey(int index, const K &k)
{//����ָ��Ԫ�صĹؼ���ֵ���������С�ѣ��ú�������ʾ��С�ؼ���ֵ
	if (!compare(k, h[index].key))
	{//���¹ؼ��ֲ��ʺ�
		#ifdef MINIMUM_HEAP_H
			cout << "Error:greater key" << endl;
		#else
			cout << "Error:less key" << endl;
		#endif
		return;
	}
	h[index].key = k;
	int parent = (index - 1) / 2;
	while (parent >= 0 && compare(k,h[parent].key))//���ȸ��ڵ��
	{//���¶��ϵ�����
		std::swap(h[index], h[parent]);
		editIndex(h[parent].obj_handle, parent);
		editIndex(h[index].obj_handle, index);
		if (parent == 0) break;//���ѵ�����
		else
		{//�����������
			index = parent;
			parent = (index - 1) / 2;
		}
	}
}

template <typename K,typename H,class EOH,class CK>
void heap<K, H, EOH, CK>::insert(const K &k,const H &hd = H())
{//����Ԫ�أ������ֱ�Ϊ�ⲿ�����Ȩֵ��Ψһ�Ա�ʶ
	if (size == h.size()) h.resize(2 * size + 1);//���vector����
	++size;
	editIndex(hd, size - 1);
	if (size == 1)//���ǵ�һ��Ԫ��
		h[size - 1] = node(k, hd);
	else
	{//����
		h[size - 1] = node(EXTREAM, hd);//����
		increaseKey(size - 1, k);
	}
}

//----------------------------------------����---------------------------------------

template <typename Value>
struct object
{//���Զ���
	int node_index;//�ڶ��еĵ�����
	int priority;//����Ȩ
	Value v;//ֵ
	object(int pri, const Value& vv) :node_index(0), v(vv), priority(pri){}//��ʼ������Ĭ��Ϊ0
};

template <typename Value>
struct editObjIdx :public binary_function < object<Value>*, int , void >
{//�Զ�����޸Ķ���Ķ���������������
	void operator()(object<Value> *p, int hh)
	{
		p->node_index = hh;
	}
};

int main()
{
	heap<int, object<int>*, editObjIdx<int>> h;
	vector<object<int>*> vec;//�洢��Щobj
	for (int i = 0; i <= 10; i += 2)
	{//obj��ȨֵΪ2 * i��ֵΪi * i
		object<int> *p = new object<int>(2 * i, i * i);
		vec.push_back(p);
		h.insert(2 * i, p);
	}
	for (int i = 1; i <= 10; i += 2)
	{
		object<int> *p = new object<int>(2 * i, i * i);
		vec.push_back(p);
		h.insert(2 * i, p);
	}
	/*while (!h.empty())
		cout << h.extractMax() << endl;
	cout << endl;*/
	for (size_t i = 0; i <= h.length(); i += 3)
	{//����ĳЩ�����Ȩֵ
		vec[i]->priority += 100;
		h.increaseKey(vec[i]->node_index, vec[i]->priority);
	}
	while (!h.empty())
		cout << h.extractMax() << endl;
	for (int i = 0; i != vec.size(); ++i)
		delete vec[i];

	/*heap<int> h;//ֻ�йؼ��ֵĶѣ���ʱHandle��EditObjIndex�Ⱦ�ΪĬ��ֵ��û����ν�ľ��
	for (int i = 0; i != 10; ++i)
		h.insert(i);
	while (!h.empty())
		cout << h.extractMax() << endl;*/
	getchar();
	return 0;
}