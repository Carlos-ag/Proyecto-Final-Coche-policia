#include <xc.h>
#include "Pic32Ini.h"
#include "UART.h"
#include "COCHE.h"
#include "LucesParpadeo.h"
#include "buzzer.h"
#include "ultrasound_sensor.h"
#include "retardo.h"
int main ( void )
{
    //ANSELA &= 0X3; //RA0 y RA1 a digital 
    //ANSELC &= ~0xf;
    
    
    //LATA=0;
    //LATB=0;
    //LATC|=0xf;
    
    //TRISA=0;
    //TRISB=0;
    //TRISC=0;
    //LATA = 0; // APAGA LAS LUCES DE POLICIA
    inicializarCoche();
    
    encenderCoche();
    pararCoche();
    inicializarUART(9600); // UART funcionando a 9600 baudios
    
    inicializar_ultrasound_sensor();
    asm("ei");
    char char_enviado;
    enviarUART("Ready\n\0");
    
    //inicializar_buzzer();
    //marchaAtras();
    float dist;
    
    encenderCoche();
while(1)
    {

        char_enviado=getChar();
        
        if(char_enviado=='w')
        {
            
            enviarUART("Avanzo\n\0");     
            avanzar();
        }

        if(char_enviado=='s')
        {
            enviarUART("Marcha Atras\n\0");
            marchaAtras();
        }

        if(char_enviado=='a')
        {
            enviarUART("Girando Izquierda\n\0");
            girarIzq();
        }

        if(char_enviado=='d')
        {
            enviarUART("Girando Derecha\n\0");
            girarDer();
        }


        if(char_enviado=='n')
        {
            enviarUART("Stop\n\0");
            pararCoche();    
        }
        
        if(char_enviado=='p')
        {
            enviarUART("Policia\n\0");
            pararCoche();
        }
        
        if(get_distance()<5)
        {
            sound_siren();
            LATCCLR = 1;
        }
        else{
            //stop_siren();
            
        }
}
}

             
    
         