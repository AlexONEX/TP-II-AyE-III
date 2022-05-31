#include <tuple>
#include "EJ_3.h"
using namespace std;

vector<int> Digraph::Dijkstra(int source){
    vector<int> dist(V_-1, numeric_limits<int>::max());
    dist[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;     
    //Pair (wieght, vertex), greater<pair<int, int>> is used to sort the queue in descending order
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

bool Digraph::BellmanFord(vector<int> &dist, vector<int> parents, int source){
    dist = {V_-1, numeric_limits<int>::max()};
    dist[source] = 0;
    vector<int> parents(V_, -1);
    for(int i = 0; i < V_ - 1; i++){
        for(int v = 0; v < V_; v++){
            for(int j = 0; j < adjList_[v].size(); j++){
                int w = adjList_[v][j].first;
                int c = adjList_[v][j].second;
                if(dist[v] != numeric_limits<int>::max() && dist[w] > dist[v] + c){
                    dist[w] = dist[v] + c;
                    parents[w] = v;
                }
            }
        }
    }
    for(int v = 0; v < V_; v++){
        for(int j = 0; j < adjList_[v].size(); j++){
            int w = adjList_[v][j].first;
            int c = adjList_[v][j].second;
            if(dist[v] != numeric_limits<int>::max() && dist[w] > dist[v] + c){
                return true;
            }
        }
    }
    return false;
}

void Digraph::addSourceVertex(){
    int s = V_;
    V_++;
    adjList_.resize(V_);
    for(int v = 0; v < V_ - 1; v++){
        addEdge(s, v, 0);
    }
}

void Digraph::Johnson(){ 
    addSourceVertex();                          //Added s as the last vertex in the graph
    vector<int> distances; vector<int> parents;
    bool existsNegCycle = BellmanFord(distances, parents, V_-1);  //O(V^2)
    if(existsNegCycle){
        return;
        //print negative weight cycle
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
        //Print dist
        //dist.clear();    //O(m) Space complexity
    };
}

int	main(){
    //Test BellmanFord
    Digraph g(5);
    g.addEdge(0,1,1);
    g.addEdge(0,2,1);
    g.addEdge(2,3,1);
    g.addEdge(3,0,-1);
    vector<int> dist; vector<int> parents;
    bool n = g.BellmanFord(dist, parents, 0);
    if(!n){
        cout << "Negative weight cycle" << endl;
    }
    else{
        for(int i = 0; i < dist.size(); i++){
            cout << dist[i] << " ";
        }
        cout << endl;
    }
    return 0;
}
