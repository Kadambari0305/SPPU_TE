#include <iostream>
#include <map>
#include <list>
#include <queue>
using namespace std;

class CityGraph {
    map<string, list<string>> adj;

public:
    CityGraph() {
     

        addRoad("Pune", "Mumbai");
        addRoad("Pune", "Nashik");
        addRoad("Pune", "Kolhapur");

        addRoad("Mumbai", "Surat");
        addRoad("Mumbai", "Goa");

        addRoad("Nashik", "Aurangabad");
        addRoad("Nashik", "Indore");

        addRoad("Aurangabad", "Nagpur");
        addRoad("Nagpur", "Bhopal");

        addRoad("Indore", "Bhopal");
        addRoad("Indore", "Ahmedabad");

        addRoad("Surat", "Ahmedabad");

        addRoad("Goa", "Mangalore");
        addRoad("Kolhapur", "Goa");

        addRoad("Mangalore", "Bangalore");
        addRoad("Bangalore", "Hyderabad");

        addRoad("Hyderabad", "Nagpur");
    }

    void addRoad(string c1, string c2) {
        adj[c1].push_back(c2);
        adj[c2].push_back(c1);   
    }


    void DFS(string city, map<string, bool> &visited) {
        visited[city] = true;
        cout << city << "  ";

        for (string neighbor : adj[city]) {
            if (!visited[neighbor]) {
                DFS(neighbor, visited);
            }
        }
    }

    void startDFS(string start) {
        map<string, bool> visited;

        cout << "\nDepth First Search Traversal:\n";
        DFS(start, visited);
        cout << endl;
    }


    void BFSRecursive(queue<string> &q, map<string, bool> &visited) {

        if (q.empty())
            return;

        string city = q.front();
        q.pop();

        cout << city << "  ";

        for (string neighbor : adj[city]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }

        BFSRecursive(q, visited);
    }

    void startBFS(string start) {
        map<string, bool> visited;
        queue<string> q;

        visited[start] = true;
        q.push(start);

        cout << "\nBreadth First Search Traversal:\n";
        BFSRecursive(q, visited);
        cout << endl;
    }

    void showCities() {
        cout << "\nAvailable Cities in Network:\n";
        for (auto i : adj) {
            cout << "- " << i.first << endl;
        }
    }
};

int main() {

    CityGraph g;
    string start;

    cout << "==== CITY NAVIGATION SYSTEM ====\n";

    g.showCities();

    cout << "\nEnter Starting City: ";
    cin >> start;

    g.startDFS(start);
    g.startBFS(start);

    return 0;
}