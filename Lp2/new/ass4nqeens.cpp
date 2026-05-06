#include <iostream>
using namespace std;

#define MAX 39

int N;                         
int board[MAX][MAX];           


bool column[MAX];
bool diag1[2 * MAX];           
bool diag2[2 * MAX];          


void printSolution() {
    cout << "\nSolution:\n";

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 1)
                cout << " Q ";
            else
                cout << " . ";
        }
        cout << endl;
    }
}

//bbfun
bool solveNQueens(int row) {

    if (row == N) {
        printSolution();
        return true;
    }


    for (int col = 0; col < N; col++) {

        //branch and bound
        if (!column[col] &&
            !diag1[row - col + N - 1] &&
            !diag2[row + col]) {

            // Place queen
            board[row][col] = 1;
            column[col] = true;
            diag1[row - col + N - 1] = true;
            diag2[row + col] = true;

            // Recursive call
            if (solveNQueens(row + 1))
                return true;

            // Backtracking (remove queen)
            board[row][col] = 0;
            column[col] = false;
            diag1[row - col + N - 1] = false;
            diag2[row + col] = false;
        }
    }

    return false;
}


int main() {

    cout << "Enter number of Queens (N): ";
    cin >> N;

    
    for (int i = 0; i < N; i++) {
        column[i] = false;
        for (int j = 0; j < N; j++)
            board[i][j] = 0;
    }

    for (int i = 0; i < 2 * N; i++) {
        diag1[i] = false;
        diag2[i] = false;
    }

    // Solve the problem
    if (!solveNQueens(0))
        cout << "\nNo solution exists for N = " << N << endl;

    return 0;
}
