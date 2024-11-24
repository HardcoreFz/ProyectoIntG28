/*
Posible Declaracion e implementacion de funciones:
//-Creditos
//-Menu principal
//-Jugar
//-CargarBalas
//-Disparar
//-dispararCPU
//-DispararSiMismo
-SiguienteTurno
-RestarVida
-Ganador
-Enemigo

*/

#define MAXHP 4
#define MAXBALAS 6

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <windows.h>

//int jugador=MAXHP, dealer=MAXHP;

typedef struct balas{
    bool activa;
    struct balas* siguientebala;
}tBalas;

typedef struct tambor
{
    tBalas *inicio;
    int numbalas;
}tTambor;

/*tBalas *balas = NULL;

tTambor *tambor = NULL;
*/

void menuPrincipal(FILE *,tTambor**,int*,int*); //1-jugar 2-creadores 3.salir 4.mostrarres sesion anterior
void jugar(tTambor**,int*,int*); //inicia el bucle de juego
void creditos(); //imprime a los desarrolladores
void cargarBalas(tTambor** ptambor); //genera una lista enlazada con balas aleatorias
bool dispararJugador(tTambor** ptambor,int* pvida); //devuelve verdadadero si es balaviva y resta vida
bool dispararCPU(tTambor** ptambor,int* pvida); //devuelve verdadero si  es balaviva y resta vida
bool Disparar(tTambor** ptambor); //devuelve verdadero si es balaviva,y elimina una bala
void Ganador(bool esJugador); //muestra el ganador
void Enemigo(tTambor**,int*,int*); //elige al azar una opcion

void sesionAnterior(FILE* parchivo); //imprime la sesion anterior si existe

//TODO:void guardarsesion(FILE* parchivo); 






void menuPrincipal(FILE *parchivo,tTambor** ptambor,int* pVidaJugador,int* pVidaCPU)
{
    int opc=0;



    while(opc != 1)
    {

    printf("1.Jugar\n2.Creadores\n3.Sesion anterior\n4.Salir");

    printf("\nElige una opcion: ");
    scanf("%d",&opc);

    switch(opc)
    {
        case 1:
            jugar(ptambor,pVidaJugador,pVidaCPU);
            break;
        case 2:
            creditos();
            break;
        case 3:
            sesionAnterior(parchivo);
            break;
        case 4:
            printf("Saliendo...");
            Sleep(100);
            exit(0);
    }
    }
}

void jugar(tTambor** ptambor,int* pVidaJugador,int* pVidaCPU)
{
    cargarBalas(ptambor);
    int opc;
    bool balaviva;
    while(*pVidaJugador > 0 && *pVidaCPU > 0)
    {
        printf("\n1.Disparar al enemigo\n2.Dispararse a si mismo\n");
        printf("\nOpcion: ");
        scanf("%d",&opc);

        printf("\n");

        if(opc == 1 && (*ptambor)->inicio != NULL)
        {
            balaviva = dispararCPU(ptambor,pVidaCPU);
            if(balaviva)
            {
                printf("\nBala viva!,vida de la cpu: %d",*pVidaCPU);
            }
            else{
                printf("\nBala de fogueo,turno de la cpu");
            }
        }
        
        if(opc == 2 && (*ptambor)->inicio != NULL)
        {
            balaviva = dispararJugador(ptambor,pVidaJugador);
            if(balaviva)
            {
                printf("\nBala viva!,vida del jugador: %d",*pVidaJugador);
                Sleep(500);
            }
        }

        if((*ptambor)->numbalas == 0)
        {
            cargarBalas(ptambor);
        }

        //si la bala es viva es turno de la cpu
        //si la bala no es viva y el jugador se disparo a si mismo,turno del jugador
        if(balaviva == false && opc == 2 && (*ptambor)->inicio != NULL)
        {
            printf("\nBala de fogueo,turno extra");
            Sleep(500);
            continue;
        }

        

        if((*ptambor)->inicio != NULL)
        {
            printf("\nTurno del enemigo");
            Sleep(500);
            Enemigo(ptambor,pVidaJugador,pVidaCPU);
        }
        

        //si no quedan balas en el tambor,se vuelve a cargar
        if((*ptambor)->numbalas == 0)
        {
            cargarBalas(ptambor);
        }

    }

}

void creditos()
{
    printf("\nDesarrolladores: ");
    printf("\nGrupo 28");
    printf("\n\nGonzalez Lourdes Guadalupe");
    printf("\n\nMiño Maidana Ulises Ivan");
    printf("\n\nKarachun Pablo Ilan");
    printf("\n\nLencinas Gomez Gonzalo Ivan");
    printf("\n\nIbarra Mateo\n\n");
}

void cargarBalas(tTambor** ptambor)
{
    //genera valores booleanos aleatorios
    srand(time(NULL));

    tTambor *tambor = (tTambor*)malloc(sizeof(tTambor));

    tambor->inicio = NULL;
    tambor->numbalas = 0;

    
    tBalas * aux = NULL;

    for(int i = 0; i < MAXBALAS; i++)
    {

        tBalas *bala = (tBalas*)malloc(sizeof(tBalas));
        if(bala == NULL)
        {
            printf("\nNo se pudo crear la bala");
            exit(1);
        }

        bala->activa = rand() % 2;
        bala->siguientebala = NULL;

        if(tambor->inicio == NULL)
        {
            tambor->inicio = bala;
        }
        else
        {
           aux->siguientebala = bala;
        }

        aux = bala;

        tambor->numbalas++;
    }

    *ptambor = tambor;
}

bool dispararJugador(tTambor** ptambor,int* pvida)
{
    if(Disparar(ptambor))
    {
        *pvida -= 1;
        return true;
    }

    return false;
}

bool dispararCPU(tTambor** ptambor,int* pvida)
{
    if(Disparar(ptambor))
    {
        *pvida -= 1;
        return true;
    }

    return false;
}

bool Disparar(tTambor** ptambor)
{

    if((*ptambor)->inicio == NULL)
    {
        printf("\nNo hay balas en el tambor\n");

        (*ptambor)->numbalas = 0;

        printf("\nCargando balas...");

        Sleep(1000);

        cargarBalas(ptambor);
        
    }

    tBalas *bala = (*ptambor)->inicio;
    (*ptambor)->inicio = bala->siguientebala; 
    (*ptambor)->numbalas--;

    bool activa = bala->activa;
    free(bala); 

    return activa;

}

void Ganador(bool esJugador)
{
    if(esJugador)
    {
        printf("\nGanaste");
    }
    else
    {
        printf("\nPerdiste");
    }
}

void Enemigo(tTambor** ptambor,int* pVidaJugador,int* pVidaCPU)
{
    bool seguirturno=false;
    bool danoaljugador = false;

    do{
        

        if(rand() % 2)
        {
            printf("\nEl enemigo se disparo a si mismo");
            danoaljugador =dispararCPU(ptambor,pVidaCPU);

            Sleep(500);

            if(danoaljugador)
            {
                printf("\nBala viva,vida de la cpu: %d\n",*pVidaCPU);
                printf("\nTurno del jugador\n");
                seguirturno = false;
            }
            else
            {
                printf("\nBala de fogueo,turno de la cpu\n");
                Sleep(500);
                seguirturno = true;
            }
            
        }
        else
        {
            printf("\nEl enemigo disparo");

            Sleep(500);

            if(dispararJugador(ptambor,pVidaJugador))
            {
                printf("\nBala viva,vida del jugador: %d\n",*pVidaJugador);
                printf("\nSiguiente turno\n");
            }
            else
            {
                printf("\nBala de fogueo,turno del jugador");
                Sleep(500);
            }

            seguirturno = false;
        }

        
    }while(seguirturno);
}

void sesionAnterior(FILE* parchivo)
{
    if(parchivo != NULL)
    {
        printf("\nCargando sesion anterior...");
        Sleep(100);

        fclose(parchivo);
    }
    else
    {
        printf("\nNo hay sesion anterior");
    }
}