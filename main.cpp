#include "grafo.h"

using namespace std;

int main () 
{   
    Grafo grafo(3);
    grafo.cargarDesdeArchivo("/data/prueba.txt");
    grafo.imprimirGrafo();

    return 0; 
}