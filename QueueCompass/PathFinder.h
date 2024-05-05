#ifndef _PATH_FINDER_H_
#define _PATH_FINDER_H_
#include <vector>
#include <string>

class PathFinder 
{
public:
	PathFinder();
	~PathFinder();

	void LoadMap(const std::string& filepath);
	void FindPath();
	void PrintPath() const;
	void SavePathToImage() const;

private:
	struct Node {
		int x;
		int y;
		char type;
		Node* neighbours[4];
	};
	std::vector<std::vector<Node*>> nodes;

	bool mapLoaded;
	Node* startNode;
	Node* endNode;
};

#endif // !_PATH_FINDER_H_
