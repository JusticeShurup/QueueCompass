#include "PathFinder.h"
#include "MyQueue.h"
#include <fstream>
#include <iostream>
#include <map>
#include "deps/stb_image.h"
#include "deps/stb_image_write.h"


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
		std::cout << row << std::endl;
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

	MakeNeighbors();

	mapLoaded = true;
}

void PathFinder::LoadMapFromImage(const std::string& filepath)
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

	int width;
	int height;
	int channels;
	unsigned char* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
	
	for (int i = 0; i < height; i++)
	{
		std::vector<Node*> nodesRow;
		for (int j = 0; j < width; j++)
		{
			Node* node;
			if(pixels[(height * i + j) * channels] == 0x80 && pixels[(height * i + j) * channels + 1] == 0x80 && pixels[(height * i + j) * channels + 2] == 0x80)
			{
				node = new Node{ j, i, '#', {nullptr,nullptr,nullptr,nullptr}};
			}
			else if (pixels[(height * i + j) * channels] == 0xFF && pixels[(height * i + j) * channels + 1] == 0x00 && pixels[(height * i + j) * channels + 2] == 0x00)
			{
				node = new Node{ j, i, 's', {nullptr,nullptr,nullptr,nullptr} };
			}
			else if (pixels[(height * i + j) * channels] == 0x00 && pixels[(height * i + j) * channels + 1] == 0x00 && pixels[(height * i + j) * channels + 2] == 0xFF)
			{
				node = new Node{ j, i, 'e', {nullptr,nullptr,nullptr,nullptr} };
			}
			else if (pixels[(height * i + j) * channels] == 0xFF && pixels[(height * i + j) * channels + 1] == 0xFF && pixels[(height * i + j) * channels + 2] == 0x00)
			{
				node = new Node{ j, i, 'o', {nullptr,nullptr,nullptr,nullptr} };
			}
			nodesRow.push_back(node);
		}
		nodes.push_back(nodesRow);
	}

	MakeNeighbors();

	stbi_image_free(pixels);

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
	/*
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = nodes.size() - 1; j >= 0; j--)
		{
			Node* node = nodes[i][j];
			//std::cout << node->type;
			switch (node->type)
			{
				case '#':
				{
					pixels[i] = 0x80);
					pixels[i] = 0x80);
					pixels[i] = 0x80);
					break;
				}
				case 's':
				{
					pixels[i] = 0xFF);
					pixels[i] = 0x00);
					pixels[i] = 0x00);
					break;
				}
				case 'e':
				{
					pixels[i] = 0x00);
					pixels[i] = 0x00);
					pixels[i] = 0xFF);
					break;
				}
				case 'x':
				{
					pixels[i] = 0x00);
					pixels[i] = 0xFF);
					pixels[i] = 0x00);
					break;
				}
				case 'o':
				{
					pixels[i] = 0xFF);
					pixels[i] = 0xFF);
					pixels[i] = 0x00);
					break;
				}
			default:
				break;
			}
		}
		//std::cout << std::endl;
	}
	*/

	for (int i = imageData.size() - 1; i > 0; i -= 3)
	{
		file.write((char*)&imageData[i], 1);
		file.write((char*)&imageData[i - 1], 1);
		file.write((char*)&imageData[i - 2], 1);
	}


	file.close();
}


void PathFinder::SaveToImage() const 
{
	const int width = nodes.size();
	const int heigth = nodes[0].size();
	const int pixelsCount = width * heigth;

	unsigned char* pixels = new unsigned char[pixelsCount * 3];
	for (int i = 0; i < width * heigth; i++)
	{
		Node* node = nodes[i / heigth][i % heigth];

		switch (node->type)
		{
		case '#':
		{
			pixels[i * 3] = 0x80;
			pixels[i * 3 + 1] = 0x80;
			pixels[i * 3 + 2] = 0x80;
			break;
		}
		case 's':
		{
			pixels[i * 3] = 0xFF;
			pixels[i * 3 + 1] = 0x00;
			pixels[i * 3 + 2] = 0x00;
			break;
		}
		case 'e':
		{
			pixels[i * 3] = 0x00;
			pixels[i * 3 + 1] = 0x00;
			pixels[i * 3 + 2] = 0xFF;
			break;
		}
		case 'x':
		{
			pixels[i * 3] = 0x00;
			pixels[i * 3 + 1] = 0xFF;
			pixels[i * 3 + 2] = 0x00;
			break;
		}
		case 'o':
		{
			pixels[i * 3] = 0xFF;
			pixels[i * 3 + 1] = 0xFF;
			pixels[i * 3 + 2] = 0x00;
			break;
		}
		default:
			break;
		}
	}

	int success = stbi_write_bmp("result.png", width, heigth, 3, pixels);
	
}

void PathFinder::MakeNeighbors()
{
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
}

