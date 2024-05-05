#include "PathFinder.h"
#include <fstream>
#include <iostream>

PathFinder::PathFinder(){}

PathFinder::~PathFinder()
{
	int size = nodes.size();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			delete nodes[i][j];
		}
	}
	nodes.clear();
}

void PathFinder::LoadMap(const std::string& filepath)
{
	nodes.clear();
	std::ifstream file(filepath);
	std::vector<std::string> fileData;
	while (!file.eof())
	{
		std::string row;
		file >> row;
		fileData.push_back(row);
	}

	for (int i = 0; i < fileData.size(); i++)
	{
		std::vector<Node*> nodesRow;
		for (int j = 0; j < fileData[i].size(); j++)
		{
			Node* node = new Node{ i, j, fileData[i][j], {nullptr,nullptr,nullptr,nullptr} };
			nodesRow.push_back(node);
		}
		nodes.push_back(nodesRow);
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes.size(); j++)
		{
			if (nodes[i][j]->type == '#')
			{
				continue;
			}

			if (i > 0 && nodes[i - 1][j]->type != '#') // up
			{
				nodes[i][j]->neighbours[0] = nodes[i - 1][j];
			}
			if (j < nodes.size() - 1 && nodes[i][j + 1]->type != '#') // right
			{
				nodes[i][j]->neighbours[1] = nodes[i][j + 1];
			}
			if (i < nodes.size() - 1 && nodes[i + 1][j]->type != '#') // down
			{
				nodes[i][j]->neighbours[2] = nodes[i + 1][j];
			}
			if (j > 0 && nodes[i][j - 1]->type != '#') // left
			{
				nodes[i][j]->neighbours[3] = nodes[i][j - 1];
			}
		}
	}
}