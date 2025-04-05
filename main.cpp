#include <iostream>
#include <stdlib.h>
#include <fstream> // For logging to CSV files
#include <chrono>
#include <direct.h> // For _mkdir on Windows

// Use Windows-specific headers if compiling on Windows
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#endif

#include "utils.h"

#define RAN_LIM 500000

double qMatrix[8][8], rMatrix[8][8], gammaLR = 0.8;
int max_index[8], available_acts[8];
int ran[RAN_LIM];

using namespace std;
using namespace std::chrono;

int main()
{
	int i, j;
	int initial_state, final_state = 7;
	int current_state, size_av_actions, action;
	double final_max = 0.0, scores[100000], score = 0.8;

	cout << "enter the initial state: ";
	cin >> initial_state;

	// Populate the random number array
	for (int i = 0; i < RAN_LIM; i++)
	{
		ran[i] = rand() % 8;
	}

	// Initialize Q-Matrix and R-Matrix
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			rMatrix[i][j] = -1.0;
			qMatrix[i][j] = 0.0;

			// Allowed transitions with reward 0
			if ((i == 0 && j == 1) || (i == 1 && j == 5) ||
				(i == 5 && j == 6) || (i == 5 && j == 4) ||
				(i == 1 && j == 2) || (i == 2 && j == 3) ||
				(i == 2 && j == 7) || (i == 4 && j == 7) ||
				(i == 1 && j == 4))
			{
				rMatrix[i][j] = 0.0;
			}
			if ((j == 0 && i == 1) || (j == 1 && i == 5) ||
				(j == 5 && i == 6) || (j == 5 && i == 4) ||
				(j == 1 && i == 2) || (j == 2 && i == 3) ||
				(j == 2 && i == 7) || (j == 4 && i == 7) ||
				(j == 1 && i == 4))
			{
				rMatrix[i][j] = 0.0;
			}
			// Transitions leading to goal state with reward 100
			if ((i == 2 && j == 7) || (i == 7 && j == 7) || (i == 4 && j == 7))
			{
				rMatrix[i][j] = 100.0;
			}
		}
	}

	cout << "R-Matrix: " << endl;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			cout << rMatrix[i][j] << "\t";
		}
		cout << "\n";
	}
	cout << "\n\n\n";
	cout << rMatrix[7][7];

	// Create LOGS directory if not exists
	_mkdir("LOGS");

	// Open CSV log files:
	ofstream logScore("LOGS/log_seq.csv");
	logScore << "Iteration,Score\n";

	ofstream logTime("LOGS/log_time.csv");
	logTime << "Iteration,IterationTime_ms,CumulativeTime_ms\n";

	ofstream logMem("LOGS/log_mem.csv");
	logMem << "Iteration,MemoryUsage_KB\n";

	// Record the overall training start time
	auto training_start = high_resolution_clock::now();
	double cumulative_time = 0.0;

	// Training loop
	for (i = 0; i < 500; i++)
	{
		auto iter_start = high_resolution_clock::now();

		cout << "\n--------------------------------------------------------------------------------\n";
		cout << "------------------------------------Training------------------------------------\n";
		cout << "--------------------------------------------------------------------------------\n";

		current_state = returnRandom(ran);
		size_av_actions = available_actions(current_state, available_acts, rMatrix);
		action = sample_next_action(size_av_actions, available_acts, ran);
		cout << "\nnext step: " << action;

		score = update(current_state, action, rMatrix, qMatrix, max_index, ran, gammaLR);
		scores[i] = score;
		cout << "\nScore: " << score;

		// Log score per iteration
		logScore << i << "," << score << "\n";

		// Measure iteration time
		auto iter_end = high_resolution_clock::now();
		auto iter_time = duration_cast<milliseconds>(iter_end - iter_start).count();
		cumulative_time += iter_time;
		logTime << i << "," << iter_time << "," << cumulative_time << "\n";

		// Measure memory usage
#ifdef _WIN32
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
		{
			long mem_usage = pmc.WorkingSetSize / 1024; // bytes to KB
			logMem << i << "," << mem_usage << "\n";
		}
#else
		struct rusage usage;
		getrusage(RUSAGE_SELF, &usage);
		long mem_usage = usage.ru_maxrss;
		logMem << i << "," << mem_usage << "\n";
#endif

		if (i != 499)
		{
			system("cls"); // Use system("cls") for Windows console clearing
		}
	}

	auto training_end = high_resolution_clock::now();
	auto total_training_time = duration_cast<milliseconds>(training_end - training_start).count();
	cout << "\n\nTotal Training time: " << total_training_time << " ms" << endl;

	logScore.close();
	logTime.close();
	logMem.close();

	// Normalization: Find the maximum Q value
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (final_max < qMatrix[i][j])
			{
				final_max = qMatrix[i][j];
			}
		}
	}

	// Print the normalized Q-matrix
	cout << "\n\n";
	cout << "--------------------------------------------------------------------------------\n";
	cout << "--------------------------------Trained Q-Matrix--------------------------------\n";
	cout << "--------------------------------------------------------------------------------\n";
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			cout << "\t" << (qMatrix[i][j] / final_max * 100.0);
		}
		cout << "\n";
	}

	// Determine the optimal path from the initial state to the goal state (state 7)
	int curr_state = initial_state;
	int visited[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int no_way = 0;
	int row_max = 0, max_ind = 0;

	cout << "Path: " << endl;
	while (visited[final_state] != 1)
	{
		cout << curr_state << " --> ";
		row_max = 0;
		max_ind = 0;
		for (int i = 0; i < 8; i++)
		{
			if (visited[i] == 0)
			{
				if (qMatrix[curr_state][i] > row_max)
				{
					max_ind = i;
					row_max = qMatrix[curr_state][i];
				}
			}
		}
		curr_state = max_ind;
		visited[max_ind] = 1;
		if (row_max == 0)
		{
			no_way = 1;
			break;
		}
		if (curr_state == final_state)
		{
			break;
		}
	}
	if (no_way == 1)
	{
		cout << "There's no way after this" << endl;
	}
	else
	{
		cout << curr_state << " is the shortest path" << endl;
	}

	return 0;
}
