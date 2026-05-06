#include <iostream>
using namespace std;

#define V 5
#define INF 9999

int minDistance(int dist[], bool visited[]) {
    int min = INF, index;

    for(int i=0;i<V;i++)
        if(!visited[i] && dist[i] < min) {
            min = dist[i];
            index = i;
        }

    return index;
}

int main() {
    int network[V][V];   

    cout << "Enter network delay matrix:\n";
    for(int i=0;i<V;i++)
        for(int j=0;j<V;j++)
            cin >> network[i][j];

    int dist[V];
    bool visited[V];

    for(int i=0;i<V;i++) {
        dist[i] = INF;
        visited[i] = false;
    }

    int source;
    cout << "Enter source router: ";
    cin >> source;

    dist[source] = 0;

    for(int count=0; count<V-1; count++) {
        int u = minDistance(dist, visited);
        visited[u] = true;

        for(int v=0; v<V; v++)
            if(!visited[v] &&
               network[u][v] &&
               dist[u] + network[u][v] < dist[v]) {

                dist[v] = dist[u] + network[u][v];
            }
    }

    cout << "\nShortest Network Delays:\n";
    for(int i=0;i<V;i++)
        cout << "Router " << i
             << " Delay: " << dist[i] << endl;

    return 0;
}
/*-------
finding path for internet to all routers(dijkstar)
0 10 0 5 0
0 0 1 2 0
0 0 0 0 4
0 3 9 0 2
7 0 6 0 0*/

