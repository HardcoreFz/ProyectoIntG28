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
#define MAXSESIONES 100

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

typedef struct {
    int disparoshechos;
    int disparosrecibidos;
    bool ganador;
} tSesion;


void menuPrincipal(FILE *,tTambor**,int*,int*); //1-jugar 2-creadores 3.salir 4.mostrarres sesion anterior
void jugar(tTambor**,int*,int*,tSesion*); //inicia el bucle de juego
void creditos(); //imprime a los desarrolladores
void cargarBalas(tTambor** ptambor); //genera una lista enlazada con balas aleatorias
bool dispararJugador(tTambor** ptambor,int* pvida); //devuelve verdadadero si es balaviva y resta vida
bool dispararCPU(tTambor** ptambor,int* pvida); //devuelve verdadero si  es balaviva y resta vida
bool Disparar(tTambor** ptambor); //devuelve verdadero si es balaviva,y elimina una bala
void Ganador(bool esJugador); //muestra el ganador
void Enemigo(tTambor**,int*,int*,tSesion*); //elige al azar una opcion

void guardarSesion(FILE* ,tSesion); //guarda la sesion actual

void sesionAnterior(FILE* parchivo); //imprime la sesion anterior si existe

void ordenarSesion(FILE* parchivo,tSesion); //ordena las sesiones anteriores y la actual

void mostrarRecuentoSesiones(FILE* parchivo);







void menuPrincipal(FILE *parchivo,tTambor** ptambor,int* pVidaJugador,int* pVidaCPU)
{
    int opc=0;

    tSesion Sesionact = {0,0,false}; // se usa para el registro de la sesion actual

    while(opc != 1)
    {

    printf("\n1.Jugar\n2.Creadores\n3.Sesion anterior\n4.Mostrar recuento de sesiones\n5.Salir\n");

    printf("\nElige una opcion: ");
    scanf("%d",&opc);

    switch(opc)
    {
        case 1:
            jugar(ptambor,pVidaJugador,pVidaCPU,&Sesionact);
            break;
        case 2:
            creditos();
            break;
        case 3:
            sesionAnterior(parchivo);
            break;
        case 4:
            mostrarRecuentoSesiones(parchivo);
            break;
        case 5:
           
            printf("Saliendo...");
            Sleep(100);
            exit(0);
            break;
            
    }
    }

    guardarSesion(parchivo,Sesionact);
    ordenarSesion(parchivo,Sesionact);
}

void jugar(tTambor** ptambor,int* pVidaJugador,int* pVidaCPU,tSesion *pSesionAct)
{
    cargarBalas(ptambor);
    int opc;
    bool balaviva;

    while(*pVidaJugador > 0 && *pVidaCPU > 0)
    {
        opc = 0; //reseteamos para que entre al bucle
        while(opc != 1 && opc != 2)
        {
            printf("\n1.Disparar al enemigo\n2.Dispararse a si mismo\n");
            printf("\nOpcion: ");
            scanf("%d",&opc);
        }

        printf("\n");

        if(opc == 1 && (*ptambor)->inicio != NULL)
        {
            balaviva = dispararCPU(ptambor,pVidaCPU);
            if(balaviva)
            {
                printf("\nBala viva!,vida de la CPU: %d\n",*pVidaCPU);
            }
            else{
                printf("\nBala de fogueo,turno de la CPU\n");
            }

            pSesionAct->disparoshechos++;
        }
        
        //SI la opcioon es dispararse a si mismo y el tambor no esta vacio
        if(opc == 2 && (*ptambor)->inicio != NULL)
        {
            balaviva = dispararJugador(ptambor,pVidaJugador);
            if(balaviva)
            {
                printf("\nBala viva!,vida del jugador: %d\n",*pVidaJugador);
                Sleep(500);
                pSesionAct->disparoshechos++;
                pSesionAct->disparosrecibidos++;
            }
        }

        if((*ptambor)->numbalas == 0)
        {
            cargarBalas(ptambor);
        }

        if(*pVidaJugador <= 0 || *pVidaCPU <= 0)
        {
            break;
        }

        //si la bala es viva es turno de la CPU
        //si la bala no es viva y el jugador se disparo a si mismo,turno del jugador
        if(balaviva == false && opc == 2 && (*ptambor)->inicio != NULL)
        {
            printf("\nBala de fogueo,turno extra\n");
            Sleep(500);
            continue;
        }

        

        

        if((*ptambor)->inicio != NULL)
        {
            printf("\nTurno del enemigo\n");
            Sleep(500);
            Enemigo(ptambor,pVidaJugador,pVidaCPU,pSesionAct);
        }
        

        //si no quedan balas en el tambor,se vuelve a cargar
        if((*ptambor)->numbalas == 0)
        {
            cargarBalas(ptambor);
        }

    }

    if(*pVidaJugador <= 0)
    {
        pSesionAct->ganador = false;
    }
    else
    {
        pSesionAct->ganador = true;
    }

}

void creditos()
{
    printf("\nDesarrolladores: ");
    Sleep(300);
    printf("\nGrupo 28");
    Sleep(300);
    printf("\n\nGonzalez Lourdes Guadalupe");
    Sleep(300);
    printf("\n\nMino Maidana Ulises Ivan");
    Sleep(300);
    printf("\n\nKarachun Pablo Ilan");
    Sleep(300);
    printf("\n\nLencinas Gomez Gonzalo Ivan");
    Sleep(300);
    printf("\n\nIbarra Mateo\n\n");
}

void cargarBalas(tTambor** ptambor)
{
    //genera valores booleanos aleatorios
    srand(time(NULL));

    tTambor *tambor = (tTambor*)malloc(sizeof(tTambor));

    tambor->inicio = NULL;
    tambor->numbalas = 0;

    int fogueo=0,vivas=0;

    
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

        if(bala->activa == 0)
        {
            fogueo++;
        }
        else
        {
            vivas++;
        }

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

    printf("\nBalas cargadas: %d",tambor->numbalas);
    printf("\nBalas vivas: %d",vivas);
    printf("\nBalas de fogueo: %d",fogueo);
    printf("\n\n");
    Sleep(1000);
}

//estas dos funciones deberian de ser solo 1,y cambiarian los argumentos,lit son las misma cosa
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

        printf("\nCargando balas...\n");

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

void Enemigo(tTambor** ptambor,int* pVidaJugador,int* pVidaCPU,tSesion* pSesionAct)
{
    bool seguirturno=false;
    bool danoaljugador = false;

    do{
        

        if(rand() % 2)
        {
            printf("\nEl enemigo se disparo a si mismo\n");
            danoaljugador =dispararCPU(ptambor,pVidaCPU);

            Sleep(500);

            if(danoaljugador)
            {
                printf("\nBala viva,vida de la CPU: %d\n",*pVidaCPU);
                printf("\nTurno del jugador\n");
                seguirturno = false;

            }
            else
            {
                printf("\nBala de fogueo,turno de la CPU\n");
                Sleep(500);
                seguirturno = true;
            }
            
            pSesionAct->disparoshechos++;
        }
        else
        {
            printf("\nEl enemigo disparo\n");

            Sleep(500);

            if(dispararJugador(ptambor,pVidaJugador))
            {
                printf("\nBala viva,vida del jugador: %d\n",*pVidaJugador);
                printf("\nSiguiente turno\n");
                
                pSesionAct->disparosrecibidos++;
            }
            else
            {
                printf("\nBala de fogueo,turno del jugador\n");
                Sleep(500);
                
            }

            pSesionAct->disparoshechos++;

            seguirturno = false;
        }

        
    }while(seguirturno);
}

void sesionAnterior(FILE* parchivo)
{

    parchivo = fopen("sesiones.dat","rb");

    if(parchivo == NULL)
    {
        printf("\nNo hay sesion anterior o error al abrir el archivo");
        fclose(parchivo);
        return;
    }
   
    printf("\nSesion anterior:\n");

    tSesion sesion;

    //va a la sesion anterior,desde el final del archivo
    fseek(parchivo,-sizeof(tSesion),SEEK_END);

    if(fread(&sesion,sizeof(tSesion),1,parchivo) == 1)
    {
        printf("\nResultados de la sesion anterior:\n");

        printf("\nDisparos hechos: %d",sesion.disparoshechos);
        printf("\nDisparos recibidos: %d",sesion.disparosrecibidos);
        printf("\nGanador: %s",sesion.ganador ? "Jugador" : "CPU");
    }
    else
    {
        printf("\nNo se pudo leer la sesion anterior");
    }
    
    fclose(parchivo);
}

void guardarSesion(FILE* parchivo,tSesion sesion)
{
    parchivo = fopen("sesiones.dat","ab");

    if(parchivo == NULL)
    {
        printf("\nError al abrir el archivo");
        fclose(parchivo);
        return;
    }

    fwrite(&sesion,sizeof(tSesion),1,parchivo);

    printf("\nSesion guardada\n");
    Sleep(1);

    fclose(parchivo);


}

void ordenarSesion(FILE* parchivo,tSesion sesion)
{

    // Abrir archivo en modo lectura para cargar datos
    parchivo = fopen("sesiones.dat", "rb");

    if (parchivo == NULL) {
        printf("\nError al abrir el archivo para lectura");
        return;
    }

    tSesion sesiones[MAXSESIONES];
    int contadorses = 0;

    // Leer las sesiones en memoria
    while (fread(&sesiones[contadorses], sizeof(tSesion), 1, parchivo) == 1) {
        contadorses++;
        if (contadorses >= MAXSESIONES) break; // Evitar desbordamiento
    }

    fclose(parchivo); // Cerrar después de lectura

    // Ordenar sesiones
    for (int i = 0; i < contadorses - 1; i++) {
        for (int j = i + 1; j < contadorses; j++) {
            // Priorizar ganador (true primero)
            if (sesiones[i].ganador < sesiones[j].ganador ||
                // Si ambos son iguales, ordenar por disparos hechos (descendente)
                (sesiones[i].ganador == sesiones[j].ganador &&
                 sesiones[i].disparoshechos < sesiones[j].disparoshechos)) {
                tSesion temp = sesiones[i];
                sesiones[i] = sesiones[j];
                sesiones[j] = temp;
            }
        }
    }

    // Abrir archivo en modo escritura para sobrescribir datos ordenados
    parchivo = fopen("sesiones.dat", "wb");

    if (parchivo == NULL) {
        printf("\nError al abrir el archivo para escritura");
        return;
    }

    // Escribir las sesiones ordenadas al archivo
    for (int i = 0; i < contadorses; i++) {
        fwrite(&sesiones[i], sizeof(tSesion), 1, parchivo);
    }

    fclose(parchivo); // Cerrar archivo después de escribir

    printf("\nSesiones reordenadas y guardadas correctamente\n");

    

}

void mostrarRecuentoSesiones(FILE* parchivo)
{

    parchivo = fopen("sesiones.dat","rb");

    if(parchivo == NULL)
    {
        printf("\nError al abrir el archivo");
        fclose(parchivo);
        return;
    }

    printf("\nRecuento de sesiones:\n");

    tSesion sesion,sesionant;

    fread(&sesion,sizeof(tSesion),1,parchivo);

    sesionant = sesion;

    int ganadorJugador = 0, ganadorCPU = 0;
    int balastotales = 0;
    int numsesiones = 0;

    while(!feof(parchivo))
    {
        if(sesion.ganador)
        {
            ganadorJugador++;
        }
        if(!sesion.ganador)
        {
            ganadorCPU++;
        }

        balastotales += sesion.disparoshechos;
        balastotales += sesion.disparosrecibidos;

        numsesiones++;

        if(fread(&sesion,sizeof(tSesion),1,parchivo)!=1)
        {
            break;
        }
    }

    printf("\nNumero de sesiones: %d",numsesiones);
    printf("\nBalas totales: %d",balastotales);
    printf("\nVeces que gano el jugador: %d",ganadorJugador);
    printf("\nVeces que gano la CPU: %d",ganadorCPU);

    fclose(parchivo);

}
