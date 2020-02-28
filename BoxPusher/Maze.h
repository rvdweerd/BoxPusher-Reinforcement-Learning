#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

struct Field
{
	Field(std::string filename)
	{
		std::ifstream in(filename);
		std::string str;
		while(std::getline(in,str))
		{
			for (size_t i = 0; i < str.size(); i++)
			{
				char ch = str[i];
				vec.push_back(ch);
				map.insert({ vec.size() - 1,ch });
				if (ch != '.' && ch != '#') startpositions[ch] = vec.size() - 1;
			}
			std::cout << str << '\n';
			height++;
		}
		size = vec.size();
		width = size / height;
	}
	std::vector<char> vec;
	size_t width=0;
	size_t height=0;
	size_t size=0;
	std::unordered_map<size_t,char> map;
	std::unordered_map<char, size_t> startpositions;
};

class Maze
{
	using ULL = unsigned long long;
public:
	Maze(std::string filename);
	std::vector<size_t> GetReachablePushPositions();
	std::vector<size_t> GetReachableBallPushPositions();
	std::vector<size_t> GetNewBallPositions();
	std::vector<ULL> GetNewPBPositions();
	std::vector<size_t> GetAccessibleNeighbors(size_t pos);
	bool IsReachableByPusher(size_t fieldPos);
	void PrintPositionsOnMaze(std::vector<size_t> vec);
	size_t GetPusherPos()
	{
		return pusherPos;
	}
	size_t NumberOfPushesToGoal();
private:
	Field field;
	size_t ballPos;
	size_t pusherPos;
	const size_t goalPos;
};