#include <xc.h> 
#include "retardo.h"

int calcular_prescaler(uint16_t tiempo);

#define PIN_LED_0 0
#define DESPL_LED_0 (1 << PIN_LED_0)

int retardo(uint16_t tiempo)
{
    int prescalers[] = {1, 2, 4, 8, 16, 32, 64, 256};
    // Primero tenemos que calcular el Prescaler
    int prescaler = calcular_prescaler(tiempo);
    
    if (prescaler == -1)
    {
        return 1;
    }
    // Inicializamos el timer 2 
    //TRISC = 0;
    T2CON = 0;
    TMR2 = 0;
    IFS0bits.T2IF = 0;
    int frecuencia = 5000;
    PR2 = tiempo * frecuencia / prescalers[prescaler] - 1; 
    // TCKPS = prescaler;
    T2CON = 0x8000 + prescaler * 16;

    while (IFS0bits.T2IF == 0)
        ;
    IFS0bits.T2IF = 0;
    T2CON = 0;
    return 0;
}

int calcular_prescaler(uint16_t tiempo)
{
    // La entrada esta en ms
    //  Posiblidades prescaler 1, 8, 64, 256 // La frecuencia del reloj es de 5MHz
    //  16 bits 0-65535
    int frecuencia = 5000;
    int holder;
    int prescalers[] = {1, 2, 4, 8, 16, 32, 64, 256}; // Loop length of prescalers
    
    int i;
    for (i = 0; i < sizeof(prescalers); i++)
    {
        holder = tiempo * frecuencia / prescalers[i] - 1;
        if (holder < 65535)
        {
            return i;
        }
    }
    return -1;
}
