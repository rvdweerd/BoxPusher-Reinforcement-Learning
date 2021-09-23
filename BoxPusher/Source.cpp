#include "RLutils.h"
#include "BFSutils.h"
#include "Worlds.h"

class Solution_RL {
public:
	int minPushBox(std::vector<std::vector<char>>& grid)
	{
		RL::RLField field(grid);
		std::cout << "Demo of Q-Learning solution... (press any key)";
		std::cin.get();
		system("cls");
		field.PlayManually();
		return RL::RL_solve(field);
	}
};

class Solution_BFS {
public:
	int minPushBox(std::vector<std::vector<char>>& grid)
	{
		BFS::Maze maze(grid);
		std::cout << "Demo of BFS solution... (press any key)";
		std::cin.get();
		
		system("cls"); 
		std::cout << "Field and initial positions.\n\n";
		std::cout << "b = Box Pusher\n";
		std::cout << "O = Box Position\n";
		std::cout << "X = Target box position\n\n";
		maze.PrintPBPosOnMaze(maze.GetPBPos_0());
		std::cout << "\nReachable Pusher Neighbors ($):\n";
		maze.PrintPositionsOnMaze(maze.GetAccessibleNeighbors(maze.GetPBPos_0(),maze.GetPusherPos_0()));
		std::cout << "(press any key)"; std::cin.get();

		system("cls"); maze.PrintPBPosOnMaze(maze.GetPBPos_0());
		std::cout << "\nAll Reachable Pusher Positions ($):\n";
		maze.PrintPositionsOnMaze(maze.GetReachablePushPositions( maze.GetPBPos_0() ));
		std::cout << "(press any key)"; std::cin.get();

		system("cls"); maze.PrintPBPosOnMaze(maze.GetPBPos_0());
		std::cout << "\nReachable Ball Push Positions ($): \n";
		maze.PrintPositionsOnMaze(maze.GetReachableBallPushPositions(maze.GetPBPos_0() ));
		std::cout << "(press any key)"; std::cin.get();

		system("cls"); maze.PrintPBPosOnMaze(maze.GetPBPos_0());
		std::cout << "\nPossible new ball positions ($): \n";
		maze.PrintPositionsOnMaze(maze.GetNewBallPositions(maze.GetPBPos_0() ));
		std::cout << "(press any key)"; std::cin.get();

		system("cls"); maze.PrintPBPosOnMaze(maze.GetPBPos_0());
		std::cout << "\nPossible new PBPositions ($):\n";
		std::vector<BFS::ULL> vec = maze.GetNewPBPositions(maze.GetPBPos_0());
		for (BFS::ULL pbpos : vec)
		{
			std::cout << "pusher:" << (pbpos >> 32) << ",ball:" << ((pbpos << 32) >> 32) << '\n';
		}
		for (BFS::ULL pbpos : vec)
		{
			maze.PrintPBPosOnMaze(pbpos);
		}

		std::cout << "Minimum number of box pushes to goal: ";
		BFS::PBData endstate = maze.NumberOfPushesToGoal();
		if (endstate.n == -1) std::cout << "[No solution]";
		else
		{
			std::cout << endstate.n << '\n';
			std::cout << "Press [Enter] to show shortest path...";
		}
		std::cin.get();
		system("cls");

		for (BFS::ULL p : endstate.path)
		{
			maze.PrintPBPosOnMaze(p);
			std::cin.get();
			system("cls");
		}
		std::cin.get();
		return endstate.n;
	}
};

int main()
{
	auto grids = LoadGrids();

	// ############################################
	// BFS for one grid
	// ############################################
	//SearchAlgo(grids[4]);
	Solution_BFS().minPushBox(grids[4]);

	// ############################################
	// RL for one grid
	// ############################################
	Solution_RL().minPushBox(grids[4]);
}