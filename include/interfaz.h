#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "../include/grafo.h"
#include "../include/jugador.h"
#include "../include/algoritmosGrafo.h"
#include "raylib.h"
#include <string>

class Interfaz {
private:
    Grafo& grafo;
    Jugador& jugador;
    AlgoritmosGrafo& algoritmos;

    std::string mensajeEstado; // Ultimo mensaje del juego
    float radioNodo; // Radio para dibujar y detectar clics en nodos

    int screenWidth;
    int screenHeight;
    int hudWidth;
    float offsetX;
    float offsetY;
    
    bool mostrarAyuda;

    bool mostrarFin; // hay ventana de fin de partida abierta
    bool finVictoria; // true = jugador gano, false = jugador perdio 
    bool salirJuego; // para cerrar la ventana completa


    // Metodos internos de dibujo
    void dibujarGrafo(); // Nodos + aristas
    void dibujarMaquinas(); // Tubeia por nivel (rojo/amarillo/verde/azul)
    void dibujarJugador(); // Posicion del jugador
    void dibujarHUD(); // Bateria, recursos, costos, texto de estado
    void dibujarVentanaFin(); 

    // Manejo del input 
    void manejarInput(); // Clics, teclas (comprar/upgrade, ciclo)
    int detectarNodoClick(const Vector2& mouse); // Para identificar nodo clickeado

public:
    Interfaz(Grafo& g, Jugador& j, AlgoritmosGrafo& a);

    // Loop principal del juego (abre ventana y corre hasta cerrarse)
    void ejecutar();
};

#endif
