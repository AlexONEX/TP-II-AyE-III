#include<bits/stdc++.h>
#include <tuple>
#include <vector>
#include <list>
#include <queue>
#include <utility>
#include <limits>
#include "EJ_1.h"
using namespace std;

int parent(vector<vector<int>> parents, int v){
    if(parents[v].size() == 0){
        return -1;
    }
    return parents[v][0];
}

vector<int> getPath(vector<vector<int>> parents, int v){
    vector<int> path;
    while(parents[v].size() > 0){
        path.push_back(v);
        v = parents[v][0];
    }
    path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

tuple<vector<vector<int>>, bool, int> Graph::bfsMinPathCounter(int s){   
    vector<bool> visited(this->V_, false);
    vector<vector<int>> pred(this->V_, vector<int>());
    vector<int> dist(this->V_, numeric_limits<int>::max());
    vector<int> paths(this->V_, 0);
    bool isGeodetic = true;
    queue<int> q;
    q.push(s);
    visited[s] = true;
    dist[s] = 0; paths[s] = 1;
    while(!q.empty()){
        int v = q.front(); q.pop();
        for(auto w: this->adjList_[v]){
            if(!visited[w]){
                visited[w] = true;
                pred[w].push_back(v);   
                q.push(w);
            }
            if(dist[w] > dist[v] + 1){  
                dist[w] = dist[v] + 1;
                paths[w] = paths[v];
            }
            else if(dist[w] == dist[v]+1){  
                paths[w] += paths[v];
                pred[w].push_back(v); 
                isGeodetic = false;
            }
        }
    }
    return make_tuple(pred, isGeodetic, s);
}

void driver(Graph g){
    vector<tuple<vector<vector<int>>, bool, int>> trees;    //Almaceno predecesores, si es geodetic, y la raíz.
    int check=0;
    for(int i=0; i<g.V_; i++){
        trees.push_back(g.bfsMinPathCounter(i));
        check+=get<1>(trees[i]);
    }
    if(check==g.V_){
        cout << "1" << endl;
        for(auto tree: trees){
            vector<vector<int>> pred = get<0>(tree);
            for(int node=0; node<g.V_; node++){
                int grand = parent(pred, node);
                if(grand == -1){
                    cout << get<2>(tree) << " ";
                }
                else{
                    cout << grand << " ";
                }
            }cout << endl;     
        }  
    }
    else{
        cout << "0" << endl;
        for(auto tree:trees){
            if(!get<1>(tree)){
                vector<int> nodes;
                for(int i=0; i<get<0>(tree).size(); i++){
                    if(get<0>(tree)[i].size() > 1){
                        nodes.push_back(i);
                    }
                }
                for(auto node: nodes){
                    int count = 2;
                    for(auto p: get<0>(tree)[node]){
                        vector<int> path = getPath(get<0>(tree), p);
                        path.push_back(node);
                        for(int i=0; i<path.size(); i++){
                            cout << path[i] << " ";
                        }
                        cout << endl; count--; if(count == 0){return;}  //Version fea para imprimir como piden
                    }
                }
            }
        }
    }
}

int main()
{
    int V = 5;
    Graph g(V);
    g.addEdge(4, 0); g.addEdge(1,4); g.addEdge(2,4); g.addEdge(3,4);
    driver(g);

    return 0;
}

