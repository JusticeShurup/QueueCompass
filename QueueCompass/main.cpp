#include "MyQueue.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "deps/stb_image.h"
#include "deps/stb_image_write.h"
#include "PathFinder.h"

int main()
{
	PathFinder pathFinder;
	//pathFinder.LoadMap("maps/map1.txt");
	pathFinder.LoadMapFromImage("maps/map1.png");
	pathFinder.FindPath();
	//pathFinder.PrintPath();
	pathFinder.SaveToImage();
	return 0;
}