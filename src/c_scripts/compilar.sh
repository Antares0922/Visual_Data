
#!/bin/bash

# Compilar sqlite3.c en un objeto (sin warnings)
gcc -w -c Modules/sqlite3.c -o sqlite3.o

# Compilar main.cpp con enlace al objeto sqlite3.o
g++ -g main.cpp sqlite3.o -o main.exe -IModules

# Borrar archivo objeto temporal
rm sqlite3.o

# Ejecutar el programa
./main.exe

