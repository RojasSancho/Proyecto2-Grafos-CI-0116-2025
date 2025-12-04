#include "../include/grafo.h"

using namespace std;

//Constructor
Grafo::Grafo(int cantidadNodos)
{
    this->cantidadNodos = cantidadNodos;
    this->listaAdyacencia.resize(cantidadNodos); // Crear un vector de pares para cada nodo
    this->nodos.resize(cantidadNodos);
    this->baseID = -1; // ID de la base se detecta al cargar archivo de texto
}

Grafo::Grafo() {
}

// Funciones para manejar nodos y aristas
void Grafo::agregarNodo(const Nodo& nodo) {
    // Verificar que el ID es válido
    if (nodo.id < 0 || nodo.id >= cantidadNodos) {
        cerr << "ID de nodo fuera de rango: " << nodo.id << endl;
        return;
    }

    // Colocar el nodo en su posición exacta
    nodos[nodo.id] = nodo;

    // Guardar ID de la base
    if (nodo.tipo == 1) {
        baseID = nodo.id;
    }
}

bool Grafo::agregarArista(int origen, int destino, int peso) {
    // Verificar que los índices (id nodos) sean válidos
    if (origen < 0 || origen >= cantidadNodos || destino < 0 || destino >= cantidadNodos) {
        cerr << "Arista invalida: " << origen << " -> " << destino << endl;
        return false; 
    }

    // Agregar la conexión de ida (Origen -> Destino)
    listaAdyacencia[origen].push_back({destino, peso});

    // Agregar la conexión de vuelta (Destino -> Origen) 
    listaAdyacencia[destino].push_back({origen, peso});

    return true;
}

// Funciones auxiliares
// Imprime la información de todos los nodos y sus aristas
void Grafo::imprimirGrafo() const {
    for (int i = 0; i < cantidadNodos; i++) {
        cout << i << " (tipo " << nodos[i].tipo << "): [";
        for (size_t j = 0; j < listaAdyacencia[i].size(); j++) {
            cout    << "(" << listaAdyacencia[i][j].first
                    << ", " << listaAdyacencia[i][j].second << ")";
            if (j + 1 < listaAdyacencia[i].size()) cout << ", ";
        }
        cout << "]\n";
    }
}

// Retorna vector de nodos adyacentes del nodo con ID dado (id debe ser válido)
const vector<pair<int,int>>& Grafo::obtenerAdyacentes(int id) const {
    if (!validarNodo(id)) {
        static const vector<pair<int,int>> vacio;
        return vacio;
    }
    return listaAdyacencia[id];
}

// Retorna la matriz de adyacencia con el grafo dado para Floyd-Warshall
std::vector<std::vector<int>> Grafo::obtenerMatrizAdyacencia()const{

    int vertices = cantidadNodos;
    const int infinito = INT_MAX/2; // Valor suficientemente "Grande" para representar valor int infinito

    // Inicializar la matriz con infinito
    std::vector<std::vector<int>> matriz(vertices, std::vector<int>(vertices, infinito));
    // Diagonal en 0 para Floyd-Warshall
    for(int i=0;i<vertices;i++){
        matriz[i][i]= 0;
    }

    // Llenar la matriz
    for(int origen=0;origen<vertices;origen++){
        for(const auto& arista : listaAdyacencia[origen]){
            int destino = arista.first;
            int peso = arista.second;
            matriz[origen][destino]= peso;
        }
    }

    return matriz; 
}

// Validar nodo (reutilizable)
bool Grafo::validarNodo(int id) const {
    return id >= 0 && id < cantidadNodos;
}

// Getters
// Retorna el nodo con el ID dado 
Nodo Grafo::obtenerNodo(int id) const {
    // Validar que este dentro del rango
    if (!validarNodo(id)) {
        cerr << "obtenerNodo: id fuera de rango\n";
        return Nodo{-1,0,0,0,0}; 
    }
    return nodos[id];
}

// Retorna referencia al nodo real (permite modificarlo)
Nodo& Grafo::obtenerNodoReferencia(int id) {
    if (!validarNodo(id)) {
        cerr << "obtenerNodoRef: id fuera de rango\n";
        throw std::out_of_range("Nodo inválido");
    }
    return nodos[id];
}

int Grafo::getCantidadNodos() const {
    return cantidadNodos;
}

int Grafo::getBaseID() const {
    return baseID;
}

// Tipo y valor 
int Grafo::getTipoNodoPorID(int id) const {
    if (!validarNodo(id)) return -1;
    return nodos[id].tipo;
}

int Grafo::getValorNodoPorID(int id) const {
    if (!validarNodo(id)) return 0;
    return nodos[id].valor;
}

// Coordenadas
int Grafo::getXNodoPorID(int id) const {
    if (!validarNodo(id)) return 0;
    return nodos[id].x;
}

int Grafo::getYNodoPorID(int id) const {
    if (!validarNodo(id)) return 0;
    return nodos[id].y;
}

std::pair<int,int> Grafo::getCoordenadasNodoPorID(int id) const {
    if (!validarNodo(id)) return {0,0};
    return {nodos[id].x, nodos[id].y};
}

// Obtener peso de arista a->b
int Grafo::getPesoArista(int nodo1, int nodo2) const {
    if (!validarNodo(nodo1) || !validarNodo(nodo2)) return -1;
    for (auto& par : listaAdyacencia[nodo1]) {
        if (par.first == nodo2) return par.second;
    }
    return -1;
}

// Consultar si es base o recurso
bool Grafo::esRecurso(int id) const {
    if (!validarNodo(id)) return false;
    return nodos[id].tipo == 2;
}

bool Grafo::esBase(int id) const {
    if (!validarNodo(id)) return false;
    return nodos[id].tipo == 1;
}

// Saber si existe arista directa
bool Grafo::existeArista(int nodo1, int nodo2) const {
    if (!validarNodo(nodo1) || !validarNodo(nodo2)) return false;
    for (auto& par : listaAdyacencia[nodo1]) {
        if (par.first == nodo2) return true;
    }
    return false;
}

// --- Cargar grafo desde archivo .txt
void Grafo::cargarDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo); // Abrir archivo en modo lectura
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo\n";
        return;
    }

    // Variables locales para control de carga de nodos o conexiones
    string lineaTexto;
    bool leyendoNodos = false;
    bool leyendoConexiones = false;

    while (getline(archivo, lineaTexto)) {
        // Casos para control de inicio de carga de nodos o conexiones
        if (lineaTexto.empty()) { continue; }
        if (lineaTexto == "NODOS:") { leyendoNodos = true; leyendoConexiones = false; continue; }
        if (lineaTexto == "CONEXIONES:") { leyendoNodos = false; leyendoConexiones = true; continue; }

        // Limpiar la linea de espacios
        lineaTexto.erase(
            remove(lineaTexto.begin(), lineaTexto.end(), ' '),
            lineaTexto.end()
        );
        // Crear flujo de entrada que contiene la linea actual
        stringstream ss(lineaTexto);
        char coma;

        if (leyendoNodos) {
            // Agregar linea con informacion de nodo al grafo
            int id, tipo, valor, x, y;
            ss >> id >> coma >> tipo >> coma >> valor >> coma >> x >> coma >> y;
            Nodo nodo{id, tipo, valor, x, y};
            agregarNodo(nodo);
        } else if (leyendoConexiones) {
            // Agregar linea con informacion de conexion (arista) al grafo
            int origen, destino, peso;
            ss >> origen >> coma >> destino >> coma >> peso;
            agregarArista(origen, destino, peso);
        }
    }

    archivo.close(); // Cerrar archivo
}