#include "algoritmosGrafo.h"
using namespace std;

//Constructor
AlgoritmosGrafo::AlgoritmosGrafo(Grafo& grafo) : grafo(grafo){}

// --- METODOS DE ALGORITMOS (Devuelven un vector con la ruta generada) ---

// --- Nivel 1: BFS/Busqueda en Anchura ---
vector<int> AlgoritmosGrafo::busquedaAnchura(int inicio, int destino) {
    int cantidadNodos = grafo.getCantidadNodos();
    queue<int> cola;                                // Utilizar principio FIFO para implementar BFS
    vector<bool> visitados(cantidadNodos, false);

    // Para reconstruir MST y rutas
    vector<int> predecesores(cantidadNodos, -1);    // Un -1 indica que nodo no tiene predecesor o padre

    visitados[inicio] = true;                       // Marcar nodo inicio como visitado
    cola.push(inicio);

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();

        if (actual == destino) break;               // Salir del ciclo al llegar al destino

        // Exploracion por capas del BFS
        for (const auto& vecino : grafo.obtenerAdyacentes(actual)) {
            int nodoVecino = vecino.first;          // Ignorar el peso y utilizar solo ID de vecinos
            if (!visitados[nodoVecino]) {
                visitados[nodoVecino] = true;       // Marcar nodo vecino como visitado
                predecesores[nodoVecino] = actual;  // Almacenar nodo anterior en vector de predecesores
                cola.push(nodoVecino);              // Insertar nodo vecino a la cola
            }
        }
    }

    // Si no se alcanzó el destino, retornar lista vacía
    if (!visitados[destino]) return {}; 

    // --- Reconstruir la ruta desde destino hasta inicio ---
    vector<int> ruta;          

    // Usar vector de predecesores para reconstruir ruta desde destino a inicio
    for (int nodoActual = destino; nodoActual != -1; nodoActual = predecesores[nodoActual]) {
        ruta.push_back(nodoActual);
    }

    // Invertir ruta hecha para tener la correcta
    reverse(ruta.begin(), ruta.end());
    return ruta;
}

// --- Nivel 1: DFS/Busqueda en Profundidad ---
vector<int> AlgoritmosGrafo::busquedaProfundidad(int inicio, int destino) {
    int cantidadNodos = grafo.getCantidadNodos();
    stack<int> pila;                                // Utilizar principio LIFO para implementar DFS
    vector<bool> visitados(cantidadNodos, false);

    // Para reconstruir rutas
    vector<int> predecesores(cantidadNodos, -1);    // Un -1 indica que nodo no tiene predecesor o padre
    
    pila.push(inicio);

    while (!pila.empty()) {
        int actual = pila.top();
        pila.pop();

        if (!visitados[actual]) {   
            visitados[actual] = true;

            if (actual == destino) break;

            auto vecinos = grafo.obtenerAdyacentes(actual);
            for (int i = vecinos.size() - 1; i >= 0; i--) {
                int nodoVecino = vecinos[i].first;
                if (!visitados[nodoVecino] && predecesores[nodoVecino] == -1) {
                    predecesores[nodoVecino] = actual;
                    pila.push(nodoVecino);
                }
            }
        }
    }

    // Si no se alcanzó el destino, retornar lista vacía
    if (!visitados[destino]) return {}; 

    // --- Reconstruir la ruta desde destino hasta inicio ---
    vector<int> ruta;

    // Usar vector de predecesores para reconstruir ruta desde destino a inicio
    for (int nodoActual = destino; nodoActual != -1; nodoActual = predecesores[nodoActual]) {
        ruta.push_back(nodoActual);
    }

    // Invertir ruta hecha para tener la correcta
    reverse(ruta.begin(), ruta.end());
    return ruta;
}

// --- Nivel 2: Greedy/Prim ---
vector<int> AlgoritmosGrafo::algoritmoPrim(int inicio, int destino) {
    int cantidadNodos = grafo.getCantidadNodos();
    vector<bool> visitados(cantidadNodos, false);   // Para reconstruir MST y rutas
    vector<int> predecesores(cantidadNodos, -1);    // Un -1 indica que nodo no tiene predecesor o padre

    // Cola de prioridad o min-heap: (pesoArista, nodoPadre, nodoHijo)
    priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<>> colaPrioridad;

    // Construir el MST desde el nodo inicio (Utilizando algoritmo de Prim)
    visitados[inicio] = true;                    // Marcar nodo de inicio como visitado
    for (auto [vecino, peso] : grafo.obtenerAdyacentes(inicio)) {
        colaPrioridad.push({peso, inicio, vecino});
    }
    
    // Ciclo principal para crear MST
    while (!colaPrioridad.empty()) {
        auto [pesoArista, nodoPadreActual, nodoActual] = colaPrioridad.top();
        colaPrioridad.pop();

        if (visitados[nodoActual]) continue;

        visitados[nodoActual] = true;
        predecesores[nodoActual] = nodoPadreActual;

        for (auto [vecino, pesoVecino] : grafo.obtenerAdyacentes(nodoActual)) {
            if (!visitados[vecino]) {
                colaPrioridad.push({pesoVecino, nodoActual, vecino});
            }
        }
    }

    // Usar vector de predecesores para reconstruir ruta desde destino a inicio
    vector<int> ruta;

    if (!visitados[destino]) {
        // Si no se alcanzó el destino, se retorna vector vacío
        return {};
    }

    for (int nodo = destino; nodo != -1; nodo = predecesores[nodo]) {
        ruta.push_back(nodo);
    }

    // Invertir ruta hecha para tener la correcta
    reverse(ruta.begin(), ruta.end());
    return ruta;
}

//Nivel 4 : Floyd-Warshall
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> AlgoritmosGrafo::algoritmoFloydWarshall() {

    int vertices = grafo.getCantidadNodos(); 
    const int infinito = INT_MAX/2; // Simulacion de infinito

    //matriz de adyacencia desde el grafo
    std::vector<std::vector<int>> distancias = grafo.obtenerMatrizAdyacencia(); 
    //matriz de predecesores 
    std::vector<std::vector<int>> predecesores(vertices, std::vector<int>(vertices,-1));

    //Inicializa la matriz de predecesores 
    for(int i=0; i < vertices ;i++){
        for(int j=0; j < vertices ;j++){
            if(i != j && distancias[i][j] != infinito){
                predecesores[i][j]= i; 
            }
        }
    }
      
    //Algortimo de Floyd-Warshall
   for(int h=0; h < vertices ;h++){
    for(int i=0; i < vertices ;i++){
        for(int j=0; j < vertices ;j++){
            if(distancias[i][h] != infinito && distancias[h][j] != infinito &&  distancias[i][h] + distancias[h][j] < distancias[i][j]) { // Comparar distancias
                distancias[i][j] = distancias[i][h] + distancias[h][j];
                predecesores[i][j] = predecesores[h][j]; 
            }
        }
    }
}

    return {distancias, predecesores};
}

//ruta del resultado del algoritmo Floyd-Warshall
std::vector<int> AlgoritmosGrafo::rutaFloydWarshall(int origen, int destino,  const std::vector<std::vector<int>>& predecesores) {
    if(predecesores[origen][destino] == -1){ //Por si no hay camino
        return{};
    }

    std::vector<int> ruta;
    int actual = destino;

    //Reconstruir la ruta 
    while(actual != -1){
        ruta.push_back(actual);
        actual = predecesores[origen][actual];
    }

    //obtener la ruta en orden 
    std::reverse(ruta.begin(), ruta.end());
    if(!ruta.empty() && ruta[0] != origen){
        return {}; //no hay ruta directa
    }

    return ruta; 

}

// Funcion para calcular costo total de una ruta
int AlgoritmosGrafo::calcularCostoRuta(const std::vector<int>& ruta) {
    if (ruta.size() < 2) return -1;                 // Si la ruta tiene menos de 2 nodos, no es valida
    int costoRuta = 0;

    for (int i = 0; i < ruta.size() - 1; i++) {
        int peso = grafo.getPesoArista(ruta[i], ruta[i+1]);
        if (peso == -1) return -1;                  // Si no existe una arista en la ruta, no es valida
        costoRuta += peso;
    }

    return costoRuta;
}

// Funcion para imprimir una ruta de forma legible
void AlgoritmosGrafo::imprimirRutaConCosto(const std::vector<int>& ruta) {
    if (ruta.empty()) {
        cout << "Ruta vacia o no encontrada." << endl;
        return;
    }

    int costo = calcularCostoRuta(ruta);

    cout << "Ruta: ";
    for (size_t i = 0; i < ruta.size(); i++) {
        cout << ruta[i];
        if (i != ruta.size() - 1) cout << " -> ";
    }
    cout << " | Costo total: " << costo << endl;
}