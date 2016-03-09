void LoadGraph_gr(Graph &graph,char *file_gr, char *file_co)//读入图数据
{
	int *maxdegree;
	double sum_arc = 0;
	FILE *fp = fopen(file_gr, "r");
	if (!fp)
	{
		std::printf("Error: cannot open the file: %s\n", file_gr);
		return;
	}
	char buf[200] = { 0 };
	int size = 0, n_arc = 0;
	//skip some information
	fgets(buf, 200, fp);
	fgets(buf, 200, fp);
	fgets(buf, 200, fp);
	fgets(buf, 200, fp);
	fscanf(fp, "p sp %d%d\n", &size, &n_arc);
	fgets(buf, 200, fp);
	fgets(buf, 200, fp);
	std::printf("This graph has %d vertices\n", size);
	std::printf("%d edges\n", n_arc);
	size++;
	//nodes=(vertex*)malloc(sizeof(vertex)*(size));
	graph.vertices.resize(size);
	graph.classes.resize(size);
	//memset(nodes,0,sizeof(vertex)*size);

	int from, to, weight;
	int max = 0;
	edge a;
	for (int i = 0; i<n_arc; i++)
	{
		char c = fgetc(fp);
		c = fgetc(fp);
		fscanf(fp, "%d%d%d", &from, &a.id_to, &a.weight);
		sum_arc += a.weight;
		if (max < a.weight)
			max = a.weight;
		//nodes[from].id=from;
		graph.vertices[from].edges.push_back(a);
		//nodes[from].degree++;
	}
	std::fclose(fp);
	std::printf("max weight %d\n", max);
	int cc[30] = { 0 };
	max = 0;
	for (int i = 1; i<size; i++)
	{
		if (graph.vertices[i].edges.size()>max)
			max = graph.vertices[i].edges.size();
		cc[graph.vertices[i].edges.size()]++;
	}
	std::printf("Finish loading graph data\n");
	//if(max>15)
	//	printf("Warning: the max degree of vertex in this graph is more than 15. SPLZ cannot calculate a correct result.\n");

	fp = fopen(file_co, "r");
	if (!fp)
	{
		std::printf("Error: cannot open the file: %s\n", file_co);
		return;
	}

	//skip some information
	fgets(buf, 200, fp);
	fgets(buf, 200, fp);
	fgets(buf, 200, fp);
	fgets(buf, 200, fp);
	fgets(buf, 200, fp);
	fgets(buf, 200, fp);
	fgets(buf, 200, fp);
	int v;
	int x, y;
	graph.bound_east = -INF;
	graph.bound_west = INF;
	graph.bound_north = -INF;
	graph.bound_south = INF;
	for (int i = 1; i<size; i++)
	{
		char c = fgetc(fp);
		c = fgetc(fp);
		fscanf(fp, "%d%d%d", &v, &x, &y);
		graph.vertices[v].shapeid = v;
		graph.vertices[v].x = x;
		graph.vertices[v].y = y;
		graph.UpdateBound(graph.vertices[v]);
	}
	std::fclose(fp);
}

void LoadClass(Graph &graph, char *file_class)//加载分类信息
{

	FILE *fp = fopen(file_class, "r");
	if (!fp)
	{
		std::printf("Error: cannot open the file: %s\n", file_class);
		return;
	}
	int size = graph.classes.size();
	int maxpart = 0;
	for (int i = 1; i<size; i++)//读取分块信息
	{
		int part;
		fscanf(fp, "%d", &part);
		graph.classes[i] = part;
		if (maxpart<part)
			maxpart = part;
	}
	graph.part = maxpart + 1;
	for (int i = 1; i < size; i++)
	{
		for (int j = 0; j < graph.vertices[i].edges.size(); j++)
		{
			graph.vertices[i].edges[j].sign.setK(graph.part);//分块内边标记
			if (graph.classes[graph.vertices[i].edges[j].id_to] == graph.classes[i])
				graph.vertices[i].edges[j].sign.setBit(graph.classes[i], true);
			else//添加各分块出口点
			{
				graph.vertexcut.push_back(i);
				graph.vertexcut.push_back(graph.vertices[i].edges[j].id_to);
			}
		}
	}

	sort(graph.vertexcut.begin(), graph.vertexcut.end());//出口点去重
	vector<int>::iterator iter = unique(graph.vertexcut.begin(), graph.vertexcut.end());
	graph.vertexcut.erase(iter, graph.vertexcut.end());
	std::fclose(fp);
}