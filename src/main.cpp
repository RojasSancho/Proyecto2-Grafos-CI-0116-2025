#include "../include/grafo.h"
#include "../include/jugador.h"
#include "../include/algoritmosGrafo.h"
#include "../include/interfaz.h"

using namespace std;

int main() 
{
    // Crea grafo y lo carga
    Grafo grafo(48);
    grafo.cargarDesdeArchivo("data/grafo.txt");

    // Crear jugador
    // Jugador(grafo, bateriaInicial, ubicacionInicial, recursosInicial)
    Jugador jugador(grafo, 100, grafo.getBaseID(), 100);

    // Crea modulo de algoritmos
    AlgoritmosGrafo algoritmos(grafo);

    // Crea interfaz grafica
    Interfaz gui(grafo, jugador, algoritmos);

    // Ejecuta el juego
    gui.ejecutar();

    return 0;
}
