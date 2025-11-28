#ifndef ALGORITMOSGRAFO_H
#define ALGORITMOSGRAFO_H

#include "Grafo.h" 

class AlgoritmosGrafo {
private:
Grafo& grafo; // Referencia al grafo sobre el que se trabaja

public:
    // Constructor
    AlgoritmosGrafo(Grafo& grafo);

    // --- Nivel 1: BFS y DFS ---
    std::vector<int> busquedaAnchura(int inicio, int objetivo);
    std::vector<int> busquedaProfundidad(int inicio, int objetivo);

    // --- Nivel 2: Greedy ---
    std::vector<int> algoritmoGreedy(int inicio, int objetivo);

    // --- Nivel 3: Dijkstra ---
    std::vector<int> algoritmoDijkstra(int inicio, int objetivo);

    // --- Nivel 4: Floyd-Warshall ---
    std::vector<std::vector<int>> algoritmoFloydWarshall();

    // Funcion para calcular costo total de una ruta
    int calcularCosto(const std::vector<int>& ruta);

};

#endif