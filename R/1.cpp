#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// Class representing an undirected graph
class Graph {
    int numVertices;
    vector<vector<int>> adjList;

public:
    Graph(int n) : numVertices(n), adjList(n) {}

    // Function to add an edge to the graph
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    // Parallel BFS traversal
    void parallelBFS(int startVertex) {
        vector<bool> visited(numVertices, false);  // Vector to keep track of visited vertices
        queue<int> queue;

        // Push the start vertex to the queue
        queue.push(startVertex);
        visited[startVertex] = true;

        while (!queue.empty()) {
            // Process a vertex from the queue
            int currentVertex;
            #pragma omp critical
            {
                currentVertex = queue.front();
                queue.pop();
            }

            cout << currentVertex << " ";

            // Enqueue all the adjacent vertices that are not visited
            #pragma omp parallel for
            for (int neighbor : adjList[currentVertex]) {
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            queue.push(neighbor);
                            visited[neighbor] = true;
                        }
                    }
                }
            }
        }
    }

    // Parallel DFS traversal
    void parallelDFS(int startVertex) {
        vector<bool> visited(numVertices, false);  // Vector to keep track of visited vertices
        stack<int> stack;

        // Push the start vertex to the stack
        stack.push(startVertex);

        while (!stack.empty()) {
            // Pop a vertex from the stack
            int currentVertex;
            #pragma omp critical
            {
                currentVertex = stack.top();
                stack.pop();
            }

            // If the vertex is not visited, mark it as visited and process it
            if (!visited[currentVertex]) {
                #pragma omp critical
                {
                    if (!visited[currentVertex]) {
                        visited[currentVertex] = true;
                        cout << currentVertex << " ";
                    }
                }

                // Push all the adjacent vertices that are not visited onto the stack
                #pragma omp parallel for
                for (int neighbor : adjList[currentVertex]) {
                    if (!visited[neighbor]) {
                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                stack.push(neighbor);
                            }
                        }
                    }
                }
            }
        }
    }
};

// Example usage
int main() {
    // Create an undirected graph
    Graph graph(6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);
    graph.addEdge(3, 5);

    cout << "Parallel BFS traversal starting from vertex 0:" << endl;
    graph.parallelBFS(0);
    cout << endl;

    cout << "Parallel DFS traversal starting from vertex 0:" << endl;
    graph.parallelDFS(0);
    cout << endl;

    return 0;
}
