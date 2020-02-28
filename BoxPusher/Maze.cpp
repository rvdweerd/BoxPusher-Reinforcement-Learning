#include "Maze.h"
#include <set>
#include <queue>
#include <algorithm>
using ULL = unsigned long long;
auto Pos2PBPos = [](size_t pusherPos_0, size_t ballPos_0)->ULL { return (ULL(pusherPos_0) << 32 | ballPos_0); };

Maze::Maze(std::string filename)
	:
	field(filename),
	ballPos_0(field.startpositions['O']),
	pusherPos_0(field.startpositions['b']),
	goalPos(field.startpositions['X'])
{
	PBPos_0 = Pos2PBPos(pusherPos_0, ballPos_0);
}
std::vector<size_t> Maze::GetReachablePushPositions(ULL PBPos)
{
	size_t pusherPos = PBPos >> 32;
	size_t ballPos = (PBPos << 32) >> 32;

	std::vector<size_t> returnvec;
	for (size_t y = 0; y < field.height; y++)
	{
		for (size_t x = 0; x < field.width; x++)
		{
			size_t pos = y * field.width + x;
			if (IsReachableByPusher(PBPos,pos)) returnvec.push_back(pos);
		}
	}
	return returnvec;
}
std::vector<size_t> Maze::GetReachableBallPushPositions(ULL PBPos)
{
	size_t pusherPos = PBPos >> 32;
	size_t ballPos = (PBPos << 32) >> 32;

	std::vector<size_t> ballNeighbors = GetAccessibleNeighbors(PBPos,ballPos);
	std::vector<size_t> returnvec;
	std::copy_if(ballNeighbors.begin(), ballNeighbors.end(), std::back_inserter(returnvec), [this,&PBPos](size_t p) {return IsReachableByPusher(PBPos,p); });
	return returnvec;
}
std::vector<size_t> Maze::GetNewBallPositions(ULL PBPos)
{
	size_t pusherPos = PBPos >> 32;
	size_t ballPos = (PBPos << 32) >> 32;

	std::vector<size_t> pushPositions = GetReachableBallPushPositions(PBPos);
	std::vector<size_t> returnvec;
	size_t ballX = ballPos % field.width;
	size_t ballY = ballPos / field.width;
	for (auto p : pushPositions)
	{
		auto CanPlace = [this,&pusherPos](size_t p)->bool {return (field.vec[p] == '.' || field.vec[p] == 'X' || p == pusherPos); };
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
std::vector<ULL> Maze::GetNewPBPositions(ULL PBPos)
{
	size_t pusherPos = PBPos >> 32;
	size_t ballPos = (PBPos << 32) >> 32;

	std::vector<size_t> pushPositions = GetReachableBallPushPositions(PBPos);
	std::vector<ULL> returnvec;
	size_t ballX = ballPos % field.width;
	size_t ballY = ballPos / field.width;
	for (auto p : pushPositions)
	{
		auto CanPlace = [this, &pusherPos](size_t p)->bool {return (field.vec[p] == '.' || field.vec[p] == 'X' || p == pusherPos); };
		int delta = ballPos - p;
		int test = ballPos + delta;
		if (test >= 0)
		{
			if (delta == 1 && ballX != field.width - 1 && CanPlace(ballPos + delta)) // push ball East
			{
				returnvec.push_back(Pos2PBPos(ballPos, ballPos + delta));
			}
			if (delta == -1 && ballX != 0 && CanPlace(ballPos + delta)) // push ball West
			{
				returnvec.push_back(Pos2PBPos(ballPos, ballPos + delta));
			}
			if (delta == field.width && ballY != field.height - 1 && CanPlace(ballPos + delta)) // push ball South
			{
				returnvec.push_back(Pos2PBPos(ballPos, ballPos + delta));
			}
			if (delta == -(int)field.width && ballY != 0 && CanPlace(ballPos + delta)) // push ball North
			{
				returnvec.push_back(Pos2PBPos(ballPos, ballPos + delta));
			}
		}
	}
	return returnvec;
}
std::vector<size_t> Maze::GetAccessibleNeighbors(ULL PBPos, size_t pos)
{
	size_t pusherPos = PBPos >> 32;
	size_t ballPos = (PBPos << 32) >> 32;

	size_t x = pos % field.width;
	size_t y = pos / field.width;
	std::vector<size_t> returnvec;
	auto CanStand = [this,&pusherPos](size_t p)->bool {return (field.vec[p] == '.' || p == pusherPos); };
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
bool Maze::IsReachableByPusher(ULL PBPos, size_t fieldPos)
{
	size_t pusherPos = PBPos >> 32;
	size_t ballPos = (PBPos << 32) >> 32;
	
	if (fieldPos == pusherPos) return true;
	else if (field.vec[fieldPos] != '.') return false;
	//std::vector<char> localField = field.vec;
	std::set<size_t> visited; visited.insert(pusherPos);
	std::queue<size_t> queue;
	queue.push(pusherPos);
	while (!queue.empty())
	{
		size_t currPos = queue.front(); queue.pop();
		if (currPos == fieldPos) return true;
		//auto test = GetAccessibleNeighbors(currPos);
		for (size_t p : GetAccessibleNeighbors(PBPos,currPos))
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
	for (auto v : vec) std::cout << v << ", ";
	std::cout << "\n\n";
}
void Maze::PrintPBPosOnMaze(ULL PBPos)
{
	std::vector<char> vec_local = field.vec;
	size_t pusherPos = PBPos >> 32;
	size_t ballPos = (PBPos << 32) >> 32;
	vec_local[pusherPos] = 'b';
	vec_local[ballPos] = 'O';

	for (size_t y = 0; y < field.height; y++)
	{
		for (size_t x = 0; x < field.width; x++)
		{
			size_t pos = y * field.width + x;
			std::cout << vec_local[pos];
		}
		std::cout << '\n';
	}
}
size_t Maze::NumberOfPushesToGoal()
{
	std::set<ULL> visited; visited.insert(PBPos_0);
	std::queue<ULL> queue; queue.push(PBPos_0);
	while (!queue.empty())
	{
		ULL curpos = queue.front(); queue.pop();
		for (ULL pbpos : GetNewPBPositions(curpos))
		{
			if (visited.find(pbpos) == visited.end())
			{
				if (size_t((pbpos << 32) >> 32) == goalPos) return 1;
				queue.push(pbpos);
				visited.insert(pbpos);
				PrintPBPosOnMaze(pbpos);
			}
		}
	}
	return 0;
}

