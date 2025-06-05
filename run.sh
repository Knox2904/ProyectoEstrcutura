#!/bin/bash

# Nombre del ejecutable
OUTPUT="run"

echo "Compilando..."

# Compilar todos los .c relevantes
gcc -I . funciones/*.c TDAs/*.c Temporizador/*.c main.c -o $OUTPUT

# Verificar si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo "Compilación exitosa."
    
    # Verificar si las carpetas existen, si no, crearlas
    for dir in CSVs funciones TDAs Temporizador; do
        [ -d "$dir" ] || mkdir "$dir"
    done
    
    echo "Ejecutando el programa..."
    ./$OUTPUT
else
    echo "Error en la compilación."
fi
