#include <xc.h>
#include <stdlib.h>
#include "retardo.h"
#include "Pic32Ini.h"

#define PIN_LED_ROJO 6
#define DESPL_LED_ROJO (1 << PIN_LED_ROJO)

#define PIN_LED_AZUL 7
#define DESPL_LED_AZUL (1 << PIN_LED_AZUL)

void invertir_luces() {
    LATCINV = DESPL_LED_ROJO;
    LATCINV = DESPL_LED_AZUL;
}

void incializar_luces() {
    LATCCLR = DESPL_LED_AZUL;
    LATCSET = DESPL_LED_ROJO;
}

void encender_todas_luces() {
    LATCSET = DESPL_LED_ROJO;
    LATCSET = DESPL_LED_AZUL;
}

void apagar_todas_luces() {
    LATCCLR = DESPL_LED_ROJO;
    LATCCLR = DESPL_LED_AZUL;
}



int main(void)
{

    TRISC = 0;
    ANSELCCLR = (PIN_LED_ROJO|PIN_LED_AZUL);

    incializar_luces();

    uint16_t delay = 500;
    while (1)
    {
       
        
        incializar_luces();
        retardo(delay);
        
        
        invertir_luces();
        retardo(delay);
        
        
        apagar_todas_luces();
        retardo(delay);
        
        encender_todas_luces();
        retardo(delay);
         
  
    }
    return 0;
}