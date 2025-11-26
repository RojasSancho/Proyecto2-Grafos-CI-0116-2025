# Proyecto2 Grafos CI-0116-2025
## Division de tareas
### Hermes - Grafo y Algoritmos (niveles 1 y 2)
1. Construir el grafo en memoria.
   Archivos: Grafo.h/cpp, Algoritmos.h/cpp
   * Elegir entre lista de adyacencia o matriz de adyacencia segun que tan denso es el grafo.
   * Crear estructuras que guarden: ID del nodo, tipo (vacio, base, recurso), valor de recurso, coordenadas.
   * Guardar aristas con origen, destino y peso.
   * Ejemplo: grafo[0] = [(1,10), (2,20)] → nodo 0 tiene aristas a nodo 1 con peso 10 y nodo 2 con peso 20. 
3. Implementar BFS: Busqueda en anchura y DFS: Busqueda en profundidad (Nivel 1)
    * Programar funciones que encuentren cualquier camino desde recurso a base ignorando pesos.
    * BFS: buscar por niveles.
    * DFS: buscar siguiendo un camino hasta el final antes de retroceder.
    * Probar con nodos de ejemplo y verificar que devuelvan rutas válidas.
4. Implementar Greedy y Dijkstra (Niveles 2 y 3).
    * Elegir siempre la arista más ligera al moverse.
    * Probar rutas con distintos nodos y comparar costos.te.
5. Funciones de cálculo de tuberías y costos.
    * Dado un nodo recurso, calcular tubería hacia la base según algoritmo elegido y sumar pesos.
    * Ejemplo: nodo recurso 2 → base 0: ruta = [2,1,0], costo total = 30.
    * Verificar que los resultados sean coherentes con la simulación.  
### Esteban - Simulación y mecánicas de juego
  Archivos: Jugador.h/cpp
1. Movimiento del jugador y batería
    * Mover de nodo en nodo, restar batería según peso de la arista.
    * Evitar que batería llegue a 0 fuera de la base (derrota).
2. Construcción y actualización de máquinas extractoras 
    * Permitir que al llegar a un nodo recurso se pueda “comprar” máquina.
    * Asociar cada máquina con un algoritmo (BFS/DFS, Greedy, Dijkstra y Floyd-Warshall).
    * Calcular la ruta de la tubería automáticamente.
    * Actualizar máquina: reemplazar la anterior y recalcular ruta y ganancias.
3. Gestión de recursos y economía del juego
    * Sumar recursos de cada máquina según la eficiencia de la tubería (valor del recurso – costo de mantenimiento).
    * Controlar saldo del jugador: decidir si comprar máquina nueva o actualizar una existente. 
    * Implementar condición de victoria: cuando se acumulan suficientes recursos para comprar el motor de plasma.
4. Pruebas de mecánicas de juego
    * Simular distintos escenarios: batería baja, varios recursos, rutas largas.
    * Asegurarse que los cálculos de ganancia y saldo sean correctos.

### Henry - Algoritmos Dijkstra y Floyd-Warshall (niveles 3 y 4)
Archivos: Algoritmos.h/cpp
1. Dijkstra (Nivel 3):
    * Calcular rutas óptimas y costo mínimo absoluto desde recursos a la base.
2. Floyd–Warshall (Nivel 4):
    * Calcular todos los caminos mínimos entre todos los pares de nodos.
    * Guardar en matriz de distancias para consulta rápida.
3. Pruebas independientes:
    * Usar grafo pequeño al inicio.
    * Verificar rutas y costos antes de integrar con simulación.
   
### Brandon - Desarrolla Interfaz Gráfica y Visualización
Archivos: Interfaz.h/cpp
1. Visualizar nodos, conexiones y rutas
    * Dibujar todos los nodos en pantalla según sus coordenadas.
    * Dibujar líneas entre nodos según las aristas.
    * Diferenciar base, nodos vacíos y recursos con colores o formas.
2. Diferenciar rutas según algoritmo
    * Mostrar tuberías generadas por máquinas:
      * BFS/DFS → rojo
      * Greedy → amarillo
      * Dijkstra → verde
    * Actualizar rutas en tiempo real si se actualiza la máquina.
3. Mostrar indicadores en tiempo real
    * Batería del jugador.
    * Recursos acumulados.
    * Costos de construcción y actualización de máquinas.
    * Ejemplo: barra de batería en pantalla y contador de recursos.
4. Integrar GUI con simulación
    * Cada acción del jugador (mover, construir máquina, actualizar) se refleja en la interfaz gráfica automáticamente.
    * La GUI debe leer datos del módulo de simulación: posición del jugador, rutas y estado de recursos.
