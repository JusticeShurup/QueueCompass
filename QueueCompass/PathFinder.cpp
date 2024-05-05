#include "PathFinder.h"
#include "MyQueue.h"
#include <fstream>
#include <iostream>
#include <map>

PathFinder::PathFinder()
{
	mapLoaded = false;
}

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
	int size = nodes.size();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			delete nodes[i][j];
		}
	}
	nodes.clear();
	std::ifstream file(filepath);
	std::vector<std::string> fileData;
	while (!file.eof())
	{
		std::string row;
		file >> row;
		fileData.push_back(row);
	}
	file.close();

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
			Node* node = nodes[i][j];
			if (node->type == '#')
			{
				continue;
			}

			if (node->type == 's')
			{
				startNode = node;
			}
			if (node->type == 'e')
			{
				endNode = node;
			}

			if (i > 0 && nodes[i - 1][j]->type != '#') // up
			{
				node->neighbours[0] = nodes[i - 1][j];
			}
			if (j < nodes.size() - 1 && nodes[i][j + 1]->type != '#') // right
			{
				node->neighbours[1] = nodes[i][j + 1];
			}
			if (i < nodes.size() - 1 && nodes[i + 1][j]->type != '#') // down
			{
				node->neighbours[2] = nodes[i + 1][j];
			}
			if (j > 0 && nodes[i][j - 1]->type != '#') // left
			{
				node->neighbours[3] = nodes[i][j - 1];
			}
		}
	}

	mapLoaded = true;
}

void PathFinder::FindPath()
{
	if (!mapLoaded)
	{
		std::cout << "Error!" << std::endl;
	}
	std::map<Node*, bool> visitedNodes;
	std::map<Node*, Node*> path;
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes.size(); j++)
		{
			visitedNodes[nodes[i][j]] = false;
		}
	}
	MyQueue<Node*> queue;
	queue.Push(startNode);
	Node* previousNode = nullptr;
	while (!queue.Empty())
	{
		Node* node = queue.Pop();
		visitedNodes[node] = true;
		for (int i = 0; i < 4; i++)
		{
			if (node->neighbours[i] != nullptr)
			{
				if (!visitedNodes[node->neighbours[i]])
				{
					path[node->neighbours[i]] = node;
					queue.Push(node->neighbours[i]);
				}
			}
		}
	}

	Node* temp = path[endNode];
	int count = 0;
	while (temp != startNode)
	{
		temp->type = 'x';
		temp = path[temp];
	}
}

void PathFinder::PrintPath() const
{
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes.size(); j++)
		{
			std::cout << nodes[i][j]->type;
		}
		std::cout << std::endl;
	}
}

void PathFinder::SavePathToImage() const
{
	std::ofstream file("result.bmp", std::ios::binary | std::ios::out);
	
	unsigned short bfType = 0x4D42;
	unsigned int fileSize = 14 + 12 + nodes.size() * nodes.size() * 3;
	unsigned short bfReserved1 = 0;
	unsigned short bfReserved2 = 0;
	unsigned int bfOffBits = 14 + 12;
	
	unsigned int biSize = 12;
	unsigned short biWidth = nodes.size();
	unsigned short biHeight = nodes.size();
	unsigned short biBitCount = 24;
	unsigned short biPlanes = 1;

	file.write((char*)&bfType, 2);
	file.write((char*)&fileSize, 4);
	file.write((char*)&bfReserved1, 2);
	file.write((char*)&bfReserved2, 2);
	file.write((char*)&bfOffBits, 4);

	file.write((char*)&biSize, 4);
	file.write((char*)&biWidth, 2);
	file.write((char*)&biHeight, 2);
	file.write((char*)&biPlanes, 2);
	file.write((char*)&biBitCount, 2);

	std::vector<unsigned char> imageData;
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = nodes.size() - 1; j >= 0; j--)
		{
			Node* node = nodes[i][j];
			std::cout << node->type;
			switch (node->type)
			{
				case '#':
				{
					imageData.push_back(0x80);
					imageData.push_back(0x80);
					imageData.push_back(0x80);
					break;
				}
				case 's':
				{
					imageData.push_back(0xFF);
					imageData.push_back(0x00);
					imageData.push_back(0x00);
					break;
				}
				case 'e':
				{
					imageData.push_back(0x00);
					imageData.push_back(0x00);
					imageData.push_back(0xFF);
					break;
				}
				case 'x':
				{
					imageData.push_back(0x00);
					imageData.push_back(0xFF);
					imageData.push_back(0x00);
					break;
				}
				case 'o':
				{
					imageData.push_back(0xFF);
					imageData.push_back(0xFF);
					imageData.push_back(0x00);
					break;
				}
			default:
				break;
			}
		}
		std::cout << std::endl;
	}

	for (int i = imageData.size() - 1; i > 0; i -= 3)
	{
		file.write((char*)&imageData[i], 1);
		file.write((char*)&imageData[i - 1], 1);
		file.write((char*)&imageData[i - 2], 1);
	}


	file.close();
}


