


int Astar(Graph &graph,int s, int t, int *label, int *path)//A*
{
	int *exlabel = new int[graph.vertices.size()];//label+��ֵ
	int *estimated_dist = new int[graph.vertices.size()];//��ֵ
	int *flag = new int[graph.vertices.size()];//����״̬�ı��
	memset(flag, 0, sizeof(int)*graph.vertices.size());

	Priority_Queue Q(graph.vertices.size() / 100, exlabel, graph.vertices.size());

	int id = s;
	path[s] = s;
	label[s] = 0;

	//flag[s]=FINISHED;

	while (id != t)//������û��·�������
	{
		/*if (id == 27153)
		{
		cout << id << endl;
		cout << vertices[id].edges.size() << endl;
		}*/
		for (int i = 0; i<graph.vertices[id].edges.size(); i++)
		{
			int id_to = graph.vertices[id].edges[i].id_to;
			switch (flag[id_to])
			{
			case NEVER://��δ������
				label[id_to] = label[id] + graph.vertices[id].edges[i].weight;
				estimated_dist[id_to] = Euclidean_Dist(graph,id_to, t);
				exlabel[id_to] = label[id_to] + estimated_dist[id_to];
				Q.Push(id_to);
				flag[id_to] = EVER;
				path[id_to] = id;
				break;
			case EVER://����������
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