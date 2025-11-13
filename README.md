# Proyecto2 Grafos CI-0116-2025
## Division de tareas
### Integrante 1
1. Construir el grafo en memoria:
   * Elegir entre lista de adyacencia o matriz de adyacencia segun que tan denso es el grafo.
   * Crear estructuras que guarden: ID del nodo, tipo (vacio, base, recurso), valor de recurso, coordenadas.
   * Guardar aristas con origen, destino y peso.
   * Ejemplo: grafo[0] = [(1,10), (2,20)] → nodo 0 tiene aristas a nodo 1 con peso 10 y nodo 2 con peso 20. 
2. Implementar BFS: Busqueda en anchura y DFS: Busqueda en profundidad (Nivel 1)
    * Programar funciones que encuentren cualquier camino desde recurso a base ignorando pesos.
    * BFS: buscar por niveles.
    * DFS: buscar siguiendo un camino hasta el final antes de retroceder.
    * Probar con nodos de ejemplo y verificar que devuelvan rutas válidas.
3. Implementar Greedy y Dijkstra (Niveles 2 y 3).
    * Greedy: siempre elegir la arista más ligera al moverse.
    * Dijkstra: calcular camino de costo mínimo absoluto.
    * Probar rutas con distintos nodos y comparar costos para asegurarse que funcionan correctamente.
4. Probar rutas y calcular costos de tuberías.
    * Crear función que, dado un nodo recurso, calcule la tubería hacia la base usando el algoritmo elegido y sume los pesos.
    * Ejemplo: nodo recurso 2 → base 0: ruta = [2,1,0], costo total = 30.
    * Verificar que los resultados sean coherentes con la simulación.  
### Integrante 2
1. Movimiento del jugador y batería
    * Implementar función para moverse de nodo en nodo.
    * Restar batería según el peso de la arista recorrida.
    * Ejemplo: si la arista tiene peso 10, restar 10% de la batería.
    * Evitar que la batería llegue a 0 fuera de la base (derrota).
2. Construcción y actualización de máquinas extractoras
    * Permitir que al llegar a un nodo recurso se pueda “comprar” máquina.
    * Asociar cada máquina con un algoritmo (BFS/DFS, Greedy, Dijkstra).
    * Calcular la ruta de la tubería automáticamente.
    * Actualizar máquina: reemplazar la anterior y recalcular ruta y ganancias.
3. Gestión de recursos y economía del juego
    * Sumar recursos de cada máquina según la eficiencia de la tubería (valor del recurso – costo de mantenimiento).
    * Controlar saldo del jugador: decidir si comprar máquina nueva o actualizar una existente. 
    * Implementar condición de victoria: cuando se acumulan suficientes recursos para comprar el motor de plasma.
4. Pruebas de mecánicas de juego
### Integrante 3
1. Visualizar nodos, conexiones y rutas
2. Diferenciar rutas según algoritmo
3. Mostrar indicadores en tiempo real
4. Integrar GUI con simulación
