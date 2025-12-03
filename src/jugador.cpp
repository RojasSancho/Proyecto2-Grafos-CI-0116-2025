#include "jugador.h"
#include <iostream>
#include <algorithm>

using namespace std;

// --- Constructor ---
Jugador::Jugador(Grafo& grafo, int bateria, int ubicacion, int saldoRecursos)
: grafo(grafo), bateria(bateria), ubicacion(ubicacion), saldoRecursos(saldoRecursos),
    // Inicializacion de constantes del juego
    BATERIA_MAXIMA(100), 
    RECARGA_PARCIAL_PORCENTAJE(60), 
    COSTO_MOTOR_PLASMA(4000), 
    COSTO_MAQUINA_NIVEL_1(50), 
    COSTO_ACTUALIZAR_NIVEL_2(150), 
    COSTO_ACTUALIZAR_NIVEL_3_4(300) 
{}

// Movimiento del jugador
void Jugador::moverJugador(int idDestino) {
    // Validar arista
    if (!grafo.existeArista(ubicacion, idDestino)) {
        cerr << "Error: No existe arista directa entre nodo " << ubicacion << " y " << idDestino << endl;
        // La GUI debe notificar que no existe camino
        return;
    }

    int costo = grafo.getPesoArista(ubicacion, idDestino);

    // Verificar batería insuficiente
    if (bateria < costo) {
        bateria = 0;     // Se queda sin batería (condición de derrota)
        cout << "¡Bateria agotada! Costo de movimiento: " << costo << endl;
        estadoJugador(); // Chequear derrota inmediatamente
        return;
    }

    // Mover
    bateria -= costo;
    ubicacion = idDestino;

    // Acciones segun nodo
    accionesEnNodo();

    // Impresión de estado actual después del movimiento
    cout << "\n--- Estado Actual ---" << endl;
    cout << "Ubicacion: Nodo " << ubicacion << endl;
    cout << "Bateria: " << bateria << " / " << BATERIA_MAXIMA << endl;
    cout << "Recursos: " << saldoRecursos << endl;
    cout << "---------------------\n" << endl;

    // Evaluar victoria o derrota
    estadoJugador();
}

// Acciones según el tipo de nodo
void Jugador::accionesEnNodo() {
    Nodo& nodoActual = grafo.obtenerNodoReferencia(ubicacion);

    switch (nodoActual.tipo) {
    case 1:
        // Nodo base: recarga completa
        bateria = BATERIA_MAXIMA;
        cout << "Regreso a la Base. Bateria restaurada al 100%." << endl;
        break;
    case 2:
        // Nodo con recursos: recarga parcial de bateria si hay máquina instalada
        if (nodoActual.existeMaquina) {
            int recargaLimite = (BATERIA_MAXIMA * RECARGA_PARCIAL_PORCENTAJE) / 100; // 60%
            if (bateria < recargaLimite) {
                bateria = recargaLimite;
                cout << "Recarga parcial en maquina. Bateria al " << RECARGA_PARCIAL_PORCENTAJE << "%." << endl;
            }
        }
        break;
    default:
        // Nodo Vacio (Tipo 0): solo paso
        cout << "Terreno baldio. Continuar explorando." << endl;
        break;
    }
}

// Ciclo Económico 
// Generación de recursos periódica por las tuberías instaladas
void Jugador::cicloEconomico() {
    for (const auto& maquina : maquinasInstaladas) {
        saldoRecursos += maquina.gananciaNeta;
    }
}

// --- Funciones de Máquinas (Implementación de Economía y Rutas) ---

// Compra de máquina en nodo actual (Nivel 1)
void Jugador::comprarMaquina(int algElegido, AlgoritmosGrafo& algoritmo) {
    Nodo& nodo = grafo.obtenerNodoReferencia(ubicacion);
    int baseID = grafo.getBaseID();
    
    // 1. Validaciones
    if (nodo.tipo != 2 || nodo.existeMaquina) return;
    if (saldoRecursos < COSTO_MAQUINA_NIVEL_1) return;

    // 2. Ejecutar algoritmo (Nivel 1: BFS o DFS)
    std::vector<int> ruta;
    if (algElegido == 1) { // 1 = BFS
        ruta = algoritmo.busquedaAnchura(ubicacion, baseID);
    } else if (algElegido == 2) { // 2 = DFS
        ruta = algoritmo.busquedaProfundidad(ubicacion, baseID);
    } else {
        return; 
    }
    
    if (ruta.empty()) return;

    // 3. Calcular Ganancia Neta
    int valorBruto = nodo.valor;
    int costoMantenimiento = algoritmo.calcularCostoRuta(ruta);
    int gananciaNeta = valorBruto - costoMantenimiento;

    if (gananciaNeta <= 0) return; 

    // 4. Registrar y actualizar estado
    saldoRecursos -= COSTO_MAQUINA_NIVEL_1;
    
    // Guardar la nueva máquina instalada en el vector
    MaquinaInstalada nuevaMaquina = {
        ubicacion, 1, ruta, costoMantenimiento, valorBruto, gananciaNeta
    };
    maquinasInstaladas.push_back(nuevaMaquina);

    // Actualizar nodo en el grafo
    nodo.existeMaquina = true;
    nodo.nivelMaquina = 1;
}

// Actualizar máquina
void Jugador::actualizarMaquina(int nivelEsperado, AlgoritmosGrafo& algoritmo) {
    Nodo& nodo = grafo.obtenerNodoReferencia(ubicacion);
    int baseID = grafo.getBaseID();

    // 1. Validaciones y Costo
    if (nodo.tipo != 2 || !nodo.existeMaquina || nodo.nivelMaquina >= nivelEsperado) return;
    
    int costoUpgrade = 0;
    if (nivelEsperado == 2) costoUpgrade = COSTO_ACTUALIZAR_NIVEL_2;
    else if (nivelEsperado == 3) costoUpgrade = COSTO_ACTUALIZAR_NIVEL_3_4;
    else if (nivelEsperado == 4) costoUpgrade = COSTO_ACTUALIZAR_NIVEL_3_4;
    else return; 

    if (saldoRecursos < costoUpgrade) return;
    
    // 2. Ejecutar algoritmo y calcular nueva ruta
    std::vector<int> ruta;
    switch (nivelEsperado) {
        case 2: // Nivel 2: Prim
            ruta = algoritmo.algoritmoPrim(ubicacion, baseID); 
            break;
        case 3: // Nivel 3: Dijkstra
            ruta = algoritmo.algoritmoDijkstra(ubicacion, baseID);
            break;
        case 4: // Nivel 4: Floyd-Warshall
            {
                auto [dist, pred] = algoritmo.algoritmoFloydWarshall();
                ruta = algoritmo.rutaFloydWarshall(ubicacion, baseID, pred);
            }
            break;
    }
    
    if (ruta.empty()) return;

    // 3. Recalcular Ganancia Neta
    int valorBruto = nodo.valor;
    int costoMantenimiento = algoritmo.calcularCostoRuta(ruta);
    int gananciaNeta = valorBruto - costoMantenimiento;

    if (gananciaNeta <= 0) return; 

    // 4. Aplicar cambios
    saldoRecursos -= costoUpgrade;
    nodo.nivelMaquina = nivelEsperado; 

    // 5. Actualizar el registro de máquinas
    auto it = std::find_if(maquinasInstaladas.begin(), maquinasInstaladas.end(), 
        [this](const MaquinaInstalada& m){ return m.idNodoRecurso == ubicacion; });
    
    if (it != maquinasInstaladas.end()) {
        it->nivel = nivelEsperado;
        it->rutaABase = ruta;
        it->costoMantenimiento = costoMantenimiento;
        it->gananciaNeta = gananciaNeta;
    }
}

// Estado del jugador
void Jugador::estadoJugador() {
    
    // Condición de derrota
    if (bateria <= 0) {
        return; 
    }

    // Condición de victoria
    if (saldoRecursos >= COSTO_MOTOR_PLASMA) { 
        return;
    }
}

// Getters
int Jugador::getBateria() const { return bateria; }
int Jugador::getUbicacion() const { return ubicacion; }
int Jugador::getRecursos() const { return saldoRecursos; }