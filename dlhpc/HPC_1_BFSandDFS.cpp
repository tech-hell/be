#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// Function to perform BFS in parallel
void parallelBFS(vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);

    // Queue for BFS traversal
    queue<int> q;
    q.push(start);

    while (!q.empty()) {
        int size = q.size();

        // Parallelize the exploration of nodes at this level
        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            int curr;
            #pragma omp critical
            {
                curr = q.front();
                q.pop();
            }

            // Process the current node if not visited
            if (!visited[curr]) {
                visited[curr] = true;
                cout << curr << " ";

                // Explore neighbors of the current node
                for (int neighbor : graph[curr]) {
                    if (!visited[neighbor]) {
                        // Mark neighbor as visited and add to the queue
                        #pragma omp critical
                        {
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
    }
}

// Function to perform DFS recursively in parallel
void parallelDFS(vector<vector<int>>& graph, vector<bool>& visited, int curr) {
    // Process the current node if not visited
    if (!visited[curr]) {
        visited[curr] = true;
        cout << curr << " ";

        // Explore neighbors of the current node in parallel
        #pragma omp parallel for
        for (int i = 0; i < graph[curr].size(); ++i) {
            int neighbor = graph[curr][i];
            parallelDFS(graph, visited, neighbor);
        }
    }
}

int main() {
    int n = 6;
    vector<vector<int>> graph(n);

    // Define the graph (adjacency list)
    graph[0] = {1, 2};
    graph[1] = {0, 3, 4};
    graph[2] = {0, 4};
    graph[3] = {1, 5};
    graph[4] = {1, 2};
    graph[5] = {3};

    // Perform BFS starting from node 0 in parallel
    cout << "Parallel BFS: ";
    parallelBFS(graph, 0);
    cout << endl;

    // Perform DFS starting from node 0 recursively and in parallel
    cout << "Parallel DFS: ";
    vector<bool> visited(n, false);
    parallelDFS(graph, visited, 0);
    cout << endl;

    return 0;
}