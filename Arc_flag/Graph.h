/************
#pragma   once
************/
#pragma  once

#include <vector>
#include <queue>
#include <limits>
#include<hash_map>
#include<omp.h>
//#include <algorithm>

using namespace std;
class Graph
{
private://私有成员自行添加，不要删除原有内容。

	vector<vertex> vertices_r;//反向图
	obstacle obstacles;//障碍集合
	vector<vector<vector<int>>> Grids;
	//坐标的四个边界
	//double bound_west;
	//double bound_east;
	//double bound_north;
	//double bound_south;



	bool isDist;//边权值是否为距离
	int global_max_speed;//全局最大通行速度
	//------zsy add 20150416 tiger only
	double global_max_weight;//最大边权值
	//--------------------------------

	int FindNearst(int x, int y);//给定坐标，查找一个可能比较近的结点。失败返回-1
	//-------zsy add 20150413--------
	//void LoadDb(DBFHandle handle);
	
	void UpdateWeightBound(edge);//更新权值上界
	//-------------------------------
	void Swap(int a, int b);
	void RandomNodes();//节点随机化

public:
	double bound_west;
	double bound_east;
	double bound_north;
	double bound_south;
	void UpdateBound(vertex);//更新边界
	vector<vertex> vertices;//图的结点集，从1开始，0处为无效值
	vector<int> moveobj;//移动对象
	vector<int> classes;//分块类别信息
	vector<degree> Degrees;//节点度排序
	int part;//分块个数
	vector<int> vertexcut;//边割节点


	void init();

	int grid_size;//网格边长
	void Reverse();//反转

	void GridsGen(int size);//划分为边长为size的网格
	void Convert();//消除约束
	

	

	int DFS(int, int *);//DFS搜索，返回连通点数量
	
	int Astar_aggressive(int s, int t, int *label, int *path);//估值系数不为1的A*
	int Bi_direct_Astar(int s, int t, int *label, int *path);//双向A*
	int Bi_queue_Astar(int s, int t, int *label, int *path);//双队列A*
	int Middle_vertices_search(int s, int t, int *label, int *path);//中间结点启动的Dijkstra
	//===========zsy add 2050412==============
	int Middle_vertices_search2(int s, int t, int *label, int *path);
	void ResetGraph();//重置图中所有数据
	void ExportConnectedGraph(int start, const char * grfile);
	//========================================
};



inline double get_dist(double a, double b)
{
	return sqrt(a*a + b*b);
}





void Graph::GridsGen(int size)//划分为边长为(bound_east-bound_west)/size+1的网格
{
	grid_size = (bound_east - bound_west) / size + 1;
	int w = abs(bound_east - bound_west) / grid_size + 1;
	int h = abs(bound_north - bound_south) / grid_size + 1;
	Grids.resize(h);
	classes.resize(vertices.size());
	for (int i = 0; i<Grids.size(); i++)
		Grids[i].resize(w);
	for (int i = 1; i<vertices.size(); i++)
	{
		int x = (vertices[i].x - bound_west) / grid_size;
		int y = (vertices[i].y - bound_south) / grid_size;
		classes[i] = y*w + x;
		Grids[y][x].push_back(i);
	}

	int c = 0;
	for (int i = 0; i<h; i++)
	for (int j = 0; j<w; j++)
	if (Grids[i][j].size()>0)
		c++;
	printf("%d grids is not empty\n", c);
}

int Graph::FindNearst(int x, int y)//给定坐标，查找一个可能比较近的结点。失败返回-1
{
	int grid_x = (x - bound_west) / grid_size;
	int grid_y = (y - bound_south) / grid_size;
	int min = INF;
	int min_v = -1;
	for (int i = 0; i<Grids[grid_y][grid_x].size(); i++)
	{
		int dist = sqrt((vertices[Grids[grid_y][grid_x][i]].x - x)*(vertices[Grids[grid_y][grid_x][i]].x - x)
			+ (vertices[Grids[grid_y][grid_x][i]].y - y)*(vertices[Grids[grid_y][grid_x][i]].y - y));
		if (dist<min)
		{
			min = dist;
			min_v = Grids[grid_y][grid_x][i];
		}
	}
	return min_v;
}


//void Graph::LoadPoint(SHPHandle handle,int pnEntities, 
//	double* padfMinBound, double* padfMaxBound)
//{
//	std::cout<<"loading point:\n";
//	std::cout<<"resizing vector:\n";
//	vertices.resize(pnEntities);
//	for(int i = 0;i < pnEntities;i++)
//	{
//		std::cout<<i + 1<<"/"<<pnEntities<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
//		SHPObject *o = SHPReadObject(handle,i);
//		
//		    vertex v;
//			v.x = o->padfX[0];
//			v.y = o->padfY[0];
//			//v.index = vertices.size();
//			v.shapeid = o->nShapeId;
//			vertices.push_back(v);
//		
//		SHPDestroyObject(o);
//	}
//	std::cout<<std::endl;
//}
//void Graph::LoadPolyLine(SHPHandle handle,int pnEntities,
//	double* padfMinBound, double* padfMaxBound)
//{
//	std::cout<<"loading polyline:\n";
//	for(int i = 0;i < pnEntities;i++)
//	{
//		std::cout<<i + 1<<"/"<<pnEntities<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
//		SHPObject *o = SHPReadObject(handle,i);
//		//polyline only
//		if(o->nVertices == 1)
//		{		
//			std::cout<<"polyline with single point"<<std::endl;
//		    /*vertex v;
//			v.x = o->padfX[0];
//			v.y = o->padfY[0];
//			v.index = vertices.size();
//			v.shapeid = o->nShapeId;
//			vertices.push_back(v);*/
//		}else
//		{
//			vertex v1,v2;
//			edge e;
//			v1 = vertices[o->panPartStart[0]];
//			v2 = vertices[o->panPartStart[o->nParts - 1]];
//			//TODO: how to find the end point
//			e.id_to = v2.shapeid;
//			e.weight = 0;
//			for(int i = 1;i < o->nVertices;i++)
//			{
//				e.weight += get_dist(o->padfX[i - 1] - o->padfX[i],o->padfY[i - 1] - o->padfY[i]);
//			}
//			v1.edges.push_back(e);
//		}
//		SHPDestroyObject(o);
//	}
//	std::cout<<std::endl;
//}
void Graph::UpdateBound(vertex v)
{
	if (v.x < bound_west)
		bound_west = v.x;
	else if (v.x > bound_east)
		bound_east = v.x;
	if (v.y < bound_south)
		bound_south = v.y;
	else if (v.y > bound_north)
		bound_north = v.y;
}
void Graph::UpdateWeightBound(edge e)
{
	if (e.max_speed > global_max_speed)
		global_max_speed = e.max_speed;
	if (e.weight > global_max_weight)
		global_max_weight = e.weight;
}
void Graph::ResetGraph()
{
	vertices.clear();
	vertices_r.clear();;//反向图
	obstacles.x.clear();//障碍集合
	obstacles.y.clear();

	//坐标的四个边界
	double bound_west = (numeric_limits<double>::min)();
	double bound_east = (numeric_limits<double>::max)();
	double bound_north = (numeric_limits<double>::max)();
	double bound_south = (numeric_limits<double>::min)();

	bool isDist = true;//边权值是否为距离
	int global_max_speed = 0;//全局最大通行速度
	//------zsy add 20150416 tiger only
	double global_max_weight = 0;//最大边权值
}

int Graph::DFS(int start, int * flag)
{
	//int *flag=new int[this->vertices.size()];//搜索状态的标记
	memset(flag, 0, sizeof(int)*this->vertices.size());
	int count = 0;
	queue<int> v;
	v.push(start);
	int id = start;
	while (id > 0)//
	{
		for (int i = 0; i < vertices[id].edges.size(); i++)
		{
			int id_to = vertices[id].edges[i].id_to;
			if (flag[id_to] == NEVER)
			{
				v.push(id_to);
				count++;
				flag[id_to] = EVER;
			}
		}
		flag[id] = FINISHED;
		if (v.empty())
			id = -1;
		else
		{
			id = v.front();
			v.pop();
		}
	}
	//delete(flag);
	return count;
}
int Euclidean_Dist(Graph &graph, int a, int b)
{
	return (int)sqrt((double)(graph.vertices[a].x - graph.vertices[b].x)*(double)(graph.vertices[a].x - graph.vertices[b].x) +
		(double)(graph.vertices[a].y - graph.vertices[b].y)*(double)(graph.vertices[a].y - graph.vertices[b].y));
}