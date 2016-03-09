void Arc_flag(Graph & graph, int s, int *label, int *path)
{
	
	int classes = graph.classes[s];
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
		
		for (int i = 0; i < graph.vertices[id].edges.size(); i++)
		{
			if (graph.vertices[id].edges[i].id_to == path[id])//&&graph.classes[id]!=classes)//找到路径的上一节点，标记该边
			{
				graph.vertices[id].edges[i].sign.setBit(classes,true);
				break;
			}
		}
		
		
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



void Arc_flag_sign(Graph & graph)
{
	int size = graph.vertexcut.size();
	for (int i = 0; i < size; i++)
	{
		
		int *label = new int[graph.vertices.size()];
		int *path = new int[graph.vertices.size()];
		Arc_flag(graph, graph.vertexcut[i], label, path);
		delete[] label;
		delete[] path;
		
	}
	/*
	int count=0;
	int size = graph.vertices.size();
	for (int i = 1; i < size; i++)
	{
		for (int j = 0; j < graph.vertices[i].edges.size(); j++)
		{
			if (graph.classes[i] != graph.classes[graph.vertices[i].edges[j].id_to])
			{
				int *label = new int[graph.vertices.size()];
				int *path = new int[graph.vertices.size()];
				Arc_flag(graph, i, label, path);
				count++;
				delete[]label;
				delete[]path;
				break;
			}
		}
	}
	cout<<"vertex in edgecut is "<<count<<end;
	*/
}
 
int Arc_flag_query(Graph & graph, int s, int t, int *label, int *path,vector<pair<int,int>> &search)
{
	int *flag = new int[graph.vertices.size()];//搜索状态的标记
	memset(flag, 0, sizeof(int)*graph.vertices.size());

	Priority_Queue Q(graph.vertices.size() / 100, label, graph.vertices.size());

	int id = s;
	path[s] = s;
	label[s] = 0;
	int classes = graph.classes[t];
	//flag[s]=FINISHED;

	while (id != t)//忽略了没有路径的情况
	{
		for (int i = 0; i<graph.vertices[id].edges.size(); i++)
		{
			if (!graph.vertices[id].edges[i].sign.getBit(classes))
				continue;
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
		//cout << id << endl;
		//if (id == -1)
		//	break;
		//search.push_back(pair<int, int>(path[id], id));//保存搜索节点，是在这里还是在switch中使得内容更精细？
	}
	delete(flag);
	return label[t];
}

int Arc_flag_query_Astar(Graph &graph, int s, int t, int *label, int *path)//A*
{
	int *exlabel = new int[graph.vertices.size()];//label+估值
	int *estimated_dist = new int[graph.vertices.size()];//估值
	int *flag = new int[graph.vertices.size()];//搜索状态的标记
	memset(flag, 0, sizeof(int)*graph.vertices.size());

	Priority_Queue Q(graph.vertices.size() / 100, exlabel, graph.vertices.size());

	int id = s;
	path[s] = s;
	label[s] = 0;
	int classes = graph.classes[t];
	//flag[s]=FINISHED;

	while (id != t)//忽略了没有路径的情况
	{
		/*if (id == 27153)
		{
		cout << id << endl;
		cout << vertices[id].edges.size() << endl;
		}*/
		for (int i = 0; i<graph.vertices[id].edges.size(); i++)
		{
			if (!graph.vertices[id].edges[i].sign.getBit(classes))
				continue;
			int id_to = graph.vertices[id].edges[i].id_to;
			switch (flag[id_to])
			{
			case NEVER://从未搜索过
				label[id_to] = label[id] + graph.vertices[id].edges[i].weight;
				estimated_dist[id_to] = Euclidean_Dist(graph, id_to, t);
				exlabel[id_to] = label[id_to] + estimated_dist[id_to];
				Q.Push(id_to);
				flag[id_to] = EVER;
				path[id_to] = id;
				break;
			case EVER://曾经搜索过
				if (label[id] + graph.vertices[id].edges[i].weight + estimated_dist[id_to] < exlabel[id_to])
				{
					label[id_to] = label[id] + graph.vertices[id].edges[i].weight;
					exlabel[id_to] = label[id_to] + estimated_dist[id_to];
					Q.Adjust(id_to);
					path[id_to] = id;
				}
				break;
			}
		}
		//if(Q.Top() < 0)
		//{
		//    cout<<id<<endl;
		//	cout<<vertices[id].edges.size()<<endl;
		//}
		flag[id] = FINISHED;
		id = Q.Top();
		Q.Pop();

	}

	//this->Visualize(s,t,flag,path);

	delete(flag);
	delete(exlabel);
	delete(estimated_dist);
	return label[t];
}