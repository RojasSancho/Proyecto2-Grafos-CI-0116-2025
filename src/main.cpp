#include "grafo.h"
#include "algoritmosGrafo.h"

using namespace std;

int main () 
{   
    Grafo grafo(50);
    grafo.cargarDesdeArchivo("data/grafo.txt");
    // grafo.imprimirGrafo();

    AlgoritmosGrafo algoritmos(grafo);
    vector<int> rutaBFS = algoritmos.busquedaAnchura(31, 4);
    vector<int> rutaDFS = algoritmos.busquedaProfundidad(31, 4);

    for (int nodo : rutaBFS) {
        cout << nodo << " ";
    }
    cout << endl;

    for (int nodo : rutaDFS) {
        cout << nodo << " ";
    }
    cout << endl;

    return 0; 
}