#include <iostream>
using namespace std;

#define V 5
#define INF 9999

int minKey(int key[], bool mstSet[]) {
    int min = INF, index;

    for(int i=0;i<V;i++)
        if(!mstSet[i] && key[i] < min) {
            min = key[i];
            index = i;
        }

    return index;
}

int main() {
    int cost[V][V];

    cout << "Enter road construction cost matrix:\n";
    for(int i=0;i<V;i++)
        for(int j=0;j<V;j++)
            cin >> cost[i][j];

    int parent[V];
    int key[V];
    bool mstSet[V];

    for(int i=0;i<V;i++) {
        key[i] = INF;
        mstSet[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for(int count=0; count<V-1; count++) {
        int u = minKey(key, mstSet);
        mstSet[u] = true;

        for(int v=0; v<V; v++)
            if(cost[u][v] && !mstSet[v] && cost[u][v] < key[v]) {
                parent[v] = u;
                key[v] = cost[u][v];
            }
    }

    cout << "\nMinimum Cost Roads:\n";
    for(int i=1;i<V;i++)
        cout << parent[i] << " - " << i
             << "  Cost: " << cost[i][parent[i]] << endl;

    return 0;
}

/*goverment want min cost connection network cities
0 2 0 6 0
2 0 3 8 5
0 3 0 0 7
6 8 0 0 9
0 5 7 9 0*/

