#include <xc.h>
#include <stdlib.h>
#include "retardo.h"
#include "Pic32Ini.h"
#include "LucesParpadeo.h"


#define PIN_LED_ROJO 6
#define DESPL_LED_ROJO (1 << PIN_LED_ROJO)

#define PIN_LED_AZUL 7
#define DESPL_LED_AZUL (1 << PIN_LED_AZUL)

char lucesMode = 0; // 0 = apagadas, 1 = rojas, 2 = azules, 3 = policia
char luces_apagadas = 0;


void InicializarTimer5()
{
    T5CON = 0;
    TMR5 = 0;

    PR5 = 39061; // 0.5 SEGUNDOS
    IPC5bits.T5IP = 4; // Prioridad 4
    IPC5bits.T5IS = 0; // Subprioridad 0
    IFS0bits.T5IF = 0;
    IEC0bits.T5IE = 1;
    T5CON = 0x8060;
}

void __attribute__((vector(_TIMER_5_VECTOR), interrupt(IPL4SOFT), nomips16))
InterrupcionTimer5(void)
{
    IFS0bits.T5IF = 0;
    if (lucesMode == 3) {
        invertir_luces();
    }
    else if (luces_apagadas == 0) {
        aparcar_todas_luces();
        luces_apagadas = 1;

    }
    else if (lucesMode == 1) {
        luces_rojas_parpadeo();
        luces_apagadas = 0;
    }
    else if (lucesMode == 2) {
        luces_azules_parpadeo();
        luces_apagadas = 0;
    }

}

void PararTimer5()
{
    T5CON = 0;
}


void invertir_luces() {
    LATCINV = DESPL_LED_ROJO;
    LATCINV = DESPL_LED_AZUL;
}

void incializar_luces_parpadeo() {
    LATCCLR = DESPL_LED_AZUL;
    LATCSET = DESPL_LED_ROJO;
}

void encender_todas_luces() {
    LATCSET = DESPL_LED_ROJO;
    LATCSET = DESPL_LED_AZUL;
}

void encender_luces_rojas() {
    LATCSET = DESPL_LED_ROJO;
}

void encender_luces_azules() {
    LATCSET = DESPL_LED_AZUL;
}


static void luces_policia() {
    lucesMode = 3;
    incializar_luces_parpadeo();
    IncializarTimer5();
}

static void luces_rojas_parpadeo() {
    lucesMode = 1;
    encender_luces_rojas();
    IncializarTimer5();

}

static void luces_azules_parpadeo() {
    lucesMode = 2;
    encender_luces_azules();
    IncializarTimer5();
}

static void aparcar_todas_luces() {
    lucesMode = 0;
    apagar_todas_luces();
    PararTimer5();
}

