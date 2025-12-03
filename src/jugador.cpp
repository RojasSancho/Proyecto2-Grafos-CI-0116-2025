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
ResultadoAccion Jugador::moverJugador(int idDestino) {
    // Validar arista
    if (!grafo.existeArista(ubicacion, idDestino)) {
        cerr << "Error: No existe arista directa entre nodo " << ubicacion << " y " << idDestino << endl;
        // La GUI debe notificar que no existe camino
        return {false, "No existe arista directa entre los nodos"};
    }

    int costo = grafo.getPesoArista(ubicacion, idDestino);

    // Verificar batería insuficiente
    if (bateria < costo) {
        bateria = 0;     // Se queda sin batería (condición de derrota)
        cout << "¡Bateria agotada! Costo de movimiento: " << costo << endl;
        estadoJugador(); // Chequear derrota inmediatamente
        return {false, "Batería insuficiente para moverse. Batería agotada."};
    }

    // Mover
    bateria -= costo;
    ubicacion = idDestino;

    // Acciones segun nodo
    auto resNodo = accionesEnNodo();
    if (!resNodo.exito) return resNodo;

    // Impresión de estado actual después del movimiento
    cout << "\n--- Estado Actual ---" << endl;
    cout << "Ubicacion: Nodo " << ubicacion << endl;
    cout << "Bateria: " << bateria << " / " << BATERIA_MAXIMA << endl;
    cout << "Recursos: " << saldoRecursos << endl;
    cout << "---------------------\n" << endl;

    // Evaluar victoria o derrota
    auto resEstado = estadoJugador();
    if (!resEstado.exito) return resEstado;

    return {true, "Jugador movido a nodo " + to_string(ubicacion)};
}

// Acciones según el tipo de nodo
ResultadoAccion Jugador::accionesEnNodo() {
    Nodo& nodoActual = grafo.obtenerNodoReferencia(ubicacion);

    switch (nodoActual.tipo) {
    case 1:
        bateria = BATERIA_MAXIMA;
        return {true, "Regresaste a la Base. Batería restaurada al 100%."};

    case 2:
        if (nodoActual.existeMaquina) {
            int recargaLimite = (BATERIA_MAXIMA * RECARGA_PARCIAL_PORCENTAJE) / 100;

            if (bateria < recargaLimite) {
                bateria = recargaLimite;
                return {true, "Recarga parcial realizada. Batería al 60%."};
            }
            return {true, "Hay máquina instalada, pero no se necesita recarga."};
        }
        return {true, "Descubriste un nodo de recursos. Podés instalar una máquina."};

    default:
        return {true, "Terreno vacío. No hay acciones disponibles."};
    }
}

// Ciclo Económico 
// Generación de recursos periódica por las tuberías instaladas
ResultadoAccion Jugador::cicloEconomico() {
    cout << "--- CICLO ECONOMICO INICIADO ---" << endl;
    int recursosGenerados = 0;
    for (const auto& maquina : maquinasInstaladas) {
        saldoRecursos += maquina.gananciaNeta;
        recursosGenerados += maquina.gananciaNeta;
    }

    auto estado = estadoJugador();
    if (!estado.exito) return estado;

    return {true, "Ganaste +" + to_string(recursosGenerados) + " recursos este ciclo. Total: " + to_string(saldoRecursos)};
}

// --- Funciones de Máquinas (Implementación de Economía y Rutas) ---

// Compra de máquina en nodo actual (Nivel 1: BFS o DFS)
ResultadoAccion Jugador::comprarMaquina(int algoritmoElegido, AlgoritmosGrafo& algoritmo) {
    Nodo& nodo = grafo.obtenerNodoReferencia(ubicacion);
    int baseID = grafo.getBaseID();
    
    // Validaciones
    if (nodo.tipo != 2) 
        return {false, "Solo podés instalar máquinas en nodos de recurso."};

    if (nodo.existeMaquina) 
        return {false, "Ya existe una máquina en este nodo."};

    if (saldoRecursos < COSTO_MAQUINA_NIVEL_1)
        return {false, "Recursos insuficientes para comprar una máquina."};

    // Ejecutar algoritmo (Nivel 1: BFS o DFS)
    vector<int> ruta;
    string nombreAlgoritmo;

    if (algoritmoElegido == 1) { // 1 = BFS
        ruta = algoritmo.busquedaAnchura(ubicacion, baseID);
        nombreAlgoritmo = "BFS";
    } else if (algoritmoElegido == 2) { // 2 = DFS
        ruta = algoritmo.busquedaProfundidad(ubicacion, baseID);
        nombreAlgoritmo = "DFS";
    } else {
        return {false, "Algoritmo inválido (usa 1 = BFS o 2 = DFS)."};
    }
    
    if (ruta.empty())
            return {false, "No existe ruta válida a la base usando " + nombreAlgoritmo};

    // Calcular Ganancia Neta
    int valorBruto = nodo.valor;
    int costoMantenimiento = algoritmo.calcularCostoRuta(ruta);
    int gananciaNeta = valorBruto - costoMantenimiento;

    if (gananciaNeta <= 0)
        return {false, "Ganancia neta negativa. No conviene instalar esta máquina."};

    // Registrar y actualizar estado
    saldoRecursos -= COSTO_MAQUINA_NIVEL_1;
    
    MaquinaInstalada nuevaMaquina = {
        ubicacion, 1, ruta, costoMantenimiento, valorBruto, gananciaNeta
    };
    maquinasInstaladas.push_back(nuevaMaquina);

    // Actualizar nodo en el grafo
    nodo.existeMaquina = true;
    nodo.nivelMaquina = 1;

    return {true, "Máquina Nivel 1 (" + nombreAlgoritmo + ") instalada. Ganancia neta: " + to_string(gananciaNeta)};
}

// Actualizar máquina
ResultadoAccion Jugador::actualizarMaquina(int nivelEsperado, AlgoritmosGrafo& algoritmo) {
    Nodo& nodo = grafo.obtenerNodoReferencia(ubicacion);
    int baseID = grafo.getBaseID();

    // Validaciones
    if (nodo.tipo != 2 || !nodo.existeMaquina)
        return {false, "No hay máquina para actualizar aquí."};

    if (nodo.nivelMaquina >= nivelEsperado)
        return {false, "La máquina ya está en este nivel."};
    
    // Calcular Costo de Mejora
    int costoUpgrade = 0;
    string nombreAlgoritmo;

    switch (nivelEsperado) {
        case 2: costoUpgrade = COSTO_ACTUALIZAR_NIVEL_2; nombreAlgoritmo = "Prim"; break;
        case 3: costoUpgrade = COSTO_ACTUALIZAR_NIVEL_3_4; nombreAlgoritmo = "Dijkstra"; break;
        case 4: costoUpgrade = COSTO_ACTUALIZAR_NIVEL_3_4; nombreAlgoritmo = "Floyd-Warshall"; break;
        default: return {false, "Nivel inválido."};
    }

    if (saldoRecursos < costoUpgrade)
        return {false, "Recursos insuficientes para actualizar la máquina."};
    
    // Ejecutar algoritmo y calcular nueva ruta
    vector<int> ruta;
    if (nivelEsperado == 2)
        ruta = algoritmo.algoritmoPrim(ubicacion, baseID);
    else if (nivelEsperado == 3)
        ruta = algoritmo.algoritmoDijkstra(ubicacion, baseID);
    else {
        auto [distancia, prededecesor] = algoritmo.algoritmoFloydWarshall();
        ruta = algoritmo.rutaFloydWarshall(ubicacion, baseID, prededecesor);
    }
    
    if (ruta.empty())
        return {false, "No se encontró ruta válida usando " + nombreAlgoritmo};

    // Recalcular Ganancia Neta
    int valorBruto = nodo.valor;
    int costoMantenimiento = algoritmo.calcularCostoRuta(ruta);
    int gananciaNeta = valorBruto - costoMantenimiento;

    if (gananciaNeta <= 0)
        return {false, "La actualización reduce demasiado la ganancia. Se cancela."};

    // Aplicar cambios
    saldoRecursos -= costoUpgrade;
    nodo.nivelMaquina = nivelEsperado; 

    // Actualizar el registro de máquinas
    auto it = std::find_if(
        maquinasInstaladas.begin(),
        maquinasInstaladas.end(),
        [this](const MaquinaInstalada& m) { return m.idNodoRecurso == ubicacion; }
    );

    if (it == maquinasInstaladas.end())
        return {false, "Error crítico: Máquina no encontrada."};

    it->nivel = nivelEsperado;
    it->rutaABase = ruta;
    it->costoMantenimiento = costoMantenimiento;
    it->gananciaNeta = gananciaNeta;

    return {true, "Máquina actualizada a nivel " + to_string(nivelEsperado) +". Nueva ganancia neta: " + to_string(gananciaNeta)};
}

// Estado del jugador
ResultadoAccion Jugador::estadoJugador() {
    if (bateria <= 0)
        return {false, "Derrota: Te quedaste sin batería."};

    if (saldoRecursos >= COSTO_MOTOR_PLASMA)
        return {false, "Victoria: Construiste el motor de plasma."};

    return {true, "Estado OK"};
}
// Getters
int Jugador::getBateria() const { return bateria; }
int Jugador::getUbicacion() const { return ubicacion; }
int Jugador::getRecursos() const { return saldoRecursos; }