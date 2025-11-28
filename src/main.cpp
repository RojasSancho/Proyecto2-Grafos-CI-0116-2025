#include "grafo.h"

using namespace std;

int main () 
{   
    Grafo grafo(50);
    grafo.cargarDesdeArchivo("data/grafo.txt");
    grafo.imprimirGrafo();

    return 0; 
}