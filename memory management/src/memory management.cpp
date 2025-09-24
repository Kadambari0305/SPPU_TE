//============================================================================
// Name        : memory.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <vector>
using namespace std;

// Function to take user input for a vector
void inputVector(vector<int>& vec, const string& prompt) {
    cout << prompt;
    for (int& val : vec) {
        cin >> val;
    }
}

// First Fit
void firstFit(vector<int> blocks, const vector<int>& processes) {
    cout << "\n--- First Fit ---\n";
    vector<bool> allocated(blocks.size(), false);

    for (int i = 0; i < processes.size(); ++i) {
        bool done = false;
        for (int j = 0; j < blocks.size(); ++j) {
            if (!allocated[j] && blocks[j] >= processes[i]) {
                cout << "Process " << i + 1 << " of size " << processes[i]
                     << "KB allocated to block " << j + 1 << endl;
                allocated[j] = true;
                done = true;
                break;
            }
        }
        if (!done) {
            cout << "Process " << i + 1 << " of size " << processes[i] << "KB not allocated\n";
        }
    }
}

// Best Fit
void bestFit(vector<int> blocks, const vector<int>& processes) {
    cout << "\n--- Best Fit ---\n";
    vector<bool> allocated(blocks.size(), false);

    for (int i = 0; i < processes.size(); ++i) {
        int bestIdx = -1;
        for (int j = 0; j < blocks.size(); ++j) {
            if (!allocated[j] && blocks[j] >= processes[i]) {
                if (bestIdx == -1 || blocks[j] < blocks[bestIdx]) {
                    bestIdx = j;
                }
            }
        }
        if (bestIdx != -1) {
            cout << "Process " << i + 1 << " of size " << processes[i]
                 << "KB allocated to block " << bestIdx + 1 << endl;
            allocated[bestIdx] = true;
        } else {
            cout << "Process " << i + 1 << " of size " << processes[i] << "KB not allocated\n";
        }
    }
}

// Worst Fit
void worstFit(vector<int> blocks, const vector<int>& processes) {
    cout << "\n--- Worst Fit ---\n";
    vector<bool> allocated(blocks.size(), false);

    for (int i = 0; i < processes.size(); ++i) {
        int worstIdx = -1;
        for (int j = 0; j < blocks.size(); ++j) {
            if (!allocated[j] && blocks[j] >= processes[i]) {
                if (worstIdx == -1 || blocks[j] > blocks[worstIdx]) {
                    worstIdx = j;
                }
            }
        }
        if (worstIdx != -1) {
            cout << "Process " << i + 1 << " of size " << processes[i]
                 << "KB allocated to block " << worstIdx + 1 << endl;
            allocated[worstIdx] = true;
        } else {
            cout << "Process " << i + 1 << " of size " << processes[i] << "KB not allocated\n";
        }
    }
}

// Next Fit
void nextFit(vector<int> blocks, const vector<int>& processes) {
    cout << "\n--- Next Fit ---\n";
    vector<bool> allocated(blocks.size(), false);
    int lastPos = 0;

    for (int i = 0; i < processes.size(); ++i) {
        bool done = false;
        int count = 0;
        int j = lastPos;

        while (count < blocks.size()) {
            if (!allocated[j] && blocks[j] >= processes[i]) {
                cout << "Process " << i + 1 << " of size " << processes[i]
                     << "KB allocated to block " << j + 1 << endl;
                allocated[j] = true;
                lastPos = (j + 1) % blocks.size();
                done = true;
                break;
            }
            j = (j + 1) % blocks.size();
            count++;
        }

        if (!done) {
            cout << "Process " << i + 1 << " of size " << processes[i] << "KB not allocated\n";
        }
    }
}

int main() {
    int numBlocks, numProcesses;

    cout << "Enter number of memory blocks: ";
    cin >> numBlocks;
    vector<int> memoryBlocks(numBlocks);
    inputVector(memoryBlocks, "Enter sizes of memory blocks (in KB): ");

    cout << "Enter number of processes: ";
    cin >> numProcesses;
    vector<int> processSizes(numProcesses);
    inputVector(processSizes, "Enter sizes of processes (in KB): ");

    firstFit(memoryBlocks, processSizes);
    bestFit(memoryBlocks, processSizes);
    worstFit(memoryBlocks, processSizes);
    nextFit(memoryBlocks, processSizes);

    return 0;
}
