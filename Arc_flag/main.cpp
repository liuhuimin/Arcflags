#include<iostream>
#include<string>
#include<time.h>
#include <stdlib.h>
#include <Windows.h>
#include<assert.h>
#include<vector>
#include"Bit.h"
#include"node.h"
#include"Priority_Queue.h"
#include"Graph.h"
#include"Arc_flag.h"
#include"ReadData.h"
#include"Dijkstra.h"
#include"Astar.h"
#include"WriteData.h"
using namespace std;

void time_test(string name,Graph & graph, int s, int t,int(*p)(Graph & graph, int s, int t, int *label, int *path, vector<pair<int, int>> &search))
{
	LARGE_INTEGER begin, end, lv;
	double secondsPerTick;
	QueryPerformanceFrequency(&lv);
	secondsPerTick = 1000000.0 / lv.QuadPart;
	double precom;
	int *label = new int[graph.vertices.size()];
	int *path = new int[graph.vertices.size()];
	vector<pair<int, int>> search;
	QueryPerformanceCounter(&begin);
	int dist = p(graph, s, t, label, path, search);
	QueryPerformanceCounter(&end);
	precom = secondsPerTick * (end.QuadPart - begin.QuadPart);
	cout << name;
	printf("cost time test:%fus\n", precom);
	cout << name;
	cout << "distance" << dist << endl;
}

void main()
{
	LARGE_INTEGER begin, end, lv;
	double secondsPerTick;
	QueryPerformanceFrequency(&lv);
	secondsPerTick = 1000000.0 / lv.QuadPart;

	
	vector<pair<int, int>> search;
	vector<pair<int, int>> search1;
	double precom1 = 0;

	//读取数据文件盒分块信息
	QueryPerformanceCounter(&begin);
	Graph graph;
	LoadGraph_gr(graph, "E:\\gis\\USA-road-d.Ny.gr", "E:\\gis\\USA-road-d.ny.co");
	LoadClass(graph, "E:\\gis\\ny_metis.graph.part.16");
	QueryPerformanceCounter(&end);
	precom1 = secondsPerTick * (end.QuadPart - begin.QuadPart);
	printf("load all data cost time:%fus\n", precom1);

	//修正数据
	int count = 0;
	for (int i = 1; i<graph.vertices.size(); i++)
		for (int j = 0; j<graph.vertices[i].edges.size(); j++)
			if (Euclidean_Dist(graph,i, graph.vertices[i].edges[j].id_to)>graph.vertices[i].edges[j].weight)
			{
				//printf("Wrong graph data %d %d\n", graph.Euclidean_Dist(i, graph.vertices[i].edges[j].id_to), graph.vertices[i].edges[j].weight);
				graph.vertices[i].edges[j].weight = Euclidean_Dist(graph,i, graph.vertices[i].edges[j].id_to);
				//printf("a.x:%d a.y:%d b.x:%d b.y:%d weight:%d\n", graph.vertices[i].x, graph.vertices[i].y, graph.vertices[graph.vertices[i].edges[j].id_to].x, graph.vertices[graph.vertices[i].edges[j].id_to].y, graph.vertices[i].edges[j].weight);
				count++;
			}


	cout << "Wrong graph data " << count << endl;

	srand(time(0));
	double dijktime=0, astartime=0, flagdtime=0,flagatime=0,preflag=0;
	
	for (int i = 0; i < 3; i++)//使用Arc_flag算法预处理图数据并统计时间
	{
		QueryPerformanceCounter(&begin);
		Arc_flag_sign(graph);
		QueryPerformanceCounter(&end);
		precom1 = secondsPerTick * (end.QuadPart - begin.QuadPart);
		preflag += precom1;
		
	}
	printf("precompute cost time:%fus\n", preflag/3);
	int N = 1000;
	
	for (int i = 0; i < N; i++)
	{
		int *label = new int[graph.vertices.size()];
		int *path = new int[graph.vertices.size()];

		//cout << "s x:" << graph.vertices[s].x << "s y:" << graph.vertices[s].y << "t x:" << graph.vertices[t].x << "t y:" << graph.vertices[t].y << endl;

		int s = (exRand() % (graph.vertices.size() - 1) + 1);
		int t = (exRand() % (graph.vertices.size() - 1) + 1);
		//s = 79765;
		//t = 144179;
		//cout << "s:" << s << "t:" << t << endl;
		//cout << "s class:" << graph.classes[s] << " t:" << graph.classes[t] << endl;
		//使用dijkstra算法计算路径并统计时间
		QueryPerformanceCounter(&begin);
		int dist = Dijkstra(graph, s, t, label, path, search);
		QueryPerformanceCounter(&end);
		precom1 = secondsPerTick * (end.QuadPart - begin.QuadPart);
		//cout << "d" << endl;
		dijktime += precom1;
		//printf("dijkstra cost time test:%fus\n", precom1);
		//cout << "dijkstra distance" << dist << endl;
		//写入算法扩展节点和路径，其中WriteSearch要在原始数据上叠加
		//WriteSearch("D:\\usa-road-ny_dijkstra.dat", graph, search);

		//WriteRoad("D:\\dijkstra_path", graph, path, s, t);

		//cout << "write path finished" << endl;

		//使用Astar算法计算路径并统计时间
		int *label2 = new int[graph.vertices.size()];
		int *path2 = new int[graph.vertices.size()];
		int dist2 = 0;
		QueryPerformanceCounter(&begin);
		dist2 = Astar(graph, s, t, label2, path2);
		QueryPerformanceCounter(&end);
		precom1 = secondsPerTick * (end.QuadPart - begin.QuadPart);
		astartime += precom1;
		//printf("astar cost time test:%fus\n", precom1);
		//cout << "a" << endl;
		//cout << "astar distance" << dist2 << endl;

		//使用Arc_flag算法预处理图数据并统计时间
		


		//使用Arc_flag算法计算最短路径
		QueryPerformanceCounter(&begin);
		int *label1 = new int[graph.vertices.size()];
		int *path1 = new int[graph.vertices.size()];
		int dist1 = Arc_flag_query(graph, s, t, label1, path1, search1);
		QueryPerformanceCounter(&end);
		precom1 = secondsPerTick * (end.QuadPart - begin.QuadPart);
		flagdtime += precom1;
		//cout << "fd" << endl;
		//printf("arcflags query cost time test:%fus\n", precom1);

		//WriteSearch("D:\\usa-road-ny_arcflag.dat", graph, search1);

		//cout << "dist:" << dist << "  dist1:" << dist1 << endl;

		QueryPerformanceCounter(&begin);
		int *label3 = new int[graph.vertices.size()];
		int *path3 = new int[graph.vertices.size()];
		int dist3 = Arc_flag_query_Astar(graph, s, t, label3, path3);
		QueryPerformanceCounter(&end);
		precom1 = secondsPerTick * (end.QuadPart - begin.QuadPart);
		flagatime += precom1;
		//cout << "s class:" << graph.classes[s] << " t:" << graph.classes[t] << endl;
		//printf("arcflags query astar cost time test:%fus\n", precom1);
		cout << "dijkstra dist:" << dist <<" a* dist:"<<dist2<< " flags dijk:" << dist1 << " flag a*:" << dist3 << endl;
		int route = path[t];
		int route1 = path1[t];

		delete[] label;
		delete[] label1;
		delete[] label2;
		delete[] label3;
		delete[] path;
		delete[] path1;
		delete[] path2;
		delete[] path3;
		/*//对比路径
		while (route==route1&&route != s)
		{
		route = path[route];
		route1 = path1[route1];
		cout << "dijkstra" << route << " arcflag" << route1 << endl;
		}
		cout << endl << "false" << endl;
		while (route != s)
		{
		route = path[route];
		route1 = path1[route1];
		cout << "dijkstra" << route << " arcflag" << route1 << endl;
		}
		while (route != s)
		{
		route = path[route];
		cout << "dijkstra" << route << endl;
		}
		while (route1 != s)
		{
		route1 = path1[route1];
		cout<<"arcflag"<< route1 << endl;
		}*/

		//cout << "dijkstra dist" << dist << " arc_flag dist" << dist1 << endl;
	}
	printf("Dijkstra cost time:%fus\n", dijktime / N);
	printf("A* cost time:%fus\n", astartime / N);
	printf("flag dijkstra cost time:%fus\n", flagdtime / N);
	printf("flag A* cost time:%fus\n", flagatime / N);
	printf("precompute cost time:%fus\n", preflag / 3);
	system("pause");
}