#include <xc.h>
#include <stdlib.h>
#include "retardo.h"
#include "Pic32Ini.h"
#include "LucesParpadeo.h"


#define PIN_LED_ROJO 1
#define DESPL_LED_ROJO (1 << PIN_LED_ROJO)

#define PIN_LED_AZUL 0
#define DESPL_LED_AZUL (1 << PIN_LED_AZUL)

char lucesMode = 0; // 0 = apagadas, 1 = rojas, 2 = azules, 3 = policia
char luces_apagadas = 0;


void InicializarTimer5()
{
    T5CON = 0;
    TMR5 = 0;

    PR5 = 39061; // 0.5 SEGUNDOS
    IPC5bits.T5IP = 4; // Prioridad 4
    IPC5bits.T5IS = 2; // Subprioridad 0
    IFS0bits.T5IF = 0;
    IEC0bits.T5IE = 1;
    T5CON = 0x8060;
}

void __attribute__((vector(_TIMER_5_VECTOR), interrupt(IPL4SOFT), nomips16))
InterrupcionTimer5(void)
{
    LATCINV = 8;
    IFS0bits.T5IF = 0;
    if (lucesMode == 3) {
        parpadear_rojas();
        parpadear_azules();
    }
    else if (luces_apagadas == 0) {
        apagar_todas_luces();
        luces_apagadas = 1;
    
    }
    else if (lucesMode == 1) {
        parpadear_rojas();
    }
    else if (lucesMode == 2) {
        parpadear_azules();
    }

}

void PararTimer5()
{
    T5CON = 0;
}

void incializar_luces_parpadeo() {
    LATACLR = DESPL_LED_AZUL;
    LATASET = DESPL_LED_ROJO;
}

void encender_todas_luces() {
    LATASET = DESPL_LED_ROJO;
    LATASET = DESPL_LED_AZUL;
}

void encender_luces_rojas() {
    LATASET = DESPL_LED_ROJO;
}

void encender_luces_azules() {
    LATASET = DESPL_LED_AZUL;
}


void luces_policia() {
    lucesMode = 3;
    incializar_luces_parpadeo();
}

void luces_rojas_parpadeo() {
    lucesMode = 1;
}

void luces_azules_parpadeo() {
    lucesMode = 2;
}

void apagar_todas_luces() {
    lucesMode = 0;
    PararTimer5();
}

void parpadear_rojas()
{
    LATAINV = DESPL_LED_ROJO;
}

void parpadear_azules()
{
    LATAINV = DESPL_LED_AZUL;
}