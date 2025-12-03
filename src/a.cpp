#include "jugador.h"
#include <iostream>
#include <algorithm> // Para std::find_if, necesario para actualizar máquina
#include <vector>

using namespace std;

// --- Constructor ---
// Inicializa las variables miembro y las constantes en la lista de inicialización.
Jugador::Jugador(Grafo& grafo, int bateria, int ubicacion, int saldoRecursos)
    : grafo(grafo), bateria(bateria), ubicacion(ubicacion), saldoRecursos(saldoRecursos),
      // Inicialización de Constantes (Valores de ejemplo)
      BATERIA_MAXIMA(100), 
      RECARGA_PARCIAL_PCT(60), 
      COSTO_MOTOR_PLASMA(5000), 
      COSTO_MAQUINA_NIVEL_1(50), 
      COSTO_ACTUALIZAR_NIVEL_2(100), 
      COSTO_ACTUALIZAR_NIVEL_3(200) 
{}

// -------------------------------------------------------------------
// --- Funciones de Movimiento y Acción ---
// -------------------------------------------------------------------

void Jugador::moverJugador(int idDestino) {
    if (!grafo.existeArista(ubicacion, idDestino)) return;

    int costo = grafo.getPesoArista(ubicacion, idDestino);

    if (bateria < costo) {
        bateria = 0; 
        return; 
    }

    bateria -= costo;
    ubicacion = idDestino;

    accionesEnNodo(); // Lógica de recarga
    estadoJugador(); // Chequeo de victoria/derrota
}

// Acciones según el tipo de nodo (Implementa la lógica de recarga parcial)
void Jugador::accionesEnNodo() {
    Nodo& nodoActual = grafo.obtenerNodoReferencia(ubicacion);

    switch (nodoActual.tipo) {
    case 1:
        // Nodo base: recarga completa
        bateria = BATERIA_MAXIMA;
        break;
    case 2:
        // Nodo recurso: recarga parcial si hay máquina instalada
        if (nodoActual.existeMaquina) {
            int recargaLimite = (BATERIA_MAXIMA * RECARGA_PARCIAL_PCT) / 100; // 60%
            if (bateria < recargaLimite) {
                bateria = recargaLimite;
            }
        }
        break;
    default:
        break;
    }
    // IMPORTANTE: La recolección de recursos (ganancia) ocurre en cicloEconomico.
}

// -------------------------------------------------------------------
// --- Ciclo Económico ---
// -------------------------------------------------------------------

// Generación de recursos periódica por las tuberías instaladas
void Jugador::cicloEconomico() {
    for (const auto& maquina : maquinasInstaladas) {
        saldoRecursos += maquina.gananciaNeta;
    }
}

// -------------------------------------------------------------------
// --- Funciones de Máquinas (Implementación de Economía y Rutas) ---
// -------------------------------------------------------------------

// Compra de máquina en nodo actual (Nivel 1)
void Jugador::comprarMaquina(int algElegido, AlgoritmosGrafo& algoritmo) {
    Nodo& nodo = grafo.obtenerNodoReferencia(ubicacion);
    int baseID = grafo.getBaseID();
    
    // 1. Validaciones
    // Debe ser nodo de recurso (2) y no debe haber máquina instalada
    if (nodo.tipo != 2 || nodo.existeMaquina) return;
    if (saldoRecursos < COSTO_MAQUINA_NIVEL_1) return;

    // 2. Ejecutar algoritmo (Nivel 1: BFS o DFS)
    std::vector<int> ruta;
    if (algElegido == 1) { // 1 = BFS
        ruta = algoritmo.busquedaAnchura(ubicacion, baseID);
    } else if (algElegido == 2) { // 2 = DFS
        ruta = algoritmo.busquedaProfundidad(ubicacion, baseID);
    } else {
        return; // Algoritmo no válido
    }
    
    if (ruta.empty()) return; // No hay ruta a la base

    // 3. Calcular Ganancia Neta
    int valorBruto = nodo.valor;
    int costoMantenimiento = algoritmo.calcularCostoRuta(ruta);
    int gananciaNeta = valorBruto - costoMantenimiento;

    // Solo se instala si la ganancia neta es positiva
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
    // Debe ser nodo de recurso (2), tener máquina y el nivel debe ser una mejora
    if (nodo.tipo != 2 || !nodo.existeMaquina || nodo.nivelMaquina >= nivelEsperado) return;
    
    int costoUpgrade = 0;
    if (nivelEsperado == 2) costoUpgrade = COSTO_ACTUALIZAR_NIVEL_2;
    else if (nivelEsperado == 3) costoUpgrade = COSTO_ACTUALIZAR_NIVEL_3;
    else if (nivelEsperado == 4) costoUpgrade = COSTO_ACTUALIZAR_NIVEL_3; // Se asume mismo costo para Nivel 4
    else return; 

    if (saldoRecursos < costoUpgrade) return;
    
    // 2. Ejecutar algoritmo y calcular nueva ruta
    std::vector<int> ruta;
    switch (nivelEsperado) {
        case 2: // Nivel 2: Prim (Greedy)
            ruta = algoritmo.algoritmoPrim(ubicacion, baseID); 
            break;
        case 3: // Nivel 3: Dijkstra (Óptimo)
            ruta = algoritmo.algoritmoDijkstra(ubicacion, baseID);
            break;
        case 4: // Nivel 4: Floyd-Warshall (Óptimo Global)
            {
                auto [dist, pred] = algoritmo.algoritmoFloydWarshall();
                ruta = algoritmo.rutaFloydWarshall(ubicacion, baseID, pred);
            }
            break;
    }
    
    if (ruta.empty()) return; // No hay ruta

    // 3. Recalcular Ganancia Neta
    int valorBruto = nodo.valor;
    int costoMantenimiento = algoritmo.calcularCostoRuta(ruta);
    int gananciaNeta = valorBruto - costoMantenimiento;

    // Solo se actualiza si la nueva ganancia es mejor o igual a la actual (o al menos > 0)
    if (gananciaNeta <= 0) return; 

    // 4. Aplicar cambios
    saldoRecursos -= costoUpgrade;
    nodo.nivelMaquina = nivelEsperado; 

    // 5. Actualizar el registro de máquinas
    // Busca la máquina instalada en el nodo actual
    auto it = std::find_if(maquinasInstaladas.begin(), maquinasInstaladas.end(), 
        [this](const MaquinaInstalada& m){ return m.idNodoRecurso == ubicacion; });
    
    if (it != maquinasInstaladas.end()) {
        it->nivel = nivelEsperado;
        it->rutaABase = ruta;
        it->costoMantenimiento = costoMantenimiento;
        it->gananciaNeta = gananciaNeta;
    }
}

// -------------------------------------------------------------------
// --- Estados ---
// -------------------------------------------------------------------

void Jugador::estadoJugador() {
    
    // Condición de derrota
    if (bateria <= 0) {
        // La GUI debe detectar derrota
        return; 
    }

    // Condición de victoria
    if (saldoRecursos >= COSTO_MOTOR_PLASMA) { 
        // La GUI debe detectar victoria
        return;
    }
}

// --- Getters ---
int Jugador::getBateria() const { return bateria; }
int Jugador::getUbicacion() const { return ubicacion; }
int Jugador::getRecursos() const { return saldoRecursos; }