#include "grafo.h"
#include "algoritmosGrafo.h"

using namespace std;

int main () 
{   
    Grafo grafo(50);
    grafo.cargarDesdeArchivo("data/grafo.txt");
    // grafo.imprimirGrafo();

    int inicioPruebas = 13;
    int destinoPruebas = 0;

    AlgoritmosGrafo algoritmos(grafo);
    vector<int> rutaBFS = algoritmos.busquedaAnchura(inicioPruebas, destinoPruebas);
    vector<int> rutaDFS = algoritmos.busquedaProfundidad(inicioPruebas, destinoPruebas);
    vector<int> rutaPrim = algoritmos.algoritmoPrim(inicioPruebas, destinoPruebas);

    cout << "\nBFS: " << endl;
    algoritmos.imprimirRutaConCosto(rutaBFS);
    cout << "\nDFS: " << endl;
    algoritmos.imprimirRutaConCosto(rutaDFS);
    cout << "\nPrim (Usando MST): " << endl;
    algoritmos.imprimirRutaConCosto(rutaPrim);
    
    cout << "\nFloyd-Warshall: " << endl;
    // Calcula todas las distancias
    auto [distancias, predecesores] = algoritmos.algoritmoFloydWarshall();
    // Obtiene una ruta especifica 
    vector<int> rutaFloyd = algoritmos.rutaFloydWarshall(inicioPruebas, destinoPruebas, predecesores);
    algoritmos.imprimirRutaConCosto(rutaFloyd);
    // Importante para verificar valores 
    cout << "Distancia minima (verificacion con matriz de costos): " << distancias[inicioPruebas][destinoPruebas] << endl; 

    // Prueba Dijkstra
    vector<int> rutaDijkstra = algoritmos.algoritmoDijkstra(inicioPruebas, destinoPruebas);
    cout << "\nDijkstra: " << endl;
    algoritmos.imprimirRutaConCosto(rutaDijkstra);

    return 0; 
}