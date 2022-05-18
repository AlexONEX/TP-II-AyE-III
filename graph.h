#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <utility>
#include <limits>
using namespace std;

using Edge = pair<int, int>;

class Graph{
public:
    int V_;    
    int M_;
    list<Edge> edgeList_;  
    Graph(int V, list<Edge> edgeList);
    void addEdge(int v, int w);
    void BFS(int s); 
    tuple<vector<vector<int>>, bool, int> bfsMinPathCounter(int s);
    vector<int> neighbors(int v, Graph G);
};

