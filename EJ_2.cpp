/*
Ejercicio 2

El grafo G(M) asociado a una matriz M ∈ {0,1}ᵐˣⁿ tiene un vértice por cada posición de M que tiene valor 1, donde dos vértices de G(M) son adyacentes si y solo si sus posiciones son adyacentes en M. Diseñar un algoritmo que, dada una matriz M, determine la cantidad de componentes conexas de G(M). El algoritmo debe consumir espacio O(n)* y tiempo O(mnα⁻¹(nm)) donde α es la función de Ackerman.
* Sin contar el espacio que ocupa la matriz M en el disco. Solo contar lo que se consume de memoria. Notar que M no se puede cargar enteramente en memoria, sino que solo se puede leer una cantidad constante de las en simultáneo.

Descripción de una instancia.
La instancia empieza con una línea con los valores m y n que representan la cantidad de las y columnas de la matriz M. Luego sigen m líneas, cada una de las cuales tiene n valores en {0,1}. El j-ésimo valor de la i-ésima fila se corresponde a Mᵢⱼ.

Descripción de la salida.
Una línea con la cantidad de componentes conexas de G(M).

Entrada de ejemplo
4 10
0 0 1 1 1 1 0 0 1 1
0 1 1 0 0 1 1 1 1 1
0 0 1 1 1 1 0 1 0 0
1 0 0 0 0 0 0 0 1 1

Salida esperada de ejemplo
3
*/


/*
Planteo de solución

Se sabe que cualquier subgrafo inducido o compresión de un grafo planar también resulta planar.
Se sabe que un grafo planar de n vertices y e aristas cumple que: e ≤ 3n.
Se sabe que G(M) es subgrafo inducido de un grafo reticular planar.

Se propone el siguiente invariante para un ciclo que procesó la matriz hasta fila i inclusive:
Sea Rᵢ el subgrafo inducido por G(M) al quitar todos los vertices correspondientes a filas de M posteriores a la fila i,
cada vértice de la fila i tiene asignado un identificador exclusivo a la componente conexa en Rᵢ a la que pertenece.
Se conoce el conteo Kᵢ de componentes conexas de G(M) contenidas en Rᵢ, que incialmente vale 0.

Con dicho invariante, se propone el siguiente método para procesar la fila i+1:
Sea F el subgrafo inducido por G(M) al quitar todos los vertices no correspondientes a la fila por procesar,
asignar a cada vértice de la fila i+1 un identificador exclusivo a la componente conexa en F a la que pertence.
Sea I el subgrafo inducido por G(M) al quitar todos los vertices no correspondientes a las filas i e i+1, y sea C una compresión de I al 
reemplazar cada conjunto de vértices con mismo identificador y misma fila por un único vértice, realizar un único recorrido del grafo C para 
identificar sus componentes conexas.
Incrementar el conteo K según la cantidad de vértices de C en la fila i que estén aislados.
Almacenar para cada vertice de G(M) en la fila i+1, un identificador exclusivo a la componente conexa en C a la que pertence el vértice dentro del 
cual fue comprimido, pues será de utilidad en la próxima iteración.

El recorrido del grafo C es realizable en complejidad temporal y espacial de orden n, siendo n el largo de cada fila, pues se tiene que C es un 
grafo planar con no más de 2*⌈n/2⌉ vertices, lo cual implica que como máximo alberga 3*2*⌈n/2⌉ aristas.
Tras iterar sobre las m filas de la matriz, se logra una complejidad temporal de O(n*m)

Como para efectuar cada iteración solo es necesario conocer los identificadores de las componentes conexas en Rᵢ de los vértices correspondientes a la última fila procesada, cuya cantidad no puede exceder ⌈n/2⌉, se logra una complejidad espacial O(n) para todo el algoritmo, sin icluir el espacio donde inicie almacenada la matriz de entrada M.
*/

#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

// Lee desde la entrada estándar, y devuelve en un vector,
// una secuencia con hileras de 1s separadas por por 0s
vector<int> leer_fila(int columnas) {
	vector<int> fila(columnas);
	for (int c = 0; c < columnas; c++) {
		int e; cin >> e; fila[c] = e;
	}
	return fila;
}

// Toma un vector con hileras de 1s separadas por por 0s
// Escribe el índice de hilera sobre cada 1 en el vector
// Devuelve la cantidad de hileras
int marcar_hileras(vector<int> & fila) {
	int hileras = fila[0];
	for (int c = 1; c < fila.size(); c++) {
		if (fila[c] == 0) continue;
		if (fila[c-1] == 0) hileras++;
		fila[c] = hileras;
	}
	return hileras;
}

void imprimir_fila(const vector<int> & fila) {
	for (int c : fila) {
		if (c) cout << char('A'+((c-1)%26));
		else cout << ' ';
	}
}


// Recorre una componente conexa y marca vertices visitados con un identificador
void recorrer(bool es_hilera, int indice, int identificador,
		vector<int>* marcas, const vector<vector<int>>* vecindarios) {
	// Si aún no fue visitado, marcar y recorrer vecinos
	int & identificador_actual = marcas[es_hilera][indice];
	if (identificador_actual == 0) {
		identificador_actual = identificador;
		for (int vecino : vecindarios[es_hilera][indice]) {
			recorrer(!es_hilera, vecino, identificador,	marcas, vecindarios);
		}
	} else {
		assert(identificador_actual == identificador);  // Está en la componente
	}
}

int main() {
	// Leer tamaño de entrada
	int filas, columnas;
	cin >> filas >> columnas;
		
	// Leer y procesar filas
	int componentes_cerradas = 0, componentes_abiertas = 0;
	vector<int> fila_previa(columnas, 0);
	for (int f = 0; f < filas; f++) {
		vector<int> fila_actual = leer_fila(columnas);
		int hileras = marcar_hileras(fila_actual);
		assert(hileras <= (columnas+1)/2);               // Condición disconexas
		
		// Buscar adyacencias entre componentes abiertas e hileras de nueva fila
		vector<vector<int>> vecindarios[] = {
			vector<vector<int>>(componentes_abiertas+1), // Componentes abiertas
			vector<vector<int>>(hileras+1)               // Hileras en esta fila
		};
		int conteo_adyacencias = 0;
		for (int c = 0; c < columnas; c++) {
			int componente = fila_previa[c], hilera = fila_actual[c];
			if (componente == 0 || hilera == 0) continue;
			// Considerar la adyacencia solo en caso de ser la primer ocurrencia
			if (vecindarios[0][componente].empty() ||
					vecindarios[0][componente].back() < hilera) {
				vecindarios[0][componente].push_back(hilera);
				vecindarios[1][hilera].push_back(componente);
				conteo_adyacencias++;
			}
		}
		assert(conteo_adyacencias <= (columnas+1)/2*3);  // Condición planaridad
		
		// Identificar componente representante de cada hilera (recorrido grafo)
		vector<int> representantes[] = {
			vector<int>(componentes_abiertas+1, 0),      // Componentes abiertas
			vector<int>(hileras+1, 0)                    // Hileras en esta fila
		};
		int componentes = 0;
		for (int hilera = 1; hilera <= hileras; hilera++) {  // Revisarlas todas
			if (representantes[1][hilera] != 0) continue;  // Identificada antes
			componentes++;                                   // Componente nueva
			recorrer(true, hilera, componentes, representantes, vecindarios);
		}
		componentes_abiertas = componentes;
		assert(componentes_abiertas <= (columnas+1)/2);  // Condición disconexas
		
		// Contabilizar componentes que se cierran
		for (int i = 1; i < representantes[0].size(); i++) {
			if (representantes[0][i] == 0) componentes_cerradas++;
		}
		
		// Guardar en la fila los nuevos identificadores de componentes abiertas
		for (int c = 0; c < columnas; c++) {
			fila_previa[c] = representantes[1][fila_actual[c]];
		}
		
		// Imprimir desarrollo
		bool verbose = false;
		if (verbose) {
			cout << componentes_cerradas << "\t|";
			imprimir_fila(fila_previa);
			cout << '|' << endl;
		}	
	}
	componentes_cerradas += componentes_abiertas;  // Componentes en última fila
	
	// Imprimir resultado
	cout << componentes_cerradas << endl;
}

