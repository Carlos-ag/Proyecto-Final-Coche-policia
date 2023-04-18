#include <xc.h>
#include <stdlib.h>
#include "retardo.h"
#include "Pic32Ini.h"
#include "buzzer.h"

#define PWM_DUTY_CYCLE 50 // Default duty cycle of the PWM signal in percent
#define MIN_FREQ 800
#define MAX_FREQ 1500




int siren_period = 0;

int TIME_FREQ_UP = 150;
int TIME_FREQ_DOWN = 250;
int frequency;
int STEP_UP;
int STEP_DOWN;

int calculate_PR4(int frequency) {
    return (40000000 / (2 * frequency)) - 1;
}

void sound(int frequency) {
    int high_time = ((calculate_PR4(frequency) + 1) * PWM_DUTY_CYCLE) / 100;

    PR4 = calculate_PR4(frequency);
    OC3RS = high_time;
}

void initialize_peripherals(void) {
    ANSELB &= ~(1 << 14);
    TRISB &= ~(1 << 14);

    
    SYSKEY=0xAA996655; // Se desbloquean los registros
    SYSKEY=0x556699AA;
    
    RPB14R = 5; // B14 -> OC3
    
    SYSKEY=0x1CA11CA1; // Volvemos a bloquear los registro
    
    
    T4CONbits.ON = 0;
    T4CONbits.TCKPS = 0; // Prescaler 1:1
    TMR4 = 0;
    T4CONbits.ON = 1;
    OC3CONbits.ON = 0;
    OC3CONbits.OCM = 6; // PWM mode on OC3
    OC3CONbits.OCTSEL = 1; // Use Timer4 as clock source for OC3
    OC3CONbits.ON = 1;
}

void InicializarTimer4()
{
    T4CON = 0;
    TMR4 = 0;
    PR4 = 49999; // 10ms period
    IPC4bits.T4IP = 2; // Priority 2
    IPC4bits.T4IS = 0; // Subpriority 0
    IFS0bits.T4IF = 0;
    IEC0bits.T4IE = 1;
    T4CON = 0x8000;
}

void __attribute__((vector(_TIMER_4_VECTOR), interrupt(IPL2SOFT), nomips16)) InterrupcionTimer4(void)
{
    IFS0bits.T4IF = 0;
    
    siren_period++;

    if (siren_period < TIME_FREQ_UP) {
        frequency += STEP_UP;
    }   
    else if (siren_period < (TIME_FREQ_UP+TIME_FREQ_DOWN)) {
        frequency -= STEP_DOWN;
    }
    else {
        siren_period = 0;
        frequency = MIN_FREQ;
    }
}

void calculate_variables(void) {
    frequency = MIN_FREQ;
    STEP_UP = (MAX_FREQ - MIN_FREQ) / TIME_FREQ_UP;
    STEP_DOWN = (MAX_FREQ - MIN_FREQ) / TIME_FREQ_DOWN;
}

void inicializar_buzzer(void) {
    calculate_variables();
    initialize_peripherals();
    InicializarTimer4();
}

void sound_siren() {
    sound(frequency);
}

void stop_siren() {
sound(0);
}
