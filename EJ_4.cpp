#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;
struct Intervalo {
    int a;
    int b;
    int origin;
};
typedef pair<int, int> arista; // {destino, peso }
typedef vector< vector<arista> > Graph;

bool overlap(Intervalo& i, Intervalo& j) {
    if (//overlap completo
        i.a < j.a && j.a < i.b && i.b < j.b) {
        return true;
    }
    return false;
}

Graph armarGrafo(vector<Intervalo> &I, vector<bool> cont) {
    int n = I.size();
    // agrego el I_0 y el I_n+1
    Intervalo I_0 = { -2,-1,-1 };
    Intervalo I_n = { 2 * n + 1,2 * n + 2,n + 2 };
    I.push_back(I_0);
    I.push_back(I_n);
    sort(I.begin(), I.end(), [&](Intervalo i, Intervalo j) {
        return i.a < j.a;
        });
    //armamos "grafos" B y C
    Graph B(I.size());
    Graph C(I.size());
    //agrego a cada uno las aristas correspondientse
    for (int i = 0; i < I.size(); i++) {
        int noOverlap_minB = 2 * n + 2;
        for (int j = i + 1; j < I.size(); j++) {
            if(I[j].a>I[i].b){
                noOverlap_minB = min(noOverlap_minB, I[j].b);
            }
            //if(cont[j]) continue;
            if(cont[I[j].origin]) continue;
            if (overlap(I[i], I[j])) {
                //esta en B
                //cout << "Esta en B: " << i << " " << j << endl;
                B[i].push_back({ j,0 });
            }
            else {
                //no overlapea
                if (I[j].a < noOverlap_minB) {
                    
                    //esta en C
                    //cout << "Esta en C: " << i << " " << j << endl;
                    C[i].push_back({ j,0 });
                    // actualizar el minimo
                }
            }
        }
    }
    // armo el grafo final;
    Graph D(2 * (n + 2));
    // recorro cada arista de B
    for (int i = 0; i < B.size(); i++) { //por cada nodo
        for (arista e : B[i]) { //recorro cada uno de sus vecinos
            // cada arista va del nodo  i  al nodo e.first con peso e.second
            // agrego en el nodo out del i-esimo (i+n) a j-in como vecino con peso 1
            //cout << "Agrego a la salida del nodo " << i << " (" << i + n + 2 << ")" << "la arista a " << e.first << " con peso " << 1 << endl;
            D[i + n + 2].push_back({ e.first,1 });

        }
    }

    //recorro cada arista de C
    for (int i = 0; i < C.size(); i++) {
        for (arista e : C[i]) {
            int peso = (i == 0) ? 0 : 1;
            //cout << "Agrego a la salida del nodo " << i << " (" << i + n + 2 << ")" << "la arista a " << e.first << " con peso " << peso << endl;
            D[i].push_back({ e.first + n + 2,peso });
        }
    }

    //agrego cada arista entre i-in y i-out
    for (int i = 0; i < I.size(); i++) {
        D[i].push_back({ i + n + 2,0 });
    }

    return D;
}

vector<int> PathMinimoN2(Graph G, int s) {
    int n = G.size();
    vector<int> parents(n);
    vector<int> dist(n, n + 2);
    set<int> pendientes;
    //pongo todos los vertices como pendientes
    for (int i = 0; i < n; i++) {
        pendientes.insert(i);
    }
    //seteo la distancia de s a s como 0
    dist[s] = 0;
    while (pendientes.size() > 0) {
        int v = *pendientes.begin();
        //selecciono al mas cercano de los pendientes
        for (int w : pendientes) {
            if (dist[w] < dist[v]) v = w;
        }
        pendientes.erase(v);
        // calculo las distancias minimas y seteo padres
        for (arista e : G[v]) {
            if (dist[e.first] > dist[v] + e.second) {
                dist[e.first] = dist[v] + e.second;
                parents[e.first] = v;
                //cout << "Ahora " << v << " es padre de " << e.first << endl;
            }
        }
    }
    //devuelvo los padres;
    return parents;
}

vector<int> DAGSort(Graph G, int s){
    int n = G.size() /  2;
    vector<int> dist(G.size(),2*n+2);
    vector<int> parent(G.size(),-1);
    dist[s] = 0;
    for(int i = 0; i < n; i++){ // ta en topo
        for(arista e: G[i]){ // in
            if (dist[e.first] > dist[i] + e.second) {
                dist[e.first] = dist[i] + e.second;
                parent[e.first] = i;
            }
        }
        for(arista e: G[i+n]){ //out
            if (dist[e.first] > dist[i+n] + e.second) {
                dist[e.first] = dist[i+n] + e.second;
                parent[e.first] = i+n;
            }
        }
    }
    return parent;
}

int main() {
    //input -> n: cantidad de intervalos, n lineas con intervalos [A, B]
    int cantidadIntervalos; cin >> cantidadIntervalos;
    vector<Intervalo> I(cantidadIntervalos);
    pair<int,int> minA = {-1,2*cantidadIntervalos}; // {indice intervalo, valor a}
    pair<int,int> maxB = {-1,-1}; // {indice intevalo, valor b}
    for (int i = 0; i < I.size(); i++) {
        int a, b; cin >> a >> b;
        I[i] = {a,b};
        I[i].origin = i;
        if (a<minA.second){
            minA = {i, a};
        }
        if(b > maxB.second){

            maxB = {i,b};
        }
    }
    if(minA.first == maxB.first){
        cout << 2 << endl;
        vector<int> sol = {minA.first};
        sol.push_back((minA.first+1)%cantidadIntervalos);
        sort(sol.begin(), sol.end());
        for(int i: sol){
            cout << i << " ";
        }
        cout << endl;
        return 0;

    }

    vector<bool> contenido(cantidadIntervalos, false);
    for(int i = 0; i < I.size(); i++){
        for(int j = 0; j < I.size(); j++){
            if(I[i].a < I[j].a && I[j].b < I[i].b){
                contenido[j] = true;
            }
        }
    }
    /* for(bool b: contenido){
        cout << b << " ";
    }
    cout << endl; */

    Graph G = armarGrafo(I, contenido);
    vector<int> padres = DAGSort(G, 0);
    int i = G.size() - 1;
    vector<int> sol;
    vector<bool> enable(cantidadIntervalos, true);
    while (i > 0) {
        int miPadre = padres[i];
        int backup = miPadre;
        if (miPadre > cantidadIntervalos) {
            miPadre = miPadre - cantidadIntervalos - 2;
        }
        int temp = I[miPadre].origin;
        if (temp != -1 && enable[temp]){
            sol.push_back(temp);
            enable[temp] = false;
        }
        /* cout << "miPadre es: " << miPadre << endl;
        cout << "Yo soy: ";
        cout << I[miPadre].a << " " << I[miPadre].b << " " << I[miPadre].origin << endl; */
        i = backup;
    }
    sort(sol.begin(), sol.end());
    cout << sol.size() << endl;
    for(int i: sol){
        cout << i << " ";
    }
    cout << endl;
    return 0;
}