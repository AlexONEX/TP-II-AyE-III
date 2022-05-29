#include "EJ_3.h"
using namespace std;

vector<int> Digraph::BellmanFord(int source){
    vector<int> dist(V_, numeric_limits<int>::max());
    dist[source] = 0;
    for(int i = 0; i < V_ - 1; i++){
        for(int v = 0; v < V_; v++){
            for(int j = 0; j < adjList_[v].size(); j++){
                int w = adjList_[v][j].first;
                int c = adjList_[v][j].second;
                if(dist[v] != numeric_limits<int>::max() && dist[w] > dist[v] + c){
                    dist[w] = dist[v] + c;
                }
            }
        }
    }
    for(int v = 0; v < V_; v++){
        for(int j = 0; j < adjList_[v].size(); j++){
            int w = adjList_[v][j].first;
            int c = adjList_[v][j].second;
            if(dist[v] != numeric_limits<int>::max() && dist[w] > dist[v] + c){
                return vector<int>();
            }
        }
    }
    return dist;
}

vector<int> Digraph::Dijkstra(int source){
    vector<int> dist(V_, numeric_limits<int>::max());
    dist[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, source));
    while(!pq.empty()){     // O(E log V) 
        int v = pq.top().second;
        pq.pop();
        for(int j = 0; j < adjList_[v].size(); j++){
            int w = adjList_[v][j].first;
            int c = adjList_[v][j].second;
            if(dist[w] > dist[v] + c){
                dist[w] = dist[v] + c;
                pq.push(make_pair(dist[w], w));
            }
        }
    }
    return dist;
}

void Digraph::addSourceVertex(){
    int s = V_;
    V_++;
    adjList_.resize(V_);
    for(int v = 0; v < V_ - 1; v++){
        addEdge(s, v, 0);
    }
}

vector<vector<int>> Digraph::Johnson(){ 
    addSourceVertex();                          //Added s as the last vertex in the graph
    vector<vector<int>> dist(V_, vector<int>(V_, numeric_limits<int>::max()));
    vector<int> distances = BellmanFord(this->V_-1); 
    if(dist.size() == 0){
        return vector<vector<int>>();
    }
    Digraph G_n = {V_-1};                       //don't need to add source vertex
    for(int v = 0; v < V_-1; v++){
        for(auto w: adjList_[v]){
            int new_cost = w.second + distances[w.first] - distances[v];
            G_n.addEdge(v, w.first, new_cost);
        }
    }
    vector<vector<int>> distanceToAllNodes;
    for(int v = 0; v < G_n.V_; v++){
        vector<int> dist = G_n.Dijkstra(v);
        distanceToAllNodes.push_back(dist);
    }
    return distanceToAllNodes;
}


int	main()
{
    //Test BellmanFord
    Digraph g(5);
    //addEdges and test BellmanFord and Dijkstra

    return 0;
}
