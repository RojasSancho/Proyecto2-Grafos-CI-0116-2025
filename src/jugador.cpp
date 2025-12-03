#include "jugador.h"

using namespace std;

Jugador::Jugador(Grafo grafo, int bateria, int ubicacion, int recursos){
    this->grafo = grafo;
    this->bateria = bateria;
    this->ubicacion = ubicacion;
    this->saldoRecursos = recursos;
}

void Jugador::moverJugador(int bateria, int idOrigen, int idDestino){
    ubicacion = idOrigen;
    Nodo nodoOrigen = grafo.obtenerNodo(idOrigen);
    Nodo nodoDestino = grafo.obtenerNodo(idDestino);

    bateria = getBateria();

    if(grafo.existeArista(nodoOrigen.id, nodoDestino.id)){
        int gastoBateria = grafo.getPesoArista(nodoOrigen.id, nodoDestino.id);
        bateria -= gastoBateria;
        ubicacion = nodoDestino.id;
    }else{
        cout << "No existe una arista directa entre los nodos " << nodoOrigen.id << " y " << nodoDestino.id << endl;
        cout << "Seleccione un nodo adyacente al jugador: " << endl;
        cin >> idDestino;
        moverJugador(bateria, idOrigen, idDestino);
    }
    estadoJugador();
}

void Jugador::accionesEnNodo(Grafo grafo){
    AlgoritmosGrafo algoritmo(grafo);
    Nodo nodoActual = grafo.obtenerNodo(ubicacion);
    int tipo = grafo.getTipoNodoPorID(ubicacion);
    int input = 0; // Variable para opciones de usuario

    if(tipo == 0){
        cout << "Nodo de paso. No hay acciones disponibles." << endl;
    }else if(tipo == 1){
        bateria = 100;
        cout << "Nodo base. La batería ha sido recargada" << endl;
    }else if(tipo == 2){
        cout << "Nodo recurso." << endl;
        if(nodoActual.existeMaquina = false){
            cout <<"Desea gastar sus recursos y crear una nueva maquina? (SI = 1, NO = Numero != 1)" << endl;
            if(input == 1){
                nodoActual.existeMaquina = true;
                saldoRecursos == 0;
                nodoActual.nivelMaquina++;
                comprarMaquina(nodoActual.valor, algoritmo);
                nodoActual.existeMaquina = true;
            }else{
                cout<<"No se ha comprado ninguna maquina." << endl;
            }
        }else{
            cout <<"Desea gastar sus recursos y actualizar la maquina? (SI = 1, NO = Numero != 1)" << endl;
            if(input == 1){
                saldoRecursos == 0;
                nodoActual.nivelMaquina++;
                actualizarMaquina(nodoActual.valor, nodoActual.nivelMaquina, algoritmo);
                if(nodoActual.nivelMaquina == 4){
                    cout << "La maquina ya esta en su nivel maximo. No requiere más actualizaciones." << endl;
                }
            }else{
                cout<<"No se ha actualizado la maquina." << endl;
            }
        }
    }else{
        cout << "Tipo de nodo desconocido." << endl;
    }
}

void Jugador::comprarMaquina(int recursos, AlgoritmosGrafo algoritmo){
    saldoRecursos += recursos;
    algoritmo.busquedaAnchura(ubicacion, grafo.getBaseID());
    algoritmo.busquedaProfundidad(ubicacion, grafo.getBaseID());
}//Falta implementar recoleccion de recursos

void Jugador::actualizarMaquina(int recursos, int nivel, AlgoritmosGrafo algoritmo){
    saldoRecursos += recursos;
    switch (nivel){
    case 2:
        algoritmo.algoritmoPrim(ubicacion, grafo.getBaseID());
        break;
    case 3:
        algoritmo.algoritmoDijkstra(ubicacion, grafo.getBaseID());
        break; 
    case 4:
        algoritmo.algoritmoFloydWarshall();
        break;
    }
}//Falta implementar recoleccion de recursos

void Jugador::estadoJugador(){
    if(bateria <= 0){
        cout << "El jugador ha quedado sin bateria. Ha perdido el juego." << endl;
    }else if(saldoRecursos >= 200){ //Hay que definir la cantidad para ganar
        cout << "El jugador ha conseguido recursos suficientes para reparar el motor y escapar. Ha ganado el juego" << endl;
    }
}

int Jugador::getBateria() const{
    return bateria;
}

int Jugador::getUbicacion() const{
    return ubicacion;
}

int Jugador::getRecursos() const{
    return saldoRecursos;
}


