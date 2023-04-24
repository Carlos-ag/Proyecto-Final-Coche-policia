#include <xc.h>
#include "Pic32Ini.h"

#define rueda_izq 9
#define rueda_der 6

void inicializarCoche(void)
{
    TRISC=0;
    SYSKEY = 0XAA996655;
    SYSKEY = 0X556699AA;
    RPC7R = 5;
    RPC8R = 5;
    SYSKEY = 0X1CA11CA1;
    
    T2CON = 0;
    TMR2 = 0;
    PR2=2000;
    T2CON = 0x8000;
}

void encenderCoche(void)
{
    if(OC1CON!=0)
    {
        return;
    }
    
    else
    {
        OC1CON = 0;
        OC1R = 2000;
        OC1RS = 2000;
        OC1CON = 0x8006;

        OC2CON = 0;
        OC2R = 2000;
        OC2RS = 2000;
        OC2CON = 0x8006;
    }
}

void avanzar(void)
{
    encenderCoche();
    LATCCLR= (1<<rueda_izq | 1<<rueda_der); 
    
    //TO DO - ENCENDER 2 LEDS BLANCOS PARA MARCAR QUE VA MARCHA ATRAS
}


void marchaAtras(void)
{
    encenderCoche();
    LATCSET= (1<<rueda_izq | 1<<rueda_der);
    
    //TO DO - ENCENDER 2 LEDS ROJOS PARA MARCAR QUE VA MARCHA ATRAS
}

void girarIzq(void)
{
    encenderCoche();
    LATCCLR= (1<<rueda_izq); //ADELANTE
    LATCSET= (1<<rueda_der); //ATRAS
}

void girarDer(void)
{
    encenderCoche();
    LATCSET= (1<<rueda_izq); //ATRAS
    LATCCLR= (1<<rueda_der); //ADELANTE
}

void pararCoche(void)
{
    OC1CON = 0;
    OC2CON = 0;
}
