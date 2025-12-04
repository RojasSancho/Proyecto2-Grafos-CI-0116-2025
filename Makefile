make: juegoGrafo

juegoGrafo:
	g++ -std=c++17 src/main.cpp src/grafo.cpp src/jugador.cpp src/algoritmosGrafo.cpp src/interfaz.cpp -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o juegoGrafo

run:
	./juegoGrafo

clean:
	rm -f juegoGrafo
