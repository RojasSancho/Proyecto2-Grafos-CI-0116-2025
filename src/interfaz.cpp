#include "../include/interfaz.h"
#include <cmath>
#include <iostream>

using namespace std;

// Implementacion de la interfaz grafica principal del juego de grafos

// Constante de zoom para escalar el grafo en la ventana
static const float ZOOM = 1.4f;

// Fuente global usada para todos los textos de la interfaz
static Font gFuente;
static bool gFuenteCargada = false;

// Constructor de la interfaz, configura referencias y dimensiones base de la ventana
Interfaz::Interfaz(Grafo& g, Jugador& j, AlgoritmosGrafo& a)
    : grafo(g), jugador(j), algoritmos(a),
      mensajeEstado("Listo."), radioNodo(15.0f),
      mostrarAyuda(false),
      mostrarFin(false),   
      finVictoria(false),     
      salirJuego(false)       
{
    screenWidth  = 1550;
    screenHeight = 850;
    hudWidth  = 420;  
    offsetX = hudWidth;  // mueve el grafo a la derecha
    offsetY = 0;
}

// Bucle principal del juego: inicializa ventana, procesa input y dibuja todo
void Interfaz::ejecutar() {

    InitWindow(screenWidth, screenHeight, "Proyecto II - Protocolo de Grafos");
    SetTargetFPS(60);

    // Carga la fuente personalizada para el HUD
    gFuente = LoadFontEx("fonts/arial.ttf", 24, 0, 250);
    SetTextureFilter(gFuente.texture, TEXTURE_FILTER_BILINEAR);
    gFuenteCargada = true;

    while (!WindowShouldClose()) {
        manejarInput();

        BeginDrawing();
        ClearBackground(BLACK);

        dibujarGrafo();
        dibujarMaquinas();
        dibujarJugador();
        dibujarHUD();
        dibujarVentanaFin();

        EndDrawing();
    }

    // Libera la fuente al salir del juego
    if (gFuenteCargada) {
        UnloadFont(gFuente);
        gFuenteCargada = false;
    }

    CloseWindow();
}

// Manejo general de entradas de mouse y teclado para juego y ventanas
void Interfaz::manejarInput() {

    // Cuando la ventana de fin esta activa solo se procesa input de los botones
    if (mostrarFin) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            int w = 500;
            int h = 220;
            int px = (screenWidth - w) / 2;
            int py = (screenHeight - h) / 2;

            Rectangle btnReiniciar;
            btnReiniciar.x = (float)(px + 50);
            btnReiniciar.y = (float)(py + h - 70);
            btnReiniciar.width  = 150;
            btnReiniciar.height = 40;

            Rectangle btnSalir;
            btnSalir.x = (float)(px + w - 200);
            btnSalir.y = (float)(py + h - 70);
            btnSalir.width  = 150;
            btnSalir.height = 40;

            if (CheckCollisionPointRec(mouse, btnReiniciar)) {
                jugador.reiniciarPartida();
                mostrarFin = false;
                finVictoria  = false;
                mensajeEstado = "Listo.";
            } else if (CheckCollisionPointRec(mouse, btnSalir)) {
                    CloseWindow();
                    exit(0);  // termina el programa inmediatamente

            }
        }
        return;  // no procesar mas input mientras la ventana este abierta
    }

    // Configuracion del boton de ayuda en el panel izquierdo
    Rectangle btnAyuda;
    btnAyuda.x = 20;
    btnAyuda.y = (float)(screenHeight - 60);
    btnAyuda.width  = 200;
    btnAyuda.height = 35;

    // Procesa clicks para ayuda y movimiento del jugador
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();

        // Si se hace click en el boton de ayuda, solo se alterna la ventana de ayuda
        if (CheckCollisionPointRec(mouse, btnAyuda)) {
            mostrarAyuda = !mostrarAyuda;
            return; // no intentamos mover jugador en este click
        }

        // Si no fue en el boton de ayuda se intenta mover al nodo clickeado
        int nodoDestino = detectarNodoClick(mouse);
        if (nodoDestino != -1) {
            ResultadoAccion res = jugador.moverJugador(nodoDestino);
            mensajeEstado = res.mensaje;

            // Se consulta el estado general del jugador para detectar victoria o derrota
            ResultadoAccion est = jugador.estadoJugador();
            if (!est.exito) {
                mostrarFin  = true;
                // Es victoria si el mensaje inicia con la palabra "Victoria"
                finVictoria = (est.mensaje.rfind("Victoria", 0) == 0);
            }
        }

    }

    // Atajo de teclado para mostrar u ocultar ayuda
    if (IsKeyPressed(KEY_H)) {
        mostrarAyuda = !mostrarAyuda;
    }

    // Teclas para comprar o mejorar maquinas segun el nivel
    if (IsKeyPressed(KEY_ONE)) {
        mensajeEstado = jugador.comprarMaquina(1, algoritmos).mensaje;
    }
    if (IsKeyPressed(KEY_TWO)) {
        mensajeEstado = jugador.comprarMaquina(2, algoritmos).mensaje;
    }
    if (IsKeyPressed(KEY_THREE)) {
        mensajeEstado = jugador.actualizarMaquina(2, algoritmos).mensaje;
    }
    if (IsKeyPressed(KEY_FOUR)) {
        mensajeEstado = jugador.actualizarMaquina(3, algoritmos).mensaje;
    }
    if (IsKeyPressed(KEY_FIVE)) {
        mensajeEstado = jugador.actualizarMaquina(4, algoritmos).mensaje;
    }

    // Ciclo economico para generar recursos de todas las maquinas activas
    if (IsKeyPressed(KEY_C)) {
        ResultadoAccion res = jugador.cicloEconomico();
        mensajeEstado = res.mensaje;

        if (!res.exito) {  // cicloEconomico ya devuelve Victoria/Derrota si aplica
            mostrarFin = true;
            finVictoria = (res.mensaje.rfind("Victoria", 0) == 0);
            }
    }

    // Compra del motor de plasma, potencial condicion de victoria
    if (IsKeyPressed(KEY_M)) {
        ResultadoAccion res = jugador.comprarMotorPlasma();
        mensajeEstado = res.mensaje;

        // Se vuelve a revisar el estado global para saber si ya se gano
        ResultadoAccion est = jugador.estadoJugador();
        if (!est.exito) {
            mostrarFin = true;
            finVictoria = (est.mensaje.rfind("Victoria", 0) == 0);
        }
}

}

// Determina si el mouse esta sobre un nodo del grafo y lo devuelve
int Interfaz::detectarNodoClick(const Vector2& mouse) {
    int n = grafo.getCantidadNodos();

    for (int i = 0; i < n; i++) {
        auto nodo = grafo.obtenerNodo(i);

        float px= nodo.x * ZOOM + offsetX;   // compensar desplazamiento + escala
        float py= nodo.y * ZOOM + offsetY;

        float dx= mouse.x - px;
        float dy= mouse.y - py;
        float dist = sqrtf(dx*dx + dy*dy);

        if (dist <= radioNodo) return i;
    }
    return -1;
}

// Dibuja el grafo: aristas, pesos y nodos con su identificador
void Interfaz::dibujarGrafo() {
    int n = grafo.getCantidadNodos();

    // Dibujo de las aristas y sus pesos
    for (int i = 0; i < n; i++) {
        auto nodoA = grafo.obtenerNodo(i);
        const auto& ady = grafo.obtenerAdyacentes(i);
        for (const auto& par : ady) {
            int j = par.first;
            if (j <= i) {
                continue; // evitar dibujar dos veces (grafo bidireccional)
            } 

            auto nodoB = grafo.obtenerNodo(j);

            Vector2 p1 = { nodoA.x * ZOOM + offsetX, nodoA.y * ZOOM + offsetY };
            Vector2 p2 = { nodoB.x * ZOOM + offsetX, nodoB.y * ZOOM + offsetY };

            // Linea base de la arista
            DrawLineEx(p1, p2, 2.0f, GRAY);

            // Calculo de la posicion del peso sobre la arista
            int peso = grafo.getPesoArista(i, j);

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            float len = sqrtf(dx*dx + dy*dy);

            Vector2 mid;
            if (len > 0.0f) {
                mid.x = (p1.x + p2.x) * 0.5f;
                mid.y = (p1.y + p2.y) * 0.5f;

                // Desplaza el texto en direccion perpendicular a la arista
                float nx = -dy / len;
                float ny =  dx / len;
                float offset = 10.0f;

                mid.x += nx * offset;
                mid.y += ny * offset;
            } else {
                // Caso si los puntos coinciden
                mid = p1;
            }

            DrawCircle(mid.x, mid.y, 10, BLACK);
            DrawText(TextFormat("%d", peso),mid.x - 6, mid.y - 6,14, WHITE);
        }
    }

    // Dibujo de los nodos con color segun su tipo
    for (int i = 0; i < n; i++) {
        auto nodo = grafo.obtenerNodo(i);

        Color colorNodo;

        if (nodo.tipo == 1) {
            colorNodo = RED;
        }
        else if (nodo.tipo == 2) {
            colorNodo = ORANGE;
        }
        else {
            colorNodo = GREEN;
        }


        float px = nodo.x * ZOOM + offsetX;
        float py = nodo.y * ZOOM + offsetY;

        DrawCircle(px, py, radioNodo, colorNodo);
        DrawText(TextFormat("%d", nodo.id), px - 6, py - 6, 12, BLACK);
    }
}

// Dibuja las maquinas instaladas y sus tuberias hacia la base
void Interfaz::dibujarMaquinas() {
    const auto& maquinas = jugador.getMaquinasInstaladas();

    for (const auto& m : maquinas) {

        Color colorTuberia;

        if (m.nivel == 1) {
            colorTuberia = RED;
        }
        else if (m.nivel == 2) {
            colorTuberia = YELLOW;
        }
        else if (m.nivel == 3) {
            colorTuberia = GREEN;
        }
        else {
            colorTuberia = BLUE;
        }


        const vector<int>& ruta = m.rutaABase;
        if (ruta.size() < 2) {
            continue;
        
        }

        // Dibuja los segmentos de la ruta de la maquina hacia la base
        for (size_t i = 0; i + 1 < ruta.size(); i++) {
            auto nA = grafo.obtenerNodo(ruta[i]);
            auto nB = grafo.obtenerNodo(ruta[i + 1]);

            Vector2 p1 = { nA.x * ZOOM + offsetX, nA.y * ZOOM + offsetY };
            Vector2 p2 = { nB.x * ZOOM + offsetX, nB.y * ZOOM + offsetY };

            DrawLineEx(p1, p2, 4.0f, colorTuberia);
        }

        // Marca el nodo de recurso asociado a la maquina
        auto recurso = grafo.obtenerNodo(m.idNodoRecurso);
        DrawCircleLines(recurso.x * ZOOM + offsetX,recurso.y * ZOOM + offsetY,radioNodo + 3, colorTuberia);
    }
}

// Dibuja la posicion actual del jugador sobre el grafo
void Interfaz::dibujarJugador() {
    auto nodo = grafo.obtenerNodo(jugador.getUbicacion());

    DrawCircle(nodo.x * ZOOM + offsetX, nodo.y * ZOOM + offsetY,radioNodo * 0.6f, SKYBLUE);
}

// Dibuja el HUD lateral con bateria, recursos, costos y ayuda
void Interfaz::dibujarHUD() {

    // Fondo del panel lateral
    DrawRectangle(0, 0, hudWidth, screenHeight, Fade(BLACK, 0.8f));

    int bateria = jugador.getBateria();
    int recursos = jugador.getRecursos();
    int bateriaMax = jugador.getBateriaMaxima();

    float pct = (float)bateria / bateriaMax;
    if (pct < 0) {
        pct = 0;
    }
    if (pct > 1) {
        pct = 1;
    }

    int x = 20, y = 20, ancho = 200, alto = 20;

    // Barra de bateria
    DrawRectangle(x, y, ancho, alto, DARKGRAY);
    DrawRectangle(x, y, (int)(ancho * pct), alto, LIME);
    DrawRectangleLines(x, y, ancho, alto, BLACK);

    // Textos principales del HUD con la fuente personalizada
    if (gFuenteCargada) {
        DrawTextEx(gFuente, "Bateria",{ (float)x, (float)(y - 22) }, 22, 1, WHITE);
        DrawTextEx(gFuente,TextFormat("%d / %d", bateria, bateriaMax),{ (float)(x + ancho + 15), (float)(y - 2) },20, 1, WHITE);

        // Recursos actuales del jugador
        DrawTextEx(gFuente,TextFormat("Recursos: %d", recursos),{ (float)x, (float)(y + alto + 12) }, 22, 1, WHITE);

        // Costos de maquinas y mejoras
        int yCost = y + alto + 50;

        DrawTextEx(gFuente,TextFormat("Costo Maquina N1:  %d", jugador.getCostoMaquinaNivel1()),
        { (float)x, (float)yCost }, 18, 1, LIGHTGRAY);

        DrawTextEx(gFuente,TextFormat("Upgrade N2:        %d", jugador.getCostoActualizarNivel2()),
        { (float)x, (float)(yCost + 22) }, 18, 1, LIGHTGRAY);

        DrawTextEx(gFuente,TextFormat("Upgrade N3/N4:     %d", jugador.getCostoActualizarNivel3_4()),
        { (float)x, (float)(yCost + 44) }, 18, 1, LIGHTGRAY);

        DrawTextEx(gFuente,TextFormat("Motor Plasma:      %d", jugador.getCostoMotorPlasma()),
        { (float)x, (float)(yCost + 66) }, 18, 1, LIGHTGRAY);

        // Mensaje de estado contextual del juego
        DrawTextEx(gFuente,
                   mensajeEstado.c_str(),
                   { (float)x, (float)(yCost + 100) },
                   20, 1, RAYWHITE);

        // Seccion de ayuda de controles rapidos
        int yHelp = yCost + 135;

        DrawTextEx(gFuente, "Controles:",{ (float)x, (float)yHelp }, 20, 1, YELLOW);
        DrawTextEx(gFuente, "Click: mover jugador",{ (float)x, (float)(yHelp + 22) }, 16, 1, LIGHTGRAY);
        DrawTextEx(gFuente, "1: BFS   | 2: DFS",{ (float)x, (float)(yHelp + 42) }, 16, 1, LIGHTGRAY);
        DrawTextEx(gFuente, "3: Prim  | 4: Dijkstra", { (float)x, (float)(yHelp + 62) }, 16, 1, LIGHTGRAY);
        DrawTextEx(gFuente, "5: Floyd | C: Ciclo eco", { (float)x, (float)(yHelp + 82) }, 16, 1, LIGHTGRAY);
        DrawTextEx(gFuente, "M: Comprar motor de plasma", { (float)x, (float)(yHelp + 102) }, 16, 1, LIGHTGRAY);
        DrawTextEx(gFuente, "H: Mostra Ayuda", { (float)x, (float)(yHelp + 122) }, 16, 1, LIGHTGRAY);
    } else {
        // Texto alternativo si la fuente personalizada no se cargo
        DrawText("Bateria", x, y - 18, 14, WHITE);
        DrawText(TextFormat("%d / %d", bateria, bateriaMax), x + ancho + 10, y, 16, WHITE);
        DrawText(TextFormat("Recursos: %d", recursos), x, y + alto + 10, 20, WHITE);
        
        int yCost = y + alto + 40;
        DrawText(TextFormat("Costo Maquina N1:  %d", jugador.getCostoMaquinaNivel1()), x, yCost, 16, LIGHTGRAY);
        DrawText(TextFormat("Upgrade N2:        %d", jugador.getCostoActualizarNivel2()), x, yCost + 20, 16, LIGHTGRAY);
        DrawText(TextFormat("Upgrade N3/N4:     %d", jugador.getCostoActualizarNivel3_4()), x, yCost + 40, 16, LIGHTGRAY);
        DrawText(TextFormat("Motor Plasma:      %d", jugador.getCostoMotorPlasma()), x, yCost + 60, 16, LIGHTGRAY);
    }

    // Configuracion y dibujo del boton de ayuda en la parte baja izquierda
    Rectangle btnAyuda;
    btnAyuda.x = 20;
    btnAyuda.y = (float)(screenHeight - 60);
    btnAyuda.width = 200;
    btnAyuda.height = 35;

    DrawRectangleRec(btnAyuda, DARKGRAY);
    DrawRectangleLinesEx(btnAyuda, 2, YELLOW);

    if (gFuenteCargada) {
        DrawTextEx(gFuente, "Ayuda (H)",{ btnAyuda.x + 10, btnAyuda.y + 7 }, 18, 1, WHITE);
    } else {
        DrawText("Ayuda (H)",(int)btnAyuda.x + 10, (int)btnAyuda.y + 10, 18, WHITE);
    }

    // Ventana emergente de ayuda con descripcion de mecanicas y logica
    if (mostrarAyuda) {
        int w = screenWidth - 600;
        int h = screenHeight - 200;
        int px = (screenWidth - w) / 2;
        int py = (screenHeight - h) / 2;

        DrawRectangle(px, py, w, h, Fade(DARKGRAY, 0.95f));
        DrawRectangleLines(px, py, w, h, YELLOW);

        int marginX = px + 20;
        int marginY = py + 20;
        int line = 0;
        float sizeTitle = 26;
        float sizeText = 18;
        float spacing = 1;

        if (gFuenteCargada) {
            // Titulo de la ventana de ayuda
            DrawTextEx(gFuente, "Ayuda del juego",
                       { (float)marginX, (float)(marginY + line * 28) },
                       sizeTitle, spacing, WHITE);
            line += 2;

            // Descripcion de las mecanicas basicas
            DrawTextEx(gFuente, "Mecanicas:",
                       { (float)marginX, (float)(marginY + line * 22) },
                       sizeText, spacing, YELLOW);
            line++;
            DrawTextEx(gFuente,
             "-Te mueves de nodo en nodo por las aristas. Cada movimiento consume bateria",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line++;
            DrawTextEx(gFuente,
             "-Si la bateria llega a 0 fuera de la base, pierdes la partida",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line++;
            DrawTextEx(gFuente,
             "-En la base la bateria se recarga al 100%; en las maquinas hasta un 60%",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line += 2;

            // Explicacion de la logica algoritmica y niveles de tecnologia
            DrawTextEx(gFuente, "Logica algoritimica:",
                       { (float)marginX, (float)(marginY + line * 22) },
                       sizeText, spacing, YELLOW);
            line++;
            DrawTextEx(gFuente,
             "-En nodos de recurso puedes comprar maquinas que generan tuberias a la base",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line++;
            DrawTextEx(gFuente,
             "-Cada ciclo economico sumas la ganancia neta de cada maquina",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line++;
            DrawTextEx(gFuente,
             "-Ganancia neta = valor del recurso - costo de mantenimiento de la tuberia",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line++;
            DrawTextEx(gFuente,
             "  (el costo es la suma de los pesos del camino que usa esa maquina)",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line += 2;

            DrawTextEx(gFuente,
             "Niveles de tecnologia de las maquinas:",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, YELLOW);
            line++;
            DrawTextEx(gFuente,
             "-Nivel 1: Tecnologia basica (Algoritmos ciegos), esta es la maquina mas barata.",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line++;
            DrawTextEx(gFuente,
             " Para construir la tuberia de regreso a la base, utiliza algoritmos de busqueda no informada,",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
             line++;
             DrawTextEx(gFuente,
             " el jugador decide si colocar una de BFS o DFS",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
             line++;
            DrawTextEx(gFuente,
             "-Nivel 2: Tecnologia estandar, al mejorar la tecnologia, el jugador accede a maquinas que utilizan un enfoque greedy. ",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line++;
            DrawTextEx(gFuente,
             " Este sistema construye la tuberia eligiendo siempre el camino vecino con menor peso inmediato. ",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
             line++;
            DrawTextEx(gFuente,
             "-Nivel 3: Tecnologia avanzada, desbloquea el uso de algoritmos de optimizacion como Dijkstra.",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line++;
            DrawTextEx(gFuente,
             " Estas maquinas garantizan el camino de costo minimo absoluto entre el recurso y la base.",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line += 2;

            // Explica la condicion de victoria y uso del ciclo economico
            DrawTextEx(gFuente, "Condicion de victoria:",{ (float)marginX, (float)(marginY + line * 22) },
            sizeText, spacing, YELLOW);
            line++;
            DrawTextEx(gFuente,
             "-Usas el ciclo economico para acumular recursos con tus maquinas",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line++;
            DrawTextEx(gFuente,
             "-Cuando tengas suficientes recursos para comprar el motor de plasma, ganas",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line += 2;

            // Consejos generales para el jugador
            DrawTextEx(gFuente,
             "Tips: vuelve a la base para recargar, evita rutas muy caras y mejora",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
            line++;
            DrawTextEx(gFuente,
             "las maquinas que esten dando buena ganancia neta.",
             { (float)marginX, (float)(marginY + line * 22) },
             sizeText, spacing, RAYWHITE);
        }
    }
}

// Dibuja la ventana emergente de fin de partida con opciones de reinicio o salida
void Interfaz::dibujarVentanaFin() {
    if (!mostrarFin) return;

    int w = 500;
    int h = 220;
    int px = (screenWidth - w) / 2;
    int py = (screenHeight - h) / 2;

    Color borde = finVictoria ? GREEN : RED;

    DrawRectangle(px, py, w, h, Fade(BLACK, 0.9f));
    DrawRectangleLines(px, py, w, h, borde);

   const char* titulo;
    if (finVictoria) {
        titulo = "Victoria";
    } else {
        titulo = "Derrota";
    }

    const char* mensaje;
    if (finVictoria) {
        mensaje = "Has comprado el motor de plasma";
    } else {
        mensaje = "La bateria llego a 0 fuera de la base";
    }


    int marginX = px + 20;
    int marginY = py + 20;

    if (gFuenteCargada) {
        DrawTextEx(gFuente, titulo, { (float)marginX, (float)marginY }, 28, 1, borde);

        DrawTextEx(gFuente, mensaje, { (float)marginX, (float)(marginY + 40) }, 20, 1, RAYWHITE);
    } else {
        DrawText(titulo, marginX, marginY, 28, borde);
        DrawText(mensaje, marginX, marginY + 40, 20, RAYWHITE);
    }

    // Boton de reinicio de partida
    Rectangle btnReiniciar;
    btnReiniciar.x = (float)(px + 50);
    btnReiniciar.y = (float)(py + h - 70);
    btnReiniciar.width  = 150;
    btnReiniciar.height = 40;

    DrawRectangleRec(btnReiniciar, DARKGRAY);
    DrawRectangleLinesEx(btnReiniciar, 2, RAYWHITE);

    // Boton de salida del juego
    Rectangle btnSalir;
    btnSalir.x = (float)(px + w - 200);
    btnSalir.y = (float)(py + h - 70);
    btnSalir.width  = 150;
    btnSalir.height = 40;

    DrawRectangleRec(btnSalir, DARKGRAY);
    DrawRectangleLinesEx(btnSalir, 2, RAYWHITE);

    if (gFuenteCargada) {
        DrawTextEx(gFuente, "Reiniciar",{ btnReiniciar.x + 15, btnReiniciar.y + 8 },20, 1, WHITE);
        DrawTextEx(gFuente, "Salir", { btnSalir.x + 45, btnSalir.y + 8 },20, 1, WHITE);
    } else {
        DrawText("Reiniciar", (int)btnReiniciar.x + 15, (int)btnReiniciar.y + 10, 20, WHITE);
        DrawText("Salir", (int)btnSalir.x + 45, (int)btnSalir.y + 10, 20, WHITE);
    }
}
