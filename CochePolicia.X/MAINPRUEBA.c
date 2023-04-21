/* 
 * File:   MAINPRUEBA.c
 * Author: Usuario
 *
 * Created on April 21, 2023, 8:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Pic32Ini.h"

/*
 * 
 */
int main(void) {

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
   
    OC1CON = 0;
    OC1R = 500;
    OC1RS = 500;
    OC1CON = 0x8006;
   
    OC2CON = 0;
    OC2R = 500;
    OC2RS = 500;
    OC2CON = 0x8006;
   
    T2CON = 0;
    TMR2 = 0;
    PR2=500;
    T2CON = 0x8000;
   
    //0 hacia delante, 1 hacia atras
    LATC |= (1<<6);//Rueda izquierda
    LATC &=~(1<<9);//Rueda derecha
}

