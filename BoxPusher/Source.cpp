#include "Maze.h"

int main()
{
	//Field field("maze1.txt");
	Maze maze("maze1.txt");
	std::cout << "Reachable Pusher Neighbors:\n";
	maze.PrintPositionsOnMaze(maze.GetAccessibleNeighbors(maze.GetPusherPos()));
	std::cout << "All Reachable Pusher Positions:\n";
	maze.PrintPositionsOnMaze(maze.GetReachablePushPositions());
	std::cout << "Reachable Ball Push Positions: \n";
	maze.PrintPositionsOnMaze(maze.GetReachableBallPushPositions());
	std::cout << "Possible new ball positions: \n";
	maze.PrintPositionsOnMaze(maze.GetNewBallPositions());
	
	std::cin.get();
}