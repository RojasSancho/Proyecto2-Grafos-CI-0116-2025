#include "grafo.h"
#include <iostream>

using namespace std;

//Constructor
Grafo::Grafo(int cantidadNodos)
{
    this->cantidadNodos = cantidadNodos;
    this->listaAdyacencia.resize(cantidadNodos); // Crea un vector de pares para cada nodo
}

// --- Funciones para manejar nodos y aristas ---
void Grafo::agregarNodo(const Nodo& nodo) {
    // Evitar agregar nodos fuera de la cantidad de nodos establecida en constructor
    if (nodos.size() >= cantidadNodos) return; 
    nodos.push_back(nodo);
}

bool Grafo::agregarArista(int origen, int destino, int peso) {
    // Verificar que los índices (id nodos) sean válidos
    if (origen < 0 || origen >= cantidadNodos || destino < 0 || destino >= cantidadNodos) {
        return false; 
    }

    // Agregar la arista a la lista de adyacencia para el nodo de origen indicado
    listaAdyacencia[origen].push_back({destino, peso});
    return true;
}

// --- Funciones auxiliares ---
// Imprime la información de todos los nodos y sus aristas
void Grafo::imprimirGrafo() const {
    for (size_t i=0; i < listaAdyacencia.size(); i++) {
        cout << i << ": [";
        for (size_t j = 0; j < listaAdyacencia[i].size(); j++) {
            cout    << "(" << listaAdyacencia[i][j].first
                    << ", " << listaAdyacencia[i][j].second << ")";
            if(j != listaAdyacencia[i].size() - 1) {
                cout << ", ";
            }
        }
        cout << "]\n";
    }
}

// Retorna el nodo con el ID dado 
Nodo Grafo::obtenerNodo(int id) const {
    return nodos[id];
}

// Retorna los nodos adyacentes del nodo con ID dado (id debe ser válido)
const vector<pair<int,int>>& Grafo::obtenerAdyacentes(int id) const {
    return listaAdyacencia[id];
}

// --- Getters ---
int Grafo::getCantidadNodos() const {
    return cantidadNodos;
}

// --- Cargar grafo desde archivo .txt
void cargarDesdeArchivo(const std::string& nombreArchivo) {
    
}