#include <iostream>
#include <vector>

using namespace std;

bool isSafe(int node, vector<vector<int>> &graph, vector<int> &color, int n, int col) {
    for (int k = 0; k < n; k++) {
        if (graph[node][k] == 1 && color[k] == col)
            return false;
    }
    return true;
}

bool solve(int node, vector<vector<int>> &graph, vector<int> &color, int m, int n) {
    if (node == n)
        return true;

    for (int col = 1; col <= m; col++) {
        if (isSafe(node, graph, color, n, col)) {
            color[node] = col;

            if (solve(node + 1, graph, color, m, n))
                return true;

            color[node] = 0;  // Backtrack
        }
    }

    return false;
}

int main() {
    int n, m;

    cout << "Enter number of vertices: ";
    cin >> n;

    cout << "Enter number of colors: ";
    cin >> m;

    vector<vector<int>> graph(n, vector<int>(n));

    cout << "Enter adjacency matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> graph[i][j];
        }
    }

    vector<int> color(n, 0);

    if (solve(0, graph, color, m, n)) {
        cout << "\nSolution Exists!\nColor Assignment:\n";
        for (int i = 0; i < n; i++)
            cout << "Vertex " << i << " -> Color " << color[i] << endl;
    } else {
        cout << "\nNo solution exists!";
    }

    return 0;
}