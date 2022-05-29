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
    addSourceVertex();      //Added s as the last vertex in the graph
    vector<vector<int>> dist(V_, vector<int>(V_, numeric_limits<int>::max()));
    vector<int> distances = BellmanFord(this->V_-1); 
    if(dist.size() == 0){
        return vector<vector<int>>();
    }
    /*
    for each vertex v ∈ G'.V
        set h(v) to the value of δ (s , v)
        computed by the Bellman-Ford algorithm
        for each edge (u, v) ∈ G'.E
            ŵ(u, v) = w( u, v ) + h( u) − h( v)
            let D = (duv) be a new n × n matrix
            for each vertex u ∈ G.V
                run DIJKSTRA( G , ŵ , u) to compute di( u, v)
                for all v ∈ G.V
            for each vertex v ∈ G.V
                minPath v->u = di(u,v) - h(v) + h(u), for all u ∈ G.V, 
    */ 
}


int	main()
{
    //Test BellmanFord
    Digraph g(5);
    //addEdges and test BellmanFord and Dijkstra

    return 0;
}
