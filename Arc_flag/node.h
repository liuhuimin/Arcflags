#define INF 0x3FFFFFFF
#define NEVER    0
#define EVER     1
#define FINISHED 2
#include<vector>
//��Ҫ��#defineΪ��·�����Ǻ�ת��Լ����Ƕ����ǩ
#pragma  once
using namespace std;

struct center
{
	double x;
	double y;
};

struct degree
{
	int shapeid;
	int degrees;
	bool operator< (degree a)
	{
		if (degrees > a.degrees)
			return true;
		else return false;
	}
};

struct edge//��
{
	int id_to;//ָ��Ľ��
	//zsy change int to double
	int weight;//��Ȩֵ
	double max_speed;//���ͨ���ٶ�
	Bit sign;
};

struct vertex
{
	//20150412 zsy add index
	//20150412 zsy change int to double (x,y)
	int shapeid;
	//int index;
	vector<edge> edges;//�ڱ�
	int x;//x����
	int y;//y����
	int turn_restr;//ת��Լ�����
};

struct obstacle//������ϰ�
{
	vector<double> x;//����ζ����x����
	vector<double> y;//����ζ����y����
};
inline int exRand()
{
	return rand()*RAND_MAX + rand();
}