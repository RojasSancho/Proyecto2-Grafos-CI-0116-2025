#include "grafo.h"
#include "algoritmosGrafo.h"

using namespace std;

int main () 
{   
    Grafo grafo(50);
    grafo.cargarDesdeArchivo("data/grafo.txt");
    // grafo.imprimirGrafo();

    AlgoritmosGrafo algoritmos(grafo);
    vector<int> ruta = algoritmos.busquedaAnchura(35, 0);

    for (int nodo : ruta) {
        cout << nodo << " ";
    }
    cout << endl;

    return 0; 
}