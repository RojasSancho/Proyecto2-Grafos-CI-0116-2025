#include "grafo.h"
#include "algoritmosGrafo.h"

using namespace std;

int main () 
{   
    Grafo grafo(50);
    grafo.cargarDesdeArchivo("data/grafo.txt");
    grafo.imprimirGrafo();

    int inicioPruebas = 5;
    int destinoPruebas = 1;

    AlgoritmosGrafo algoritmos(grafo);
    vector<int> rutaBFS = algoritmos.busquedaAnchura(inicioPruebas, destinoPruebas);
    vector<int> rutaDFS = algoritmos.busquedaProfundidad(inicioPruebas, destinoPruebas);
    vector<int> rutaPrim = algoritmos.algoritmoPrim(inicioPruebas, destinoPruebas);

    algoritmos.imprimirRutaConCosto(rutaBFS);
    
    algoritmos.imprimirRutaConCosto(rutaDFS);

    algoritmos.imprimirRutaConCosto(rutaPrim);
    

    return 0; 
}