Simulador de asignacion de memoria (First/Best/Worst)

Compilacion:

```bash
g++ -std=c++17 -Wall -Wextra -o parcial "parcial.cpp"
```

Uso:

- Ejecutar interactivamente (sin argumentos):
  ./parcial
  Luego escribir comandos: A <PID> <tam>, L <PID>, M, EXIT

- Ejecutar con redirección desde archivo de comandos:
  ./parcial < tests/test2.txt

Formato de comandos en el archivo o stdin:
- A <PID> <tam>    : Asignar memoria al proceso PID
- L <PID>          : Liberar memoria del proceso PID
- M                : Mostrar estado de memoria y fragmentacion
- EXIT             : Terminar la ejecucion

Archivos de prueba (tests/): test1.txt .. test5.txt

Fragmentacion:
- El programa muestra fragmentacion externa (huecos libres) e interna (desperdicio dentro de bloques asignados).

Notas:
- Memoria minima permitida: 100
- Algoritmos soportados: First, Best, Worst

