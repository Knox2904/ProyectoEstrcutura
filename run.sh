#!/bin/bash

# Nombre del archivo fuente y ejecutable
SOURCE="main.c"
OUTPUT="mi_programa"

echo "Compilando $SOURCE..."
gcc $SOURCE -o $OUTPUT

# Verificar si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo "Compilación exitosa."
    
    # Verificar si las carpetas existen, si no, crearlas
    for dir in CSVs funciones TDAs temporizador; do
        [ -d "$dir" ] || mkdir "$dir"
    done
    
    echo "Ejecutando el programa..."
    ./$OUTPUT
else
    echo "Error en la compilación."
fi
