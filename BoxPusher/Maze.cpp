#include "Maze.h"
#include <set>
#include <queue>
#include <algorithm>
using ULL = unsigned long long;
auto PBPos = [](size_t pusherPos, size_t ballPos)->ULL { return (ULL(ballPos) << 32 | pusherPos); };

Maze::Maze(std::string filename)
	:
	field(filename),
	ballPos(field.startpositions['O']),
	pusherPos(field.startpositions['b']),
	goalPos(field.startpositions['X'])
{
}
std::vector<size_t> Maze::GetReachablePushPositions()
{
	std::vector<size_t> returnvec;
	for (size_t y = 0; y < field.height; y++)
	{
		for (size_t x = 0; x < field.width; x++)
		{
			size_t pos = y * field.width + x;
			if (IsReachableByPusher(pos)) returnvec.push_back(pos);
		}
	}
	return returnvec;
}
std::vector<size_t> Maze::GetReachableBallPushPositions()
{
	std::vector<size_t> ballNeighbors = GetAccessibleNeighbors(ballPos);
	std::vector<size_t> returnvec;
	std::copy_if(ballNeighbors.begin(), ballNeighbors.end(), std::back_inserter(returnvec), [this](size_t p) {return IsReachableByPusher(p); });
	return returnvec;
}
std::vector<size_t> Maze::GetNewBallPositions()
{
	std::vector<size_t> pushPositions = GetReachableBallPushPositions();
	std::vector<size_t> returnvec;
	size_t ballX = ballPos % field.width;
	size_t ballY = ballPos / field.width;
	for (auto p : pushPositions)
	{
		auto CanPlace = [this](size_t p)->bool {return (field.vec[p] == '.' || field.vec[p] == 'X' || field.vec[p] == 'b'); };
		int delta = ballPos - p;
		int test = ballPos + delta;
		if (test >= 0)
		{
			if (delta == 1 && ballX != field.width - 1 && CanPlace(ballPos+delta)) returnvec.push_back(ballPos + delta);
			if (delta == -1 && ballX != 0 && CanPlace(ballPos + delta)) returnvec.push_back(ballPos + delta);
			if (delta == field.width && ballY != field.height - 1 && CanPlace(ballPos + delta)) returnvec.push_back(ballPos + delta);
			if (delta == -(int)field.width && ballY != 0 && CanPlace(ballPos + delta)) returnvec.push_back(ballPos + delta);
		}
	}
	return returnvec;
}
std::vector<ULL> Maze::GetNewPBPositions()
{
	std::vector<size_t> pushPositions = GetReachableBallPushPositions();
	std::vector<ULL> returnvec;
	size_t ballX = ballPos % field.width;
	size_t ballY = ballPos / field.width;
	for (auto p : pushPositions)
	{
		auto CanPlace = [this](size_t p)->bool {return (field.vec[p] == '.' || field.vec[p] == 'X' || field.vec[p] == 'b'); };
		int delta = ballPos - p;
		int test = ballPos + delta;
		if (test >= 0)
		{
			if (delta == 1 && ballX != field.width - 1 && CanPlace(ballPos + delta))
			{
				returnvec.push_back(PBPos(pusherPos,ballPos + delta));
			}
			if (delta == -1 && ballX != 0 && CanPlace(ballPos + delta))
			{
				returnvec.push_back(PBPos(pusherPos, ballPos + delta));
			}
			if (delta == field.width && ballY != field.height - 1 && CanPlace(ballPos + delta))
			{
				returnvec.push_back(PBPos(pusherPos, ballPos + delta));
			}
			if (delta == -(int)field.width && ballY != 0 && CanPlace(ballPos + delta))
			{
				returnvec.push_back(PBPos(pusherPos, ballPos + delta));
			}
		}
	}
	return returnvec;
}
std::vector<size_t> Maze::GetAccessibleNeighbors(size_t pos)
{
	size_t x = pos % field.width;
	size_t y = pos / field.width;
	std::vector<size_t> returnvec;
	auto CanStand = [this](size_t p)->bool {return (field.vec[p] == '.' || field.vec[p] == 'b'); };
	// North
	if (y != 0 && CanStand(pos - field.width)) returnvec.push_back(pos - field.width);
	// East
	if (x != field.width-1 && CanStand(pos + 1)) returnvec.push_back(pos + 1);
	// South
	if (y != field.height-1 && CanStand(pos + field.width)) returnvec.push_back(pos + field.width);
	// West
	if (x != 0 && CanStand(pos - 1)) returnvec.push_back(pos - 1);
	return returnvec;
}
bool Maze::IsReachableByPusher(size_t fieldPos)
{
	if (field.vec[fieldPos] == 'b') return true;
	else if (field.vec[fieldPos] != '.') return false;
	//std::vector<char> localField = field.vec;
	std::set<size_t> visited; visited.insert(pusherPos);
	std::queue<size_t> queue;
	queue.push(pusherPos);
	while (!queue.empty())
	{
		size_t currPos = queue.front(); queue.pop();
		if (currPos == fieldPos) return true;
		auto test = GetAccessibleNeighbors(currPos);
		for (size_t p : GetAccessibleNeighbors(currPos))
		{
			if (visited.find(p) == visited.end())
			{
				if (p == fieldPos) return true;
				queue.push(p);
				visited.insert(p);
			}
		}
	}
	return false;
}
void Maze::PrintPositionsOnMaze(std::vector<size_t> vec)
{
	for (size_t y = 0; y < field.height; y++)
	{
		for (size_t x = 0; x < field.width; x++)
		{
			size_t pos = y * field.width + x;
			if (std::find(vec.begin(), vec.end(), pos) != vec.end()) std::cout << '$';
			else std::cout << field.vec[pos];
		}
		std::cout << '\n';
	}
}

size_t Maze::NumberOfPushesToGoal()
{
	return 0;
}

