#include "RLutils.h"
#include "BFSutils.h"
#include "Worlds.h"

class Solution2 {
public:
	int minPushBox(std::vector<std::vector<char>>& grid)
	{
		RL::RLField field(grid);
		field.PlayManually();
		return RL::RL_solve(field);
	}
};


void SearchAlgo() {
	std::vector<std::vector<char>> grid1 =
	{
		{'#', '#', '#', '#', '#', '#'},
		{'#', 'T', '#', '#', '#', '#'},
		{'#', '.', '.', 'B', '.', '#'},
		{'#', '.', '#', '#', '.', '#'},
		{'#', '.', '.', '.', 'S', '#'},
		{'#', '#', '#', '#', '#', '#'} };

	std::vector<std::vector<char>> grid2 =
	{ {'#', '#', '#', '#', '#', '#'},
	{'#', 'T', '#', '#', '#', '#'},
	{'#', '.', '.', 'B', '.', '#'},
	{'#', '#', '#', '#', '.', '#'},
	{'#', '.', '.', '.', 'S', '#'},
	{'#', '#', '#', '#', '#', '#'} };

	std::vector<std::vector<char>> grid3 =
	{ {'#', '#', '#', '#', '#', '#'},
	{'#', 'T', '.', '.', '#', '#'},
	{'#', '.', '#', 'B', '.', '#'},
	{'#', '.', '.', '.', '.', '#'},
	{'#', '.', '.', '.', 'S', '#'},
	{'#', '#', '#', '#', '#', '#'} };

	std::vector<std::vector<char>> grid4 =
	{ {'#', '#', '#', '#', '#', '#', '#'},
	{'#', 'S', '#', '.', 'B', 'T', '#'},
	{'#', '#', '#', '#', '#', '#', '#'} };

	Maze maze("maze2.txt");
	//Maze maze(grid1);
	maze.PrintPBPosOnMaze(maze.GetPBPos_0());


	//std::cout << "Reachable Pusher Neighbors:\n";
	//maze.PrintPositionsOnMaze(maze.GetAccessibleNeighbors(maze.GetPBPos_0(),maze.GetPusherPos_0()));
	//std::cout << "All Reachable Pusher Positions:\n";
	//maze.PrintPositionsOnMaze(maze.GetReachablePushPositions( maze.GetPBPos_0() ));
	//std::cout << "Reachable Ball Push Positions: \n";
	//maze.PrintPositionsOnMaze(maze.GetReachableBallPushPositions(maze.GetPBPos_0() ));
	//std::cout << "Possible new ball positions: \n";
	//maze.PrintPositionsOnMaze(maze.GetNewBallPositions(maze.GetPBPos_0() ));
	//
	//std::cout << "new PBPositions:\n";
	//std::vector<ULL> vec = maze.GetNewPBPositions(maze.GetPBPos_0());
	//for (ULL pbpos : vec)
	//{
	//	std::cout << "pusher:" << (pbpos >> 32) << ",ball:" << ((pbpos << 32) >> 32) << '\n';
	//}
	//for (ULL pbpos : vec)
	//{
	//	maze.PrintPBPosOnMaze(pbpos);
	//}
	std::cout << "b = Box Pusher\n";
	std::cout << "O = Box Position\n";
	std::cout << "X = Target box position\n\n";

	std::cout << "Minimum number of box pushes to goal: ";
	PBData endstate = maze.NumberOfPushesToGoal();
	if (endstate.n == -1) std::cout << "[No solution]";
	else
	{
		std::cout << endstate.n << '\n';
		std::cout << "Press [Enter] to show shortest path...";
	}
	std::cin.get();
	system("cls");

	for (ULL p : endstate.path)
	{
		maze.PrintPBPosOnMaze(p);
		std::cin.get();
		system("cls");
	}

	std::cin.get();

}
void RLrun() {
	std::vector<std::vector<char>> grid1 =
	{
		{'#', '#', '#', '#', '#', '#'},
		{'#', 'T', '#', '#', '#', '#'},
		{'#', '.', '.', 'B', '.', '#'},
		{'#', '.', '#', '#', '.', '#'},
		{'#', '.', '.', '.', 'S', '#'},
		{'#', '#', '#', '#', '#', '#'} };
	RL::RLField rlField(grid1);
	RL::RL_solve(rlField);
}

int main()
{
	auto grids = LoadGrids();
	//RLrun();
	//SearchAlgo();
	

	/*for (auto grid : allGrids) {
		auto f = RL::RLField(grid);
		RL::RL_test(f);
		std::cout << Solution2().minPushBox(grid);
	}*/

	auto f = RL::RLField(grids[4]);
	std::cout << Solution2().minPushBox(grids[4]);
}