#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>

using namespace std;

typedef pair<int, int> Edge;

struct Digraph{
    int V_;  
    int E_;                       
    vector<vector<Edge>> adjList_;  
    Digraph(int V){
        V_ = V;
        E_ = 0;
        adjList_= vector<vector<Edge>>(V_);
    }            
    void addEdge(int h, int t, int w);     
    vector<int> Dijkstra(int source);
    bool BellmanFord(vector<int> &dist, int source);
    void Johnson();
    void addSourceVertex();
};

void Digraph::addEdge(int t, int h, int w){ // h: head, t: tail, w: weight
    adjList_[t].push_back(make_pair(h, w)); 
    E_++;
}
