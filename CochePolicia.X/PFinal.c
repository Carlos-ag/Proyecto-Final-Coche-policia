#include <xc.h>
#include "Pic32Ini.h"
#include "UART.h"
#include "COCHE.h"

int main ( void )
{
    ANSELC &= ~0xf;
    
    LATA=0;
    LATB=0;
    LATC=0xf;
    
    TRISA=0;
    TRISB=0;
    TRISC=0;
    
   
    inicializarCoche();
    encenderCoche();
    inicializarUART(9600); // UART funcionando a 9600 baudios
    
    char char_enviado;
    while(1)
    {
        char_enviado=getChar();
        
                 if(char_enviado=="w")
        {
            avanzar();
        }

        if(char_enviado=="s")
        {
            marchaAtras();
        }

        if(char_enviado=="a")
        {
            girarIzq();
        }

        if(char_enviado=="d")
        {
            girarDer();
        }


        if(char_enviado=='\0')
        {
            pararCoche();;    
        }
        
        /*
        switch(char_enviado)
        {
            case "w":
                avanzar();
            break;

            case "s":
                marchaAtras();
            break;

            case "a":
                girarIzq();
            break;

            case "d":
                girarDer();
            break;

            default:
                pararCoche();
            break;    
        }
        */
             
    }
         
}