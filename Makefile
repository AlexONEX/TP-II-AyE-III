ej1: EJ_1.cpp
	g++ -O3 EJ_1.cpp -o ej1

ej2: EJ_2.cpp
	g++ -O3 EJ_2.cpp -o ej2

ej3: EJ_3.cpp
	g++ -O3 EJ_3.cpp -o ej3

ej4: EJ_4.cpp
	g++ -O3 EJ_4.cpp -o ej4

clean:
	rm -f instancias-4/*.out

testEj1:
	for f in $(shell ls instancias-1/*.in); do echo $${f} ; /usr/bin/time -f "Execution time: %E" ./ej1 < $${f} > /dev/null; done

testEj2:
	for f in $(shell ls instancias-2/*.in); do echo $${f} ; /usr/bin/time -f "Execution time: %E" ./ej2 < $${f} > /dev/null; done

testEj3:
	for f in $(shell ls instancias-3/*.in); do echo $${f} ; /usr/bin/time -f "Execution time: %E" ./ej3 < $${f} > /dev/null; done

testEj4:
	for f in $(shell ls instancias-4/*.in); do echo $${f} ; /usr/bin/time -f "Execution time: %E" ./ej4 < $${f} > /dev/null; done

all: ej1 ej2 ej3 ej4 testEj1 testEj2 testEj3 testEj4