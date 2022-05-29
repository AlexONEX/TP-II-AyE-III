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

struct Digraph{
    int V_;                         // No. of vertices.
    vector<vector<Edge>> adjList_;  // Adjacency List.
    Digraph(int V);            // Constructor
    void addEdge(int v, int w, int c);     // Self explanatory.
    vector<int> Dijkstra(int source);
    vector<int> BellmanFord(int source);
    vector<vector<int>> Johnson();
    void addSourceVertex();
};

void Digraph::addEdge(int v, int w, int c){
    adjList_[v].push_back(Edge(w, c));
}
