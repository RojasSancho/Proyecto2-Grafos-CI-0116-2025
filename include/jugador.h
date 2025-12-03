#ifndef JUGADOR_H
#define JUGADOR_H

#include "grafo.h"
#include "algoritmosGrafo.h"
#include <vector>
#include <string>

class Jugador {
    private:
        Grafo grafo; //Grafo sobre el que se mueve el jugador
        int bateria;
        int ubicacion;
        int saldoRecursos;
    public:
        // --- Constructor ---
        Jugador(Grafo grafo, int bateria, int ubicacion, int saldoRecursos);

        // --- Funciones para manejar acciones del jugador ---
        //Mueve el jugador de un nodo a otro, descontando la bateria correspondiente
        void moverJugador(int bateria, int idOrigen, int idDestino);

        //Define si se puede realizar alguna accion en el nodo actual o si simplemente es de paso
        void accionesEnNodo(Grafo grafo);

        //Compra una maquina en el nodo actual
        void comprarMaquina(int recursos, AlgoritmosGrafo algoritmo);

        //En caso de existir una maquina en el nodo actual, aumenta su nivel
        void actualizarMaquina(int recursos, int nivel, AlgoritmosGrafo algoritmo);

        //Define las conidiciones de victoria o derrota del jugador
        void estadoJugador();

        // --- Getters ---
        int getBateria() const;
        int getUbicacion() const;
        int getRecursos() const;
};

#endif