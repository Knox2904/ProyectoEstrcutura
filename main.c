#include <stdio.h> 
#include "TDAs/list.h"
#include "TDAs/queue.h"
#include "funciones/wordle.h"
#include "Temporizador/temporizador.h"
#include "TDAs/extra.h"
#include <windows.h>

void menuGeneral() {
  limpiarPantalla();
  puts("========================================");
  puts("     Bienvenido , que desea hacer");
  puts("========================================");

  puts("1) Jugar Kahoot");
  puts("2) Jugar Wordle");
  puts("3) Salir");
}

void jugarWordle(){
    printf("a") ; 
}




int main(){
    SetConsoleOutputCP(CP_UTF8);
    char opcion ; 

    do{
        menuGeneral() ; 
        printf("Que desea hacer : \n") ; 
        scanf(" %c" , &opcion) ; 

        switch (opcion){
            case '1' :
                //jugarKahoot() ;
                break;
            case '2' :
                jugarWordle() ; 
                break;
            case '3' :
                break;
            default :
                printf("opcion  no valida , selecione una opcion valida (1 o 2)\n") ; 
                break;
        }
        presioneTeclaParaContinuar();
    }while(opcion != '3') ; 

    return 0 ; 
}