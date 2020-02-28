#include "Maze.h"

int main()
{
	using ULL = unsigned long long;

	//Field field("maze1.txt");
	Maze maze("maze1.txt");
	std::cout << "Reachable Pusher Neighbors:\n";
	maze.PrintPositionsOnMaze(maze.GetAccessibleNeighbors(maze.GetPBPos_0(),maze.GetPusherPos_0()));
	std::cout << "All Reachable Pusher Positions:\n";
	maze.PrintPositionsOnMaze(maze.GetReachablePushPositions( maze.GetPBPos_0() ));
	std::cout << "Reachable Ball Push Positions: \n";
	maze.PrintPositionsOnMaze(maze.GetReachableBallPushPositions(maze.GetPBPos_0() ));
	std::cout << "Possible new ball positions: \n";
	maze.PrintPositionsOnMaze(maze.GetNewBallPositions(maze.GetPBPos_0() ));
	
	std::cout << "new PBPositions:\n";
	std::vector<ULL> vec = maze.GetNewPBPositions(maze.GetPBPos_0());
	for (ULL pbpos : vec)
	{
		std::cout << "pusher:" << (pbpos >> 32) << ",ball:" << ((pbpos << 32) >> 32) << '\n';
	}
	for (ULL pbpos : vec)
	{
		maze.PrintPBPosOnMaze(pbpos);
	}
	std::cin.get();
}