
int Dijkstra(Graph & graph, int s, int t, int *label, int *path, vector<pair<int, int>> &search)
{
	int *flag = new int[graph.vertices.size()];//搜索状态的标记
	memset(flag, 0, sizeof(int)*graph.vertices.size());

	Priority_Queue Q(graph.vertices.size() / 100, label, graph.vertices.size());

	int id = s;
	path[s] = s;
	label[s] = 0;

	//flag[s]=FINISHED;

	while (id != t)//忽略了没有路径的情况
	{
		for (int i = 0; i<graph.vertices[id].edges.size(); i++)
		{
			int id_to = graph.vertices[id].edges[i].id_to;
			switch (flag[id_to])
			{
			case NEVER://从未搜索过
				label[id_to] = label[id] + graph.vertices[id].edges[i].weight;
				Q.Push(id_to);
				flag[id_to] = EVER;
				path[id_to] = id;
				break;
			case EVER://曾经搜索过
				if (label[id] + graph.vertices[id].edges[i].weight < label[id_to])
				{
					label[id_to] = label[id] + graph.vertices[id].edges[i].weight;
					Q.Adjust(id_to);
					path[id_to] = id;
				}
				break;
			}
		}
		flag[id] = FINISHED;
		id = Q.Top();
		Q.Pop();
		search.push_back(pair<int, int>(path[id], id));//保存搜索节点，是在这里还是在switch中使得内容更精细？
	}
	delete(flag);
	return label[t];
}

void Dijkstra_ALL(Graph & graph,int s, int *label, int *path)
{
	int *flag = new int[graph.vertices.size()];//搜索状态的标记
	memset(flag, 0, sizeof(int)*graph.vertices.size());

	Priority_Queue Q(graph.vertices.size() / 100, label, graph.vertices.size());

	int id = s;
	path[s] = s;
	label[s] = 0;

	//flag[s]=FINISHED;
	Q.Push(id);

	while (!Q.isEmpty())//忽略了没有路径的情况
	{
		id = Q.Top();
		Q.Pop();
		for (int i = 0; i<graph.vertices[id].edges.size(); i++)
		{
			int id_to = graph.vertices[id].edges[i].id_to;
			switch (flag[id_to])
			{
			case NEVER://从未搜索过
				label[id_to] = label[id] + graph.vertices[id].edges[i].weight;
				Q.Push(id_to);
				flag[id_to] = EVER;
				path[id_to] = id;
				break;
			case EVER://曾经搜索过
				if (label[id] + graph.vertices[id].edges[i].weight < label[id_to])
				{
					label[id_to] = label[id] + graph.vertices[id].edges[i].weight;
					Q.Adjust(id_to);
					path[id_to] = id;
				}
				break;
			}
		}
		flag[id] = FINISHED;

	}
	delete(flag);

}