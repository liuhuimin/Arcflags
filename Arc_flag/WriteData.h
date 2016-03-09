void WriteSearch(char *file, Graph &graph, vector<pair<int, int>> &search)//在边信息上叠加扩展节点信息到文件
{
	FILE *fp = fopen(file, "a");
	int size = search.size();
	if (fp == NULL) {
		printf("cannot open file/n"); exit(1);
	}
	for (int i = 0; i < size; i++)
	{
		fprintf(fp,"\n%d %d %d %d %c", graph.vertices[search[i].first].x, graph.vertices[search[i].first].y, graph.vertices[search[i].second].x, graph.vertices[search[i].second].y,'X');
	}
	fclose(fp);
}
void WriteRoad(char *file, Graph &graph, int *paths, int s, int t)//重写路径到文件
{
	FILE *fp = fopen(file, "w");
	int path = t;
	if (fp == NULL) {
		printf("cannot open file/n"); exit(1);
	}
	do{
		fprintf(fp, "%d %d %d %d\n", graph.vertices[paths[path]].x, graph.vertices[paths[path]].y, graph.vertices[path].x, graph.vertices[path].y);
		path = paths[path];
	} while (path != s);
	fclose(fp);
}