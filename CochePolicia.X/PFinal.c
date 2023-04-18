#include <xc.h>
#include "Pic32Ini.h"
#include "UART.h"
#include "COCHE.h"
#include "LucesParpadeo.h"
#include "buzzer.h"

int main ( void )
{
    ANSELA &= 0X3; //RA0 y RA1 a digital
    ANSELC &= ~0x3ff; //RC0-9 a digital
    
    LATA=0;
    LATB=0;
    LATC=0xf;
    
    TRISA=0;
    TRISB=0;
    TRISC=0;
    LATA = 0; // APAGA LAS LUCES DE POLICIA
    pararCoche();
    //inicializarCoche();
    //encenderCoche();
    //inicializarUART(9600); // UART funcionando a 9600 baudios
    

    
    //char char_enviado;
    asm("ei");
    INTCONbits.MVEC = 1;
    inicializar_buzzer();
    sound_siren();
    while(1){
        
     //luces_policia();
     
      //luces_rojas_parpadeo();
    //luces_azules_parpadeo();
    //aparcar_todas_luces();
    

    }
}
   /* while(1)
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
             
    
         