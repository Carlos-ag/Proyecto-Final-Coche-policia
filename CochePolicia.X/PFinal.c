#include <xc.h>
#include "Pic32Ini.h"


#define rueda_izq 6
#define rueda_der 9

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
    LATCCLR= (1<<rueda_izq); //ALANTE
    LATCSET= (1<<rueda_der); //ATRAS
}

void girarDer(void)
{
    LATCSET= (1<<rueda_izq); //ATRAS
    LATCCLR= (1<<rueda_der); //ALANTE
}

void pararCoche(void)
{
    OC1CON = 0;
    OC2CON = 0;
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
        OC1R = 500;
        OC1RS = 500;
        OC1CON = 0x8006;

        OC2CON = 0;
        OC2R = 500;
        OC2RS = 500;
        OC2CON = 0x8006;
    }
}

int main ( void )
{
    ANSELC &= ~0xf;
    
    LATA=0;
    LATB=0;
    LATC=0xf;
    
    TRISA=0;
    TRISB=0;
    TRISC=0;
    
    SYSKEY = 0XAA996655;
    SYSKEY = 0X556699AA;
    RPC7R = 5;
    RPC8R = 5;
    SYSKEY = 0X1CA11CA1;
    
    encenderCoche();
    
    T2CON = 0;
    TMR2 = 0;
    PR2=500;
    T2CON = 0x8000;
    
    
    while(1)
   {
        avanzar();
   }
         
}