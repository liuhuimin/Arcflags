/*****************
ʹ�ö��Ͱ�ṹʵ��
�ο����ף�
https://www.cs.princeton.edu/courses/archive/fall03/cs528/handouts/a%20practical%20shortest.pdf
*****************/
#include<malloc.h>
#include<string.h>
#define HeapLeftChild(i) ((i * 2) + 1)
#define HeapParent(i) ((i - 1) / 2)
#define HeapRightFromLeft(i) (i + 1)


class Priority_Queue
{

public:
	Priority_Queue(int init_capacity,//��ʼ�ռ��С 
		int *label,//�����ж����ȼ���Ȩֵ,ע�������ȼ��������в��ɸ�д���������ݣ�
		int n_vertices//��Ӧ��ͼ�Ľ����
		);
	~Priority_Queue();//�ͷſռ�

	void Push(int vertex);//����һ�����
	void Adjust(int vertex);//����Ѵ��ڣ��������λ�á�
	void Pop();//�������ȼ���ߵĽ��
	int Top();//�������ȼ���߽���ֵ
	bool isEmpty();//�����Ƿ�Ϊ��
	int Size();//���ض���Ԫ������

	class Iterator
	{
	private:
		Priority_Queue &Queue;//���ȼ����е�ʵ�����ݣ���Ҫ�������ȼ��������ݵ�ʵ�ʴ洢��ʽ������
		int index;
	public:
		Iterator(Priority_Queue &Q, int init_index) : Queue(Q), index(init_index){}

		void operator++()
		{
			index++;
		}
		int operator*()
		{
			return Queue._heap[index];
		}
		bool operator !=(Iterator iter)
		{
			return iter.index != this->index;
		}
	};
	Iterator begin()
	{
		return Iterator(*this, 0);
	}
	Iterator end()
	{
		return Iterator(*this, _count);
	}

private://˽�г�Ա���ж���
	int _count;
	int _length;
	int *_heap;
	static const int DefaultCapacity = 100;
	int *label;
	int *hash;
};

Priority_Queue::Priority_Queue(int init_capacity, int *label, int n_vertices)
{
	this->_length = (init_capacity > 0) ? init_capacity : DefaultCapacity;
	this->_heap = new int[this->_length];
	this->_count = 0;
	this->label = label;
	this->hash = new int[n_vertices];
	for (int i = 0; i<n_vertices; i++)//���ܲ���Ҫ���
		hash[i] = -1;

}

Priority_Queue::~Priority_Queue()
{
	delete _heap;
	delete hash;
}

void Priority_Queue::Push(int vertex)
{
	int index, i;

	if (this->_count == this->_length)
	{
		int *localArray = new int[this->_count * 2];
		/*for (int j = 0; j < this->_count; j++)
		{
		localArray[j] = this->_heap[j];
		}*/
		memcpy(localArray, this->_heap, sizeof(int)*this->_count);
		delete this->_heap;
		this->_heap = localArray;
		this->_length *= 2;
	}

	i = this->_count;
	while (i > 0)
	{
		index = HeapParent(i);
		if (label[vertex] >= label[this->_heap[index]])
		{
			break;
		}
		this->_heap[i] = this->_heap[index];
		hash[this->_heap[i]] = i;
		i = index;
	}
	this->_heap[i] = vertex;
	hash[this->_heap[i]] = i;
	this->_count++;
}

void Priority_Queue::Adjust(int vertex)
{
	int i = hash[vertex];
	while (i > 0)
	{
		int p = HeapParent(i);
		if (label[vertex] >= label[this->_heap[p]])
		{
			break;
		}
		this->_heap[i] = this->_heap[p];
		hash[this->_heap[i]] = i;
		i = p;
	}
	this->_heap[i] = vertex;
	hash[this->_heap[i]] = i;
}

void Priority_Queue::Pop()
{
	int z;
	//hash[this->Top] = -1;
	if (this->_count > 0)
	{
		int i = 0;
		for (int j = HeapLeftChild(i); j < this->_count; j = HeapLeftChild(i))
		{
			int index = HeapRightFromLeft(j);
			int num4 = ((index < this->_count) && (label[this->_heap[index]]<label[this->_heap[j]])) ? index : j;
			if (label[this->_heap[this->_count - 1]] <= label[this->_heap[num4]])
				break;
			this->_heap[i] = this->_heap[num4];
			hash[this->_heap[i]] = i;
			i = num4;
		}
		this->_heap[i] = this->_heap[this->_count - 1];
		hash[this->_heap[i]] = i;
	}
	this->_count--;
}

int Priority_Queue::Top()
{
	if (this->_count>0)
		return this->_heap[0];
	else
	{
		//printf("PriorityQueue is empty.\n");
		return -1;
	}
}

bool Priority_Queue::isEmpty()
{
	return this->_count <= 0;
}

int Priority_Queue::Size()
{
	return this->_count;
}