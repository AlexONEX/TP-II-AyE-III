#include "EJ_3.h"
#define INF 0x3f3f3f3f
using namespace std;

bool Digraph::BellmanFord(vector<int> &dist, int source){
    bool isNegativeCycle = false;
    dist.assign(this->V_, INF); vector<int> parent;
    parent.assign(this->V_, -1);
    dist[source] = 0;
    parent[source] = source;
    bool f=true;
    int s= source;
    for(int q=0; q<V_; q++){  
        s = s%(V_);
        for(int i=0; i<this->adjList_[s].size(); i++){
            int t = this->adjList_[s][i].first;
            int w = this->adjList_[s][i].second;
            if(dist[s] != INF && dist[s] + w < dist[t]){
                dist[t] = dist[s] + w;
                parent[t] = s;
                f=false;
            }
        }
        s++;
    }
    int C=-1;
    for(int u=0; u<this->V_; u++){
        for(int j=0; j<this->adjList_[u].size(); j++){
            int v = this->adjList_[u][j].first;
            int w = this->adjList_[u][j].second;
            if(dist[v] != INF && dist[v] > dist[u] + w){
                C=v; u=this->V_; j=this->adjList_[u].size();
                isNegativeCycle = true;
                cout << "0" << endl;
            }
        }
    }
    if(C!=-1){
        vector<int> cycle;
        bool k=true;
        while(k){
            cycle.push_back(C);
            C = parent[C];
            if(C == cycle[0] || C == cycle.back()) k=false;
        }
        reverse(cycle.begin(), cycle.end());
        for(int i=0; i<cycle.size(); i++){
            cout<<cycle[i]<<" ";
        }
        cout<< cycle[0] << endl;
    }
    return isNegativeCycle;
}



void Digraph::addSourceVertex(){
    int s = V_;
    V_++;
    adjList_.resize(V_);
    for(int v = 0; v < V_ - 1; v++){
        addEdge(s, v, 0);
    }
}

vector<int> Digraph::Dijkstra(int source){
    vector<int> dist(V_, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;     
    pq.push(make_pair(0, source));
    dist[source] = 0;
    while(!pq.empty()){     // O(E log V) 
        int v = pq.top().second;
        pq.pop();
        for(auto i: adjList_[v]){
            int w = i.first; int weight = i.second;
            if(dist[w] > dist[v] + weight){
                dist[w] = dist[v] + weight;
                pq.push(make_pair(dist[w], w));
            }
        }
    }
    return dist;
}

void Digraph::Johnson(){ 
    vector<int> distances;
    bool ngc = BellmanFord(distances, 0);
    if(!ngc){
        addSourceVertex();   //Added s as the last vertex in the graph
        BellmanFord(distances, V_-1);      //O(E^2)
        Digraph G_n = {V_-1};              //don't need to add source vertex
        for(int v = 0; v < V_-1; v++){
            for(auto w: adjList_[v]){
                int new_cost = w.second + distances[v] - distances[w.first];
                //c'(v,w) = c(v,w) + h(v) - h(w)
                G_n.addEdge(v, w.first, new_cost);
            }
        }
        cout << "1" << endl;
        for(int v = 0; v < G_n.V_; v++){
            vector<int> dist = G_n.Dijkstra(v);
            //Undo the sum c'(v,w) = c(v,w) + h(v) - h(w)
            //Now c(v,w) = c'(v,w) - h(v) + h(w)
            vector<int> real_distances;
            for(int w = 0; w < G_n.V_; w++){
                if(dist[w] != INF){
                    int new_cost = dist[w] - distances[v] + distances[w];
                    real_distances.push_back(new_cost);
                }
                else{
                    real_distances.push_back(INF);
                }
            }
            for(int i=0; i<real_distances.size(); i++){
                if(real_distances[i] != INF){
                    cout << real_distances[i] << " ";
                }
                else{cout << "INF ";}
            }
        cout << endl;
        dist.clear();    //O(m) Space complexity
        };
    }
}

int	main(){
    /* Testing Dijkstra
    Digraph g = {4};
    g.addEdge(0,1,1);
    g.addEdge(0,2,4);
    g.addEdge(1,2,2);
    g.addEdge(1,3,6);
    g.addEdge(2,3,3);
    vector<int> dist = g.Dijkstra(0);
    for(int i = 0; i < g.V_; i++){
        cout << "Vertex " << i << ": " << dist[i] << endl;
    }*/
    /* Testing BellmanFord
    Digraph g = {5};
    vector<int> dist; 
    g.addEdge(0,1,-1);
    g.addEdge(1,2,2);
    g.addEdge(2,3,3);
    g.addEdge(3,4,-3);
    g.addEdge(4,0,-3);
    g.BellmanFord(dist, 0);
    */
    // /* Testing Johnson
    Digraph g = {4};
    g.addEdge(0,1,3);
    g.addEdge(1,2,2);
    g.addEdge(2,3,6);
    g.addEdge(3,0,-1);
    g.addEdge(1,3,-1);
    g.Johnson();
    // */
   return 0;
}
