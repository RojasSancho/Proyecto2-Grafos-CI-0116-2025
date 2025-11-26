#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <utility>   // para std::pair
#include <string>

struct Nodo {
    int id;        // ID del nodo
    int tipo;      // 0 = vacío, 1 = base, 2 = recurso
    int valor;     // valor del recurso si aplica
    int x, y;      // coordenadas opcionales para visualización
};

class Grafo {
    private:
        int cantidadNodos;
        std::vector<Nodo> nodos;   // lista de nodos
        std::vector<std::vector<std::pair<int,int>>> listaAdyacencia; // lista de adyacencia (destino, peso)
    
    public:
        // Constructor
        Grafo(int nodos);

        // Funciones para manejar nodos y aristas
        void agregarNodo(const Nodo& nodo);
        void agregarArista(int origen, int destino, int peso);
        void imprimirGrafo();

};

#endif