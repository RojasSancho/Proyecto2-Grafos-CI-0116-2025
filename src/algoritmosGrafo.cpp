#include "algoritmosGrafo.h"

using namespace std;

//Constructor
AlgoritmosGrafo::AlgoritmosGrafo(Grafo& grafo) : grafo(grafo){}

// --- METODOS DE ALGORITMOS (Devuelven un vector con la ruta generada) ---

// --- Nivel 1: BFS ---
vector<int> AlgoritmosGrafo::busquedaAnchura(int inicio, int destino) {
    int cantidadNodos = grafo.getCantidadNodos();
    queue<int> cola;                                // Utilizar principio FIFO para implementar BFS
    vector<bool> visitados(cantidadNodos, false);
    vector<int> predecesores(cantidadNodos, -1);    // Un -1 indica que nodo no tiene predecesor o padre

    visitados[inicio] = true;                       // Marcar nodo inicio como visitado
    cola.push(inicio);

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();

        if (actual == destino) break;               // Salir de ciclo al llegar al destino

        // Exploracion por capas del BFS
        for (const auto& vecino : grafo.obtenerAdyacentes(actual)) {
            int nodoVecino = vecino.first;          // Ignorar el peso y utilizar solo vecinos
            if (!visitados[nodoVecino]) {
                visitados[nodoVecino] = true;       // Marcar nodo vecino como visitado
                predecesores[nodoVecino] = actual;  // Almacenar nodo anterior en vector de predecesores
                cola.push(nodoVecino);              // Insertar nodo vecino a la cola
            }
        }
    }

    // Reconstruir la ruta desde destino hasta inicio
    vector<int> ruta;
    if (!visitados[destino]) return ruta; // Si no se llegó al destino, retornar lista vacía

    // Usar vector de predecesores para reconstruir ruta inversa
    for (int nodoActual = destino; nodoActual != -1; nodoActual = predecesores[nodoActual]) {
        ruta.push_back(nodoActual);
    }

    // Invertir ruta hecha para tener la correcta
    reverse(ruta.begin(), ruta.end());
    return ruta;
}