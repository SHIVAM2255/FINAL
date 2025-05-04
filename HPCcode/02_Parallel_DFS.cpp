/*
 * Problem Statement:
 * Design and implement Parallel Depth First Search based on existing algorithms using OpenMP.
 * Use a Tree or an undirected graph for DFS.
 *
 * How to run:
 * 1. Open terminal in the directory containing the file
 * 2. Compile: g++ -fopenmp 02_Parallel_DFS.cpp -o 02_Parallel_DFS
 *    (if above not worked): g++ 02_Parallel_DFS.cpp -o 02_Parallel_DFS
 *    (General command): g++ -fopenmp fileName.cpp -o fileName or g++ fileName.cpp -o fileName
 * 3. Run: ./02_Parallel_DFS or .\02_Parallel_DFS
 */

#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

struct Graph
{
    int V;
    vector<vector<int>> adj;

    Graph(int V)
    {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected graph
    }

    void DFS(int start)
    {
        vector<bool> visited(V, false);
#pragma omp parallel
        {
#pragma omp single nowait
            {
                DFSUtil(start, visited);
            }
        }
        cout << endl;
    }

    void DFSUtil(int u, vector<bool> &visited)
    {
        visited[u] = true;
        cout << u << " ";

#pragma omp parallel for
        for (int i = 0; i < adj[u].size(); i++)
        {
            int v = adj[u][i];
            if (!visited[v])
            {
#pragma omp task
                {
                    DFSUtil(v, visited);
                }
            }
        }
    }
};

int main()
{
    // Example 1: Small graph for basic understanding
    cout << "Example 1: Small Graph (5 vertices)\n";
    Graph g1(5);
    g1.addEdge(0, 1);
    g1.addEdge(0, 2);
    g1.addEdge(1, 3);
    g1.addEdge(2, 4);
    cout << "DFS traversal starting from vertex 0: ";
    g1.DFS(0);
    cout << "\n";

    // Example 2: Slightly larger graph
    cout << "Example 2: Larger Graph (7 vertices)\n";
    Graph g2(7);
    g2.addEdge(0, 1);
    g2.addEdge(0, 2);
    g2.addEdge(1, 3);
    g2.addEdge(2, 4);
    g2.addEdge(3, 5);
    g2.addEdge(4, 6);
    cout << "DFS traversal starting from vertex 0: ";
    g2.DFS(0);

    cout << "\n";

    // User input for a graph
    int V;
    cout << "Enter the number of vertices: ";
    cin >> V;

    Graph g(V);

    int edges;
    cout << "Enter the number of edges: ";
    cin >> edges;

    cout << "Enter edges: (u, v)" << endl;
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    cout << "Parallel DFS traversal:" << endl;
    g.DFS(0);

    return 0;
}

