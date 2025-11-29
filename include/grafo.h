#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <utility>   // para std::pair
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

struct Nodo {
    int id;        // ID del nodo
    int tipo;      // 0 = vacío, 1 = base, 2 = recurso
    int valor;     // valor del recurso si aplica
    int x, y;      // coordenadas opcionales para visualización
};

class Grafo {
    private:
        int cantidadNodos;
        int baseID;

        std::vector<Nodo> nodos;   // lista de nodos
        std::vector<std::vector<std::pair<int,int>>> listaAdyacencia; // lista de adyacencia (destino, peso)
    
    public:
        // --- Constructor ---
        Grafo(int cantidadNodos);

        // --- Funciones para manejar nodos y aristas ---
        // Agrega un nodo al grafo
        void agregarNodo(const Nodo& nodo);

        //Agrega una arista dirigida desde 'origen' a 'destino' con un peso
        bool agregarArista(int origen, int destino, int peso);

        // --- Funciones auxiliares ---
        // Imprime la información de todos los nodos y sus aristas
        void imprimirGrafo() const;

        // Retorna el nodo con el ID dado 
        Nodo obtenerNodo(int id) const;

        // Retorna los nodos adyacentes del nodo con ID dado (id debe ser válido)
        const std::vector<std::pair<int,int>>& obtenerAdyacentes(int id) const;

        // Saber si existe arista directa
        bool existeArista(int nodo1, int nodo2) const;

        bool validarNodo(int id) const;

        // --- Getters ---
        int getCantidadNodos() const;
        int getBaseID() const;
        int getPesoArista(int nodo1, int nodo2) const;
        int getTipoNodoPorID(int id) const;
        int getValorNodoPorID(int id) const;
        int getXNodoPorID(int id) const;
        int getYNodoPorID(int id) const;
        std::pair<int,int> getCoordenadasNodoPorID(int id) const;

        // Consultar si un nodo es base o recurso
        bool esRecurso(int id) const;
        bool esBase(int id) const;

        // --- Cargar grafo desde archivo .txt ---
        void cargarDesdeArchivo(const std::string& nombreArchivo);
};

#endif