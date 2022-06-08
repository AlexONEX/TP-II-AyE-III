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
