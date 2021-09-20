#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <random> 
#include <set>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <Windows.h>

namespace RL {
	using ULL = unsigned long long;
	auto Pos2PBPos = [](std::pair<size_t, size_t> pbpos)->ULL { return (ULL(pbpos.first) << 32 | pbpos.second); };
	auto PBPos2Pos = [](ULL pbPos)->std::pair<size_t, size_t> { return { pbPos >> 32 , pbPos << 32 >> 32 }; };

	void cls(HANDLE hConsole, int cursorX = 0, int cursorY = 0) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		SMALL_RECT scrollRect;
		COORD scrollTarget;
		CHAR_INFO fill;

		// Get the number of character cells in the current buffer.
		if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		{
			return;
		}

		// Scroll the rectangle of the entire buffer.
		scrollRect.Left = 0;
		scrollRect.Top = 0;
		scrollRect.Right = csbi.dwSize.X;
		scrollRect.Bottom = csbi.dwSize.Y;

		// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
		scrollTarget.X = 0;
		scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

		// Fill with empty spaces with the buffer's default text attribute.
		fill.Char.UnicodeChar = TEXT(' ');
		fill.Attributes = csbi.wAttributes;

		// Do the scroll
		ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

		// Move the cursor to the top left corner too.
		csbi.dwCursorPosition.X = cursorX;
		csbi.dwCursorPosition.Y = cursorY;

		SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
	}

	class EpsGreedyPolicy {
	public:
		EpsGreedyPolicy(std::unordered_map < ULL, std::vector<float>> Q, float epsilon)
			:
			Q(Q),
			eps(epsilon),
			rng(std::random_device()()),
			dist4(std::uniform_int_distribution<int>(0, 3)),
			unif(std::uniform_real_distribution<float>(0, 1))
		{}
		int Sample(ULL state) {
			// Samples an action following the policy given a state
			if (unif(rng) <= eps) { // Sample randomly
				return int(dist4(rng));
			}
			else { // Sample greedily
				auto Qvec = Q[state];
				int argmax = 0;
				float maxval = -1e9;
				for (size_t i = 0; i < 4; i++) {
					if (Qvec[i] > maxval) {
						argmax = i;
						maxval = Qvec[i];
					}
				}
				return argmax;
			}
		}
		void SetEpsilon(float epsilon) {
			eps = epsilon;
		}
		float GetEpsilon() {
			return eps;
		}
		std::unordered_map < ULL, std::vector<float>>& GetQ() {
			return Q;
		}
		void PrintQ() {
			for (auto item : Q) {
				std::cout << PBPos2Pos(item.first).first << "," << PBPos2Pos(item.first).second << ")" << ": (";
				for (size_t i = 0; i < 4; i++) {
					std::cout << item.second[i] << ',';
				}
				std::cout << ')' << std::endl;
			}
		}
	private:
		std::unordered_map < ULL, std::vector<float>> Q;
		float eps;
		std::mt19937 rng;
		std::uniform_int_distribution<int> dist4;
		std::uniform_real_distribution<float> unif;
	};

	struct RLField
	{
		struct FieldState
		{
			ULL PusherBoxPositions = 0; // Unique state (bits 1-16: Pusher cell number, bits 17-32 Box cell number)
			size_t n = 0;				// Step counter in episode
			std::vector<ULL> path;		// Path history of states
			float reward = 0;			// Reward received after step
			bool done = false;			// True if terminal state is reached
		};
		RLField(std::vector<std::vector<char>>& grid)
		{
			height = grid.size();
			width = grid[0].size();
			size = height * width;
			int startPosPusher;
			int startPosBall;
			for (size_t y = 0; y < height; y++)
			{
				for (size_t x = 0; x < width; x++)
				{
					char ch = 0;
					if (grid[y][x] == 'S') {
						ch = '.';
						startPosPusher = y * width + x;
					}
					else if (grid[y][x] == 'T') {
						ch = 'X';
					}
					else if (grid[y][x] == 'B') {
						ch = '.';
						startPosBall = y * width + x;
					}
					else {
						ch = grid[y][x];

					}
					fieldVec.push_back(ch);
					fieldMap.insert({ y * width + x, ch });
				}
			}
			fieldState = { Pos2PBPos({startPosPusher,startPosBall}), 0, {}, false };
			fieldState_0 = fieldState;
			fieldState.path.push_back(fieldState.PusherBoxPositions);

			// setup winapi consolescreen
			hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
			if (hStdOut == INVALID_HANDLE_VALUE) std::cout << "ERROR INITIALIZING WINAPI";

			/* Get the number of cells in the current buffer */
			if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) std::cout << "ERROR INITIALIZING WINAPI";
			cellCount = csbi.dwSize.X * csbi.dwSize.Y;

			/* Fill the entire buffer with spaces */
			if (!FillConsoleOutputCharacter(
				hStdOut,
				(TCHAR)' ',
				cellCount,
				homeCoords,
				&count
			)) std::cout << "ERROR INITIALIZING WINAPI";

			/* Fill the entire buffer with the current colors and attributes */
			if (!FillConsoleOutputAttribute(
				hStdOut,
				csbi.wAttributes,
				cellCount,
				homeCoords,
				&count
			)) std::cout << "ERROR INITIALIZING WINAPI";

			/* Move the cursor home */
			SetConsoleCursorPosition(hStdOut, homeCoords);
		}
		void DrawField() {
			auto pbPositions = PBPos2Pos(fieldState.PusherBoxPositions);
			for (size_t y = 0; y < height; y++)
			{
				for (size_t x = 0; x < width; x++)
				{
					if (y * width + x == pbPositions.first) {
						std::cout << 'p';
					}
					else if (y * width + x == pbPositions.second) {
						std::cout << 'O';
					}
					else {
						std::cout << fieldMap[y * width + x];
					}
				}
				std::cout << std::endl;
			}
		}
		void Step(int a) {
			auto pbPositions = PBPos2Pos(fieldState.PusherBoxPositions);
			fieldState.n++;
			fieldState.reward = -1.;
			if (a == 0) { // Stepping north
				//std::cout << "N: ";
				if (pbPositions.first <= width || fieldMap[pbPositions.first - width] == '#') { // blocked or end of field
					//std::cout << "Bumped into wall going North\n";
				}
				else if (pbPositions.second == pbPositions.first - width) { // box north of player
					if (pbPositions.first >= 2 * width && fieldMap[(pbPositions.first - 2 * width)] != '#') { // room to push
						fieldState.PusherBoxPositions = Pos2PBPos({ pbPositions.first - width, pbPositions.second - width });
						//std::cout << "Pushed box North\n";
					}
					else { // no room to push
						//std::cout << "No room to push box\n";
					}
				}
				else { // free walk North
					fieldState.PusherBoxPositions = Pos2PBPos({ pbPositions.first - width, pbPositions.second });
					//std::cout << "Took a step North\n";
				}
			}
			else if (a == 1) { // Stepping East
				//std::cout << "E: ";
				if ((pbPositions.first + 1) % width == 0 || fieldMap[pbPositions.first + 1] == '#') { // blocked or end of field
					//std::cout << "Bumped into wall going East\n";
				}
				else if (pbPositions.second == pbPositions.first + 1) { // box east of player
					if ((pbPositions.first + 1) % width > 1 && fieldMap[(pbPositions.first + 2)] != '#') { // room to push
						fieldState.PusherBoxPositions = Pos2PBPos({ pbPositions.first + 1, pbPositions.second + 1 });
						//std::cout << "Pushed box East\n";
					}
					else { // no room to push
						//std::cout << "No room to push box\n";
					}
				}
				else { // free walk East
					fieldState.PusherBoxPositions = Pos2PBPos({ pbPositions.first + 1, pbPositions.second });
					//std::cout << "Took a step East\n";
				}
			}
			else if (a == 2) { // Stepping South
				//std::cout << "S: ";
				if ((pbPositions.first / width) + 1 == height || fieldMap[pbPositions.first + width] == '#') { // blocked or end of field
					//std::cout << "Bumped into wall going South\n";
				}
				else if (pbPositions.second == pbPositions.first + width) { // box South of player
					if ((pbPositions.first / width) + 1 < height - 1 && fieldMap[(pbPositions.first + 2 * width)] != '#') { // room to push
						fieldState.PusherBoxPositions = Pos2PBPos({ pbPositions.first + width, pbPositions.second + width });
						//std::cout << "Pushed box South\n";
					}
					else { // no room to push
						//std::cout << "No room to push box\n";
					}
				}
				else { // free walk South
					fieldState.PusherBoxPositions = Pos2PBPos({ pbPositions.first + width, pbPositions.second });
					//std::cout << "Took a step South\n";
				}
			}
			else if (a == 3) { // Stepping West
				//std::cout << "W: ";
				if ((pbPositions.first + 1) % width == 1 || fieldMap[pbPositions.first - 1] == '#') { // blocked or end of field
					//std::cout << "Bumped into wall going West\n";
				}
				else if (pbPositions.second == pbPositions.first - 1) { // box West of player
					if ((pbPositions.first + 1) % width > 2 && fieldMap[(pbPositions.first - 2)] != '#') { // room to push
						fieldState.PusherBoxPositions = Pos2PBPos({ pbPositions.first - 1, pbPositions.second - 1 });
						//std::cout << "Pushed box West\n";
					}
					else { // no room to push
						//std::cout << "No room to push box\n";
					}
				}
				else { // free walk West
					fieldState.PusherBoxPositions = Pos2PBPos({ pbPositions.first - 1, pbPositions.second });
					//std::cout << "Took a step West\n";
				}
			}
			// Check terminal conditions
			if (fieldState.n > size * 3) { // Episode time limit (assuming this is because of deadlock)
				fieldState.done = true;
			}
			if (fieldMap[PBPos2Pos(fieldState.PusherBoxPositions).second] == 'X') { // Box on target
				fieldState.reward = 100;
				fieldState.done = true;
			}
		}
		void Reset() {
			fieldState = fieldState_0;
		}
		void DrawSolution(EpsGreedyPolicy& policy) {
			float eps_old = policy.GetEpsilon();
			policy.SetEpsilon(0.f);
			Reset();
			cls(hStdOut);
			std::cout << "Drawing solution using optimal policy learned using Q-learning\n\n";
			std::cout << "X = Target position\n";
			std::cout << "O = Box position\n";
			std::cout << "p = Pusher position\n";
			std::cout << ". = Accessible cell\n";
			std::cout << "X = Wall\n\n";
			DrawField();

			do {
				std::cin.get();
				SetConsoleCursorPosition(hStdOut, { 0,8 });
				Step(policy.Sample(fieldState.PusherBoxPositions));
				DrawField();
			} while (!fieldState.done);
			std::cout << "\nTarget reached. Steps:" << fieldState.n;
			std::cin.get();
			policy.SetEpsilon(eps_old);
		}
		std::vector<char> fieldVec;					// Flat vector of cell chararcters '.', 'B', 'p', 'X'
		std::unordered_map<int, char> fieldMap;		// Maps cell numbers to characters
		size_t width = 0;
		size_t height = 0;
		size_t size = 0;
		FieldState fieldState;
		FieldState fieldState_0;					// Start state, used when resetting to start state
	private:
		HANDLE                     hStdOut;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD                      count;
		DWORD                      cellCount;
		COORD                      homeCoords = { 0, 0 };
	};

	void RL_test(RLField& field) {

		while (!field.fieldState.done) {
			field.DrawField();
			char input; std::cin >> input;
			if (input == 'q') {
				return;
			}
			if (input == 'w') {
				field.Step(0);
			}
			if (input == 'd') {
				field.Step(1);
			}
			if (input == 's') {
				field.Step(2);
			}
			if (input == 'a') {
				field.Step(3);
			}
		}
		std::cout << "Target reached. Steps:" << field.fieldState.n;
		return;
	}

	void RL_solve(RLField& field, float epsilon = .15f, float discount_factor = 1.0f, float alpha = 0.5f, size_t num_trajectories = 100) {
		// Solves the box pusher problem using Q-learning
		// Create data struct for Q
		std::unordered_map < ULL, std::vector<float>> Q;	// State-action value function
		for (size_t i = 0; i < field.size; i++) {
			for (size_t j = 0; j < field.size; j++) {
				if (i != j && field.fieldMap[i] != '#' && field.fieldMap[j] != '#') {
					// valid states: pusher and box not on same cell and not on wall positions
					Q.insert({ Pos2PBPos({i,j}), {1.f,1.f,1.f,1.f} });
				}
			}
		}

		// Create policy
		EpsGreedyPolicy policy(Q, epsilon);

		// Main Q-learning algorithm
		for (size_t k = 0; k < num_trajectories; k++) {
			field.Reset();
			while (!field.fieldState.done) {
				ULL state = field.fieldState.PusherBoxPositions;
				int action = policy.Sample(state);
				field.Step(action);
				float r = field.fieldState.reward;
				ULL new_state = field.fieldState.PusherBoxPositions;
				float max_Q = *std::max_element(policy.GetQ()[new_state].begin(), policy.GetQ()[new_state].end());
				// Q value update step
				policy.GetQ()[state][action] += alpha * (r + discount_factor * max_Q - policy.GetQ()[state][action]);
			}
			if (field.fieldState.n < 100) {
				std::cout << k << "," << field.fieldState.n << "\n";
			}
		}
		policy.PrintQ();
		field.DrawSolution(policy);
		return;
	}
}

