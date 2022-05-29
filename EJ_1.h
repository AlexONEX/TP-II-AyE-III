#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <utility>
#include <limits>
#include <tuple>
#include <vector>

using namespace std;

using Edge = pair<int, int>;

struct Graph{
    int V_;                      // No. of vertices.
    int M_;                      // No. of edges.
    vector<vector<int>> adjList_;   // Adjacency List.
    Graph(int V);            // Constructor
    void addEdge(int v, int w);     // Self explanatory.
    void BFS(int s); 
    tuple<vector<vector<int>>, bool, int> bfsMinPathCounter(int s);
    vector<int> getPath(vector<vector<int>> parents, int v);

};

Graph::Graph(int V){
    V_ = V;
    adjList_.resize(V_);
}

void Graph::addEdge(int v, int w){
    adjList_[v].push_back(w);
    adjList_[w].push_back(v);
}
