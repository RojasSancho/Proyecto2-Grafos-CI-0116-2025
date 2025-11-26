#ifndef ALGORITMOSGRAFO_H
#define ALGORITMOSGRAFO_H

#include "Grafo.h" // Para poder acceder a la estructura del grafo

class AlgoritmosGrafo {
private:
Grafo& grafo; // Referencia al grafo sobre el que se trabaja

public:
    // Constructor
    AlgoritmosGrafo(Grafo& g);

    // Algoritmos BFS y DFS (Nivel 1)
    std::vector<int> busquedaAnchura(int inicio, int objetivo);
    std::vector<int> busquedaProfundidad(int inicio, int objetivo);

    // Algoritmo Greedy (Nivel 2)
    std::vector<int> algoritmoGreedy(int inicio, int objetivo);

    // Algoritmos Dijkstra y Floyd-Warshall (Nivel 3 y 4)
    std::vector<int> algoritmoDijkstra(int inicio, int objetivo);
    std::vector<std::vector<int>> algoritmoFloydWarshall();

    // Funcion para calcular costo total de una ruta
    int calcularCosto(const std::vector<int>& ruta);

};

#endif