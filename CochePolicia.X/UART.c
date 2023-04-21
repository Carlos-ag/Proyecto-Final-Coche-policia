#include <xc.h>
#include "Pic32Ini.h"

#define MAX_CAD 50

static int cola_tx=0;
static int cabeza_tx=0;
static int cola_rx=0;
static int cabeza_rx=0;
static char pila_tx[MAX_CAD], pila_rx[MAX_CAD];

__attribute__( (vector(32), interrupt(IPL3SOFT), nomips16) )
        void InterruptUART1(void)
{
    if(IFS1bits.U1RXIF==1) //receptor interrumpió
    {
        LATCINV = 2;
        if ((cabeza_rx+1==cola_rx) || (cabeza_rx+1==MAX_CAD && cola_rx==0) )
        {
            ; //no hago nada, espero a tener hueco
        }
        else{
            pila_rx[cabeza_rx]=U1RXREG;
            cabeza_rx++;
            if(cabeza_rx==MAX_CAD)
            {
                cabeza_rx=0;
            }
        }        
        IFS1bits.U1RXIF=0; //Bajo el flanco
    }
    
    if(IFS1bits.U1TXIF==1) //Transmisor interrumpió, quiero mandar algo
    {        
        if (cola_tx==cabeza_tx)
        {
            //He acabado
            IEC1bits.U1TXIE=0; //Desactivo interrupciones
            //Las activare de nuevo cuando quiera mandar algo
            //IFS1bits.U1TXIF=0;//Desactivo el flanco para no entrar en bucle

        }
        else
        {
            U1TXREG=pila_tx[cola_tx];
            cola_tx++;
            if(cola_tx==MAX_CAD)
            {
                cola_tx=0;
            }
        }
        IFS1bits.U1TXIF=0; // Desactivo el flanco para evitar el bucle
    }
}
char getChar(void)
{
    char c;
    if (cola_rx!=cabeza_rx)
    {
        asm(" di");
        c= pila_rx[cola_rx];
        cola_rx++;
        if(cola_rx==MAX_CAD)
        {
            cola_rx=0;
        }
        asm(" ei");
        //enviarUART(&c);
    }
    else{
        c= '\0';
    }

    return c;
}
    
void enviarUART(char p_enviar[])
{       
    while(*p_enviar!='\0')
    {
        if ((cabeza_tx+1==cola_tx) || (cabeza_tx+1==MAX_CAD && cola_tx==0))
        {
            break; //lleno
        }
        
        else
        {
            pila_tx[cabeza_tx]=*p_enviar;
            p_enviar++;
            cabeza_tx++;
            if(cabeza_tx==MAX_CAD)
            {
                cabeza_tx=0;
            }
        }
    }
    IEC1bits.U1TXIE=1; //Activo que se pueda enviar interrupciones

}


void inicializarUART(int baudios)
{
   int sum = 0;
   int multiplier =16;
   if (baudios > 38400)
   {
       multiplier = 4;
       sum = 8;
   }
   long brg = (5000000 /(multiplier*baudios))-1;
   U1BRG = brg;

   IFS1bits.U1RXIF = 0; 
   IEC1bits.U1RXIE = 1;
   IFS1bits.U1TXIF = 0;
   IPC8bits.U1IP = 3; //Prioridad de 3
   IPC8bits.U1IS = 1; //Subprio de 1
   
   //ANSELC&= ~0xF; //leds digital
   ANSELB&= ~(1<<13)|(1<<7);
   
   //TRISA=0;
   TRISB|=  1<<7 | 1<<13;
   //TRISC=0;
   
   LATC|=0xF;

   SYSKEY=0xAA996655;
   SYSKEY=0X556699AA;
   U1RXR=3;//UART RX
   RPB7R=1;//pin 7 encendido con la UART TX
   SYSKEY=0x1CA11CA1;;
    
    
   U1STAbits.URXISEL=0; //Interrumpo en cuanto llega 1 char
   U1STAbits.UTXISEL=2; //FIFO vacia - interrupcion
   U1STAbits.UTXEN=1;
   U1STAbits.URXEN=1;
    
   U1MODE = 0x8000 +sum; //modo UMODEbits.BRGH
   
   INTCONbits.MVEC=1;
   asm("ei");
}