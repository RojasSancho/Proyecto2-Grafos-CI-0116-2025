#include "grafo.h"
#include "algoritmosGrafo.h"

using namespace std;

int main () 
{   
    Grafo grafo(5);
    grafo.cargarDesdeArchivo("data/prueba.txt");
    grafo.imprimirGrafo();

    int inicioPruebas = 0;
    int destinoPruebas = 0;

    AlgoritmosGrafo algoritmos(grafo);
    vector<int> rutaBFS = algoritmos.busquedaAnchura(inicioPruebas, destinoPruebas);
    vector<int> rutaDFS = algoritmos.busquedaProfundidad(inicioPruebas, destinoPruebas);
    vector<int> rutaPrim = algoritmos.algoritmoPrim(inicioPruebas, destinoPruebas);

    algoritmos.imprimirRutaConCosto(rutaBFS);
    
    algoritmos.imprimirRutaConCosto(rutaDFS);

    algoritmos.imprimirRutaConCosto(rutaPrim);
    
 


    cout << "\n Floyd-Warshall: " << endl;
    // calcula todas las distancias
    auto [distancias, predecesores] = algoritmos.algoritmoFloydWarshall();
    // obtiene una ruta especifica 
    vector<int> rutaFloyd = algoritmos.rutaFloydWarshall(inicioPruebas, destinoPruebas, predecesores);
    algoritmos.imprimirRutaConCosto(rutaFloyd);
    //Importante para verificar valores 
    cout << "Distancia minima (verificacion con matriz de costos): " << distancias[inicioPruebas][destinoPruebas] << endl; 

    return 0; 
}