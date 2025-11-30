#include "grafo.h"
#include "algoritmosGrafo.h"

using namespace std;

int main () 
{   
    Grafo grafo(50);
    grafo.cargarDesdeArchivo("data/grafo.txt");
    // grafo.imprimirGrafo();

    AlgoritmosGrafo algoritmos(grafo);
    vector<int> rutaBFS = algoritmos.busquedaAnchura(17, 3);
    vector<int> rutaDFS = algoritmos.busquedaProfundidad(17, 3);

    algoritmos.imprimirRutaConCosto(rutaBFS);
    
    algoritmos.imprimirRutaConCosto(rutaDFS);
    

    return 0; 
}