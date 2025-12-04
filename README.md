# Proyecto II – Protocolo de Grafos  
---

## Requisitos del Sistema

### **1. Lenguaje y Librerías**
Este proyecto está desarrollado en **C++** y utiliza la librería gráfica **raylib**.

### **2. Dependencias necesarias**
Se debe tener instalado:

- **g++** (compatible con C++17)
- **raylib**
- Librerías del sistema usadas por raylib:
- `GL`, `m`, `pthread`, `dl`, `rt`, `X11`

---

## Instalación de Dependencias

### **Ubuntu / Debian**
```bash
sudo apt update
sudo apt install g++ make
sudo apt install libraylib-dev libraylib-doc
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev libxext-dev
```

### Puede que haga falta descargarlo manualmente
```bash
git clone https://github.com/raysan5/raylib
cd raylib
mkdir build && cd build
cmake ..
make
sudo make install
```

### 🔹 **Arch Linux**
```bash
sudo pacman -S raylib gcc make
```

### 🔹 **Windows (MSYS2 recomendado)**
```bash
pacman -S mingw-w64-x86_64-raylib
```

Asegúrese de compilar siempre desde **mingw64.exe**, no desde MSYS.



## Compilación

Este proyecto incluye un **Makefile básico**:

### **Compilar**
```bash
make
```

### **Ejecutar**
```bash
make run
```

### **Limpiar compilación**
```bash
make clean
```

---

## Mecánicas del Juego

- Te movés de nodo en nodo a través de aristas.
- Cada movimiento gasta batería según el peso de la arista.
- Si la batería llega a 0 fuera de la base: **se acaba la partida**.
- En la base se recarga al 100%.
- En máquinas instaladas se recarga hasta el 60%.
- En nodos de recurso podés instalar una máquina:
  - **Nivel 1:** BFS / DFS  
  - **Nivel 2:** Greedy (Prim)
  - **Nivel 3:** Dijkstra
  - **Nivel 4 (opcional):** Floyd/A*
- Cada máquina genera recursos en cada ciclo económico.
- **Se gana la partida** al comprar el **Motor de Plasma de Franklin Chang**.

---

## Controles

| Tecla / Acción | Función |
|----------------|---------|
| Click          | Mover jugador |
| 1              | Comprar máquina Nivel 1 (BFS) |
| 2              | Comprar máquina Nivel 1 (DFS) |
| 3              | Upgrade a Nivel 2 (Greedy) |
| 4              | Upgrade a Nivel 3 (Dijkstra) |
| 5              | Upgrade Nivel 4 |
| C              | Ciclo económico |
| M              | Comprar motor de plasma |
| H              | Mostrar ayuda |
| Ventana final  | Reiniciar o Salir |

---

## Condición de Victoria
Acumular suficientes recursos para comprar el **Motor de Plasma**.  

Una vez comprado:
- aparece ventana de victoria  
- podés **reiniciar** o **salir** del juego  

---

## Condición de Derrota
Quedarte sin batería fuera de la base.  
Se muestra una ventana con opciones:
- Reiniciar partida  
- Salir del juego  


