#include "Task12.h"
#include <fstream>
#include "../IO.h"


using namespace TASK_12;


bool Graph::AreNeighbours(uint v, uint w)
{
	std::set<uint>& v_n = adjacency_table[v];
	std::set<uint>& w_n = adjacency_table[w];

	if (v_n.find(w) != v_n.end())
		return true;

	//Check other way round (Only matters in weighted graph)
	else if (w_n.find(v) != w_n.end())
		return true;

	return false;
}

bool Graph::IsPath(uint v, uint w, std::vector<uint>& out)
{
	bool* visited = new bool[largest_node + 1]{false};
	bool answer = IsPath(v, w, out, visited);
	delete[] visited;
	return answer;
}

bool Graph::IsPath(uint v, uint w, std::vector<uint>& out, bool* visited) 
{
	if (AreNeighbours(v, w))
	{
		out.push_back(v);
		out.push_back(w);
		return true;
	}

	out.push_back(v);
	for (uint n : GetNeighbours(v))
	{
		if (!visited[n])
		{
			visited[n] = true;
			if (IsPath(n, w, out, visited))
				return true;
			else
				visited[n] = false;
		}
	}
	out.pop_back();

	return false;
}

Graph Graph::Construct(std::string file_path) 
{
	Graph g;

	std::ifstream file;
	file.open(file_path, std::ifstream::in);

	if (!file.is_open())
	{
		IO::out << "Invalid path!\n";
		return g;
	}

	char c;
	uint node = 1;
	uint relation = 1;
	std::set<uint> neighbours;

	while (file.get(c))
	{
		if (c == '\n')
		{
			//Print
			{
				IO::out_debug << "(" << node << "):{";
				for (const uint i : neighbours)
					IO::out_debug << i << ",";
				IO::out_debug << "}\n";
			}

			g.CreateNode(node, neighbours);
			node++;
			relation = 1;
			neighbours.clear();
		}
		else
		{
			if (c == '1')
				neighbours.insert(relation);
			relation++;
		}
	}
	g.largest_node = node - 1;
	file.close();

	return g;
}

void TASK_12::Execute()
{
	std::string file_path;
	IO::out << "Input the graph file path (File should be in matrix format where 1 is arc and 0 is no connection):\n";
	IO::in >> file_path;

	Graph g = Graph::Construct(file_path);

	uint v, w;
	std::vector<uint> path;

	IO::out << "Which 2 nodes would you like to find a path for?\n";
	IO::in >> v;
	while (v > g.GetLargestNode())
	{
		IO::out << "Cannot be greater than " << g.GetLargestNode() << "\n";
		IO::in >> v;
	}

	IO::in >> w;
	while (w > g.GetLargestNode())
	{
		IO::out << "Cannot be greater than " << g.GetLargestNode() << "\n";
		IO::in >> w;
	}
	
	bool path_found = g.IsPath(v, w, path);


	if (path_found)
	{
		IO::out << "Path found\n";

		IO::out << "Path:\n\t";
		for (uint& i : path)
			IO::out << i << ",";
		IO::out << "\n";  

		//Write to file
		std::ofstream file("t12_output.txt");
		file.clear();

		for (uint& i : path)
			file << i << ",";
		file.close();
		IO::out << "Path written to t12_output.txt\n";
	}
	else if(v == w)
		IO::out << "No additional path found.";
	else
		IO::out << "No path found.";

}