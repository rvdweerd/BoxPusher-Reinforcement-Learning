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
				vec0.push_back(ch);
				map.insert({ vec0.size() - 1,ch });
				if (ch != '.' && ch != '#') startpositions[ch] = vec0.size() - 1;
			}
			std::cout << str << '\n';
			height++;
		}
		size = vec0.size();
		width = size / height;
		vec = vec0;
		vec[startpositions['b']] = '.';
		vec[startpositions['O']] = '.';
	}
	std::vector<char> vec0;
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
	std::vector<size_t> GetReachablePushPositions(ULL PBPos);
	std::vector<size_t> GetReachableBallPushPositions(ULL PBPos);
	std::vector<size_t> GetNewBallPositions(ULL PBPos);
	std::vector<ULL> GetNewPBPositions(ULL PBPos);
	std::vector<size_t> GetAccessibleNeighbors(ULL PBPos, size_t pos);
	bool IsReachableByPusher(ULL PBPos, size_t fieldPos);
	void PrintPositionsOnMaze(std::vector<size_t> vec);
	void PrintPBPosOnMaze(ULL PBPos);
	size_t GetPusherPos_0()
	{
		return pusherPos_0;
	}
	ULL GetPBPos_0()
	{
		return PBPos_0;
	}
	size_t NumberOfPushesToGoal();
private:
	Field field;
	size_t ballPos_0;
	size_t pusherPos_0;
	ULL PBPos_0;
	const size_t goalPos;
};