#include<bits/stdc++.h>
#include "graph.h"
#include <tuple>
#include <algorithm>
#include <vector>
#include <queue>
#include <utility>
#include <limits>

using namespace std;

Graph Dijkstra(Graph g, int source){
    vector<int> dist(g.V_, numeric_limits<int>::max());
    vector<int> prev(g.V_, -1);
    vector<bool> visited(g.V_, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[source] = 0;
    pq.push(make_pair(0, source));
    while(!pq.empty()){
        int u = pq.top().second;
        pq.pop();
        if(visited[u]) continue;
        visited[u] = true;
        for(auto e : g.edgeList_){
            if(e.first == u){
                int v = e.second;
                if(dist[v] > dist[u] + 1){
                    dist[v] = dist[u] + 1;
                    prev[v] = u;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }
    }
    return Graph(g.V_, prev);
}

Graph BellmanFord(Graph g, int source){
    vector<int> dist(g.V_, numeric_limits<int>::max());
    vector<int> prev(g.V_, -1);
    vector<bool> visited(g.V_, false);
    dist[source] = 0;
    for(int i = 0; i < g.V_ - 1; i++){
        for(auto e : g.edgeList_){
            int u = e.first;
            int v = e.second;
            if(dist[u] != numeric_limits<int>::max() && dist[u] + 1 < dist[v]){
                dist[v] = dist[u] + 1;
                prev[v] = u;
            }
        }
    }
    for(auto e : g.edgeList_){
        int u = e.first;
        int v = e.second;
        if(dist[u] != numeric_limits<int>::max() && dist[u] + 1 < dist[v]){
            dist[v] = numeric_limits<int>::max();
            prev[v] = u;
        }
    }
    return Graph(g.V_, prev);
}