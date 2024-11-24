/*
PROYECTO: RULETA RUSA

OBJETIVOS:
-Crear un TDA para el proyecto
-Usar alguna estructura dinamica
-Usar archivos de texto o binarios


*/

#include "ProyectoIntegrador.h"

int vidaJugador=MAXHP, vidaCPU=MAXHP;

tTambor *tambor=NULL;
FILE * archivo=NULL;

int main()
{
    menuPrincipal(archivo,&tambor,&vidaJugador,&vidaCPU);

    if(vidaJugador <= 0)
    {
        Ganador(false);
    }
    else
    {
        Ganador(true);
    }
    

    return 0;
}