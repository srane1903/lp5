#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

class Graph {
    int V; // number of vertices
    vector<vector<int> > adj; // adjacency listz

public:
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    // Add edge between two nodes
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected graph
    }

    // Parallel BFS
    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "\nBFS: ";

        while (!q.empty()) {
            int current = q.front();
            q.pop();
            cout << current << " ";

            // Parallel loop to check all neighbors
            #pragma omp parallel for
            for (int i = 0; i < adj[current].size(); i++) {
                int neighbor = adj[current][i];
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    // Helper function for DFS
    void DFSUtil(int current, vector<bool>& visited) {
        visited[current] = true;
        cout << current << " ";

        // Parallel loop for DFS
        #pragma omp parallel for
        for (int i = 0; i < adj[current].size(); i++) {
            int neighbor = adj[current][i];
            if (!visited[neighbor]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        DFSUtil(neighbor, visited);
                    }
                }
            }
        }
    }

    // Parallel DFS
    void DFS(int start) {
        vector<bool> visited(V, false);
        cout << "DFS: ";
        DFSUtil(start, visited);
        cout << endl;
    }
};

int main() {
    int V, E;

    cout << "Enter number of vertices: ";
    cin >> V;

    cout << "Enter number of edges: ";
    cin >> E;

    Graph g(V);

    cout << "Enter " << E << " edges (u v):" << endl;
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    g.BFS(0); // Start BFS from node 0
    g.DFS(0); // Start DFS from node 0

    return 0;
}


