#include "MyQueue.h"
#include <iostream>
#include "PathFinder.h"


int main()
{
	PathFinder pathFinder;
	pathFinder.LoadMap("maps/map1.txt");
	pathFinder.FindPath();
	//pathFinder.PrintPath();
	pathFinder.SavePathToImage();

	return 0;
}