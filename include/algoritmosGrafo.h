#ifndef ALGORITMOSGRAFO_H
#define ALGORITMOSGRAFO_H

#include "Grafo.h" 
#include <vector>
#include <utility>
#include <queue> 
#include <algorithm>

class AlgoritmosGrafo {
private:
    Grafo& grafo; // Referencia al grafo sobre el que se trabaja

public:
    // Constructor
    AlgoritmosGrafo(Grafo& grafo);

    // --- METODOS DE ALGORITMOS (Devuelven un vector con la ruta generada) ---

    // --- Nivel 1: BFS y DFS ---
    std::vector<int> busquedaAnchura(int inicio, int destino);
    std::vector<int> busquedaProfundidad(int inicio, int destino);

    // --- Nivel 2: Greedy/Prim ---
    std::vector<int> algoritmoPrim(int inicio, int destino);

    // --- Nivel 3: Dijkstra ---
    std::vector<int> algoritmoDijkstra(int inicio, int destino);

    // --- Nivel 4: Floyd-Warshall ---
    // Devuelve un par de matrices: primera = distancias, segunda = predecesores
    std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> algoritmoFloydWarshall();
    // Reconstruye la ruta desde 'origen' hasta 'destino' usando la matriz de predecesores de Floyd-Warshall
    std::vector<int> rutaFloydWarshall(int origen, int destino, const std::vector<std::vector<int>>& predecesores);

    // Funcion para calcular costo total de una ruta
    int calcularCosto(const std::vector<int>& ruta);

};

#endif