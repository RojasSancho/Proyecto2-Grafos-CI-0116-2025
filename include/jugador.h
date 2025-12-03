#ifndef JUGADOR_H
#define JUGADOR_H

#include "grafo.h"
#include "algoritmosGrafo.h"
#include <vector>
#include <string>

// Struct para gestionar cada tuberia ya instalada
struct MaquinaInstalada {
    int idNodoRecurso;
    int nivel;
    std::vector<int> rutaABase;
    int costoMantenimiento;
    int valorRecursoBruto;
    int gananciaNeta;
};

class Jugador {
    private:
        Grafo& grafo;  //Referencia a Grafo sobre el que se mueve el jugador
        int bateria;
        int ubicacion;
        int saldoRecursos;

        // Constantes del juego (inicializadas en el .cpp)
        const int BATERIA_MAXIMA;
        const int RECARGA_PARCIAL_PCT;
        const int COSTO_MOTOR_PLASMA;
        const int COSTO_MAQUINA_NIVEL_1; 
        const int COSTO_ACTUALIZAR_NIVEL_2; 
        const int COSTO_ACTUALIZAR_NIVEL_3;

        std::vector<MaquinaInstalada> maquinasInstaladas;

    public:
        // --- Constructor ---
        Jugador(Grafo& grafo, int bateria, int ubicacion, int saldoRecursos);

        // --- Funciones para manejar acciones del jugador ---
        //Mueve el jugador de un nodo a otro, descontando la bateria correspondiente
        void moverJugador(int idDestino); 

        //Define si se puede realizar alguna accion en el nodo actual o si simplemente es de paso
        void accionesEnNodo(); 

        // Ciclo de economia del juego
        void cicloEconomico();

        //BFS =1, DFS = 2
        void comprarMaquina(int algoritmoElegido, AlgoritmosGrafo& algoritmo);

        //En caso de existir una maquina en el nodo actual, aumenta su nivel
        void actualizarMaquina(int nivelEsperado, AlgoritmosGrafo& algoritmo);

        // --- Estados de victoria/derrota ---
        void estadoJugador();

        // --- Getters ---
        int getBateria() const;
        int getUbicacion() const;
        int getRecursos() const;

        // Getter para la interfaz: devuelve la lista de tuberías instaladas
        const std::vector<MaquinaInstalada>& getMaquinasInstaladas() const { return maquinasInstaladas; }
};

#endif