# ProyectoEstrcutura
Pon a Prueba tu Mente - Suite de Minijuegos en C

Descripción
Pon a Prueba tu Mente es una aplicación de consola desarrollada en C que ofrece una suite de dos minijuegos clásicos: Kahoot y Wordle. El proyecto nace como un ejercicio práctico para aplicar y demostrar el uso eficiente de Estructuras de Datos Abstractas (TDAs) en la construcción de una aplicación funcional e interactiva desde cero.

✅ Características Principales:

Suite de Juegos: Accede a dos juegos completos desde un único menú principal.

Juego de Kahoot:

Carga de un banco de 55 preguntas variadas desde un archivo CSV.

Sistema de juego con 10 preguntas aleatorias por partida.

Límite de tiempo de 20 segundos por pregunta para añadir un desafío.

Puntuación dinámica basada en la correctitud y la rapidez de la respuesta.

Historial persistente de las últimas 10 partidas jugadas.

Juego de Wordle:

Múltiples dificultades (Fácil, Normal, Extremo) que ajustan el largo de la palabra y los intentos.

Un modo "Infinito" que encadena palabras de todas las dificultades.

Feedback visual con letras coloreadas para indicar aciertos.

Sistema de guardado de partidas y estadísticas en un archivo CSV.

Visor de historial para repasar partidas anteriores y ver estadísticas de victorias.

Opción para resetear todo el historial.


## Compilación y Ejecución

### Requisitos

-Tener instalado un compilador de C, como GCC. -Un editor de texto o IDE para trabajar con el código fuente.

### Compilar
Clonar el Repositorio: Para compilar y ejecutar el programa, sigue estos pasos:

Asegúrate de tener un compilador de C instalado en tu sistema (como GCC). Descarga los archivos del proyecto y colócalos en un mismo directorio. Compila el programa utilizando el siguiente comando en terminal git bash: ejm : cd "/c/Users/fcast/Desktop/Programacion/Proyecto/ProyectoEstrcutura" para hacer eso hay que ver en que carpeta se coloco el programa, entrar en la carpeta y luego copiar la direccion y hacer los cambios respectivos a la direccion, ya que originalmente es asi : C:\Users\fcast\Desktop\Programacion\Proyecto\ProyectoEstrcutura y deberias ser asi : /c/Users/fcast/Desktop/Programacion/Proyecto/ProyectoEstrcutura Ejecuta el programa con: ./run.sh


## Estructura del Proyecto

- **main.c**: Contiene el menú principal que permite al usuario elegir entre Kahoot, Wordle o salir. Orquesta la lógica general.

- **kahoot.c / kahoot.h**: Implementa toda la lógica del juego Kahoot, desde la carga de preguntas hasta el cálculo de puntajes y el historial.

- **wordle.c / wordle.h**: Implementa la lógica del juego Wordle, incluyendo las dificultades, la evaluación de intentos y la gestión del historial.

## TDAs/: Carpeta que contiene las implementaciones de las Estructuras de Datos.

- **/list.c / list.h**: TDA Lista enlazada genérica.

- **map.c / map.h**: TDA Mapa asociativo, construido sobre la lista.

- **stack.c / stack.h**: TDA Pila (Stack), construido sobre la lista.

- **extra.c / extra.h**: Funciones utilitarias como limpiarPantalla() y presioneTeclaParaContinuar().

- **CSVs/**: Carpeta que contiene los archivos de datos para ambos juegos.

- **kahoot_55_preguntas.csv**: Banco de preguntas para Kahoot.

- **wordle_*.csv**: Archivos con las palabras para cada dificultad de Wordle.

- **historial_WordleChad.csv**: Archivo donde se guarda el historial de Wordle.

- **README.md**: Este archivo.


Estructuras de Datos Utilizadas (TDAs)
El núcleo del proyecto es la implementación y uso de TDAs para gestionar la información de manera eficiente.

### TDA Lista (`List`)
```c

typedef struct List {
  Node *head;
  Node *tail;
  Node *current;
  int size;
} List;
```
Uso: Es la estructura de datos fundamental del proyecto.

En Kahoot, se usa para cargar y almacenar la lista completa de preguntas desde el archivo CSV. La función aleatorizarPreguntas opera sobre esta lista para asegurar que cada partida sea diferente.

Sirve como la base para implementar los TDAs Mapa y Pila, demostrando la reutilización de código y la composición de estructuras.

### TDA Mapa (`Map`)

```c
typedef struct {
  void *key;
  void *value;
} MapPair;

typedef struct Map {
  int (*lower_than)(void *key1, void *key2);
  int (*is_equal)(void *key1, void *key2);
  List *ls;
} Map;
```
Uso: Implementado sobre una lista, este TDA es clave en la función de historial de Wordle.

Se utiliza para almacenar las partidas guardadas, asociando un número de partida (la clave) con todos los datos de esa partida (el valor). Esto permite buscar y mostrar una partida específica de manera rápida y eficiente cuando el usuario desea ver los detalles.

### TDA Pila (`Stack`)
```c

 //Definido como un puntero a una Lista
typedef List Stack;
```

Uso: Este TDA, implementado sobre la lista, tiene dos roles distintos y cruciales en el programa:

En Kahoot, se usa para gestionar el historial de partidas. Al ser una estructura LIFO (Last-In, First-Out), es perfecta para guardar y mostrar las partidas más recientes primero.

En el modo "Infinito" de Wordle, se usa para crear el mazo de desafíos. Se cargan y barajan palabras de todas las dificultades y se apilan. En cada ronda, se "saca" (pop) una nueva palabra de la cima de la pila para que el jugador la resuelva.

### Estructuras principales del juego, primero kahoot , luego Wordle
```c
// tipos de preguntas
typedef enum {
    MULTIPLE_CHOICE = 0,
    TRUE_FALSE = 1
} TipoPregunta;

// estructura para una pregunta
typedef struct {
    char pregunta[MAX_PREGUNTA];
    char respuestas[4][MAX_RESPUESTA];  // para multiple 
    int respuesta_correcta;  // 0-3 para multiple, 0-1 para V/F
    TipoPregunta tipo;
    char categoria[MAX_CATEGORIA];
} Pregunta;

// partida completa
typedef struct {
    time_t fecha;
    int puntaje_total;
    int preguntas_correctas;
    int total_preguntas;
    float tiempo_promedio;
} Partida;
esto fue kahoot, ahora Wordle

typedef struct{
    
    char** TablaWordle ; // usa la fila como intento para poner ahi la palabra 
    char*  palabraObjetivo ; // se explica solo , es la palabra que el jugador adivina 
    int maxIntentos ;   // la cantidad de intentos maximos 
    int largoPalabra ;  // el largo de la palabra , se usa para controlar que lo que el jugador quiero adivinar sea del mismo largo que la palabra a adivinar 
    int intentoActual ;  // usalo para saber la fila en donde escribir 

}Wordle;

typedef struct {
    int numero_partida; // numero de la partida
    char* palabraObjetivo; // igual que el struct de arriba, se explica solo
    char** intentos; // cantidad de intentos, esta en dinamico por la variabilidad segun dificultad
    int numIntentosRealizados; // cantidad de intentos del usuario
    int largoPalabra;// el largo de la palabra
    bool gano; // flag para ver si gano o no
} PartidaGuardada;

```
## FUNCIONAMIENTO PRINCIPAL

El programa se inicia y presenta un menú principal con tres opciones: Jugar Kahoot, Jugar Wordle o Salir.

Si se elige Kahoot:

Se muestra un menú con las instrucciones.

Al empezar, el juego carga las preguntas, las baraja y presenta 10 de ellas al jugador.

El jugador responde contrarreloj. Al final, se muestra el puntaje y los resultados.

La partida se guarda en el historial (TDA Pila).

Si se elige Wordle:

El jugador ve las reglas y elige una dificultad (incluyendo el modo Infinito).

El juego carga una palabra aleatoria (o una pila de palabras para el modo infinito).

El jugador ingresa intentos y recibe feedback con colores.

Al ganar o perder, la partida se guarda en el archivo historial_WordleChad.csv.

Desde el menú de Wordle, el jugador puede acceder al historial para ver estadísticas y repeticiones de partidas (usando el TDA Mapa).

## Ejemplo de Menús
Menú Principal

```
========================================
                  Bienvenido
========================================
1) Jugar Kahoot
2) Jugar Wordle
3) Salir
```

Menú de Kahoot
```
========================================
           Bienvenido a Kahoot
========================================

Instrucciones del Kahoot:
- Tienes 20 segundos para responder cada pregunta.
- Si no respondes a tiempo, se considerará incorrecta.
- Solo responderás 10 preguntas por partida.
- Preguntas de selección múltiple y verdadero/falso.
1. Empezar!
2. Ver Historial de Partidas
3. Salir
```

Menú de Wordle

```
========================================
           Seleccione dificultad
========================================
1) Facil 🌴
2) Normal ⚡
3) Extremo 💀
4) Pruebate a ti mismo 💪
5) Ver Historial y Estadisticas 📊
6) Resetear Estadísticas 🗑️
7) Salir
```
### Créditos

## equipo Wordle:

Gabriel López : hizo funciones la estructura del wordle, el esqueleto, y la estructura del main, tambien hizo funciones de wordle como: 
la funcion infinito, crearWordle, GuardarIntento, DestruirWordle, ReglasWordle, etc, en general el esqueleto del wordle, ademas de realizar cambios generales y solucion de errores al wordle.c.

Felipe Castro: hizo funciones para que se imprimiera el wordle, se evaluara el intento, se vereficara si la posicion de la letra era correcta, etc y solucion a errores en wordle.c, sus funciones fueron: MostrarHistorial y toda funcion que este relacionada con esta, Mostrar WordleColoreado, ImprimirResultado, evaluarIntento.

## Equipo Kahoot:

Luis Gúzman: hizo funciones como menuKahoot, Mostrar_pregunta, obtener_respuesta_usuario_con_tiempo, calcular_puntaje, en general el esqueleto de kahoot, tambien se dedico a revisar errores y areglar en kahoot, tambien ayudo con las creaciones de csv de ambos juegos y algunas funciones para leer csv.

Juan Araya : hizo contribuciones importantes en csv, como leerlos, como escribirlos, e ideas de que como dividir dificultades en temas de wordle, y preguntas en kahoot. en kahoot ayudo a implementar el historial y a mostrar los resultados tanto, como del historial como de las partidas en generales, las funciones fueron: mostrar_resultado_final, cargar_preguntas_csv, mostrar_historial, jugar_kahoot.
tambien ayudo a testear rigurosamente las funcionalidades de ambos videojuegos. 


*****contribuciones de IAS******
varias IAs Ayudaron en este proyecto, por ejemplo copilot ayudo con la idea general de como leer csv, de como se deberian modelar las funciones, que tdas son los mas efectivos, solucion de errores, etc.
chat gpt ayudo a dar ideas de como generar el csv, como ordenarlo, etc.
gemini ayudo a ver la aplicacion en general y a ver como generar el historial de wordle, tambien ayudo a ver posibles errores que traerian algunas funciones y como solucionarlos.

## Notas Adicionales
El programa fue desarrollado y probado en un entorno Windows. Utiliza bibliotecas específicas como windows.h y conio.h para el manejo de colores en la consola, la limpieza de pantalla y la captura de teclas sin necesidad de presionar "Enter", por lo que su compilación está orientada a este sistema operativo.

## Notas Finales
- si es que no se hace lo que pide, por ejemplo si es que escribes una letra en vez de un numero en la partida de kahoot cuenta como incorrecta, en wordle te tira error.

- si es que escribes una solo letra en wordle te pueden rellenar con una aleatoria, no se sabe por que.

- a veces la primera letra en wordle no se toma.

- si es que tienes una pregunta correcta en kahoot y no pasas, el tiempo va a contar para el tiempo promedio en tu historial.

- el historial de Wordle te puede dar problemas con el antivirus al generar un nuevo archivo y editarlo. si no das permiso puede que no te deje jugar y te de error de compilacion, estamos intentando solucionar esto.

- el historial de kahoot a veces solo puede guardar de 4 - 10 partidas, incluso a veces llegando a mas, pero no se sabe por que.

