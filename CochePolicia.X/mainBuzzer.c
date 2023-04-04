#include <xc.h>
#include <stdlib.h>
#include "retardo.h"
#include "Pic32Ini.h"

#define BUZZER_PIN RPB15 // Buzzer pin on RPB15
#define PWM_DUTY_CYCLE 50 // Default duty cycle of the PWM signal in percent
#define PIN_LED 2 


int siren_period = 0;


int MIN_FREQ = 800;
int MAX_FREQ = 1500;
int TIME_FREQ_UP = 150;
int TIME_FREQ_DOWN = 250;
int frequency;
int STEP_UP;
int STEP_DOWN;
int button_state = 0;

// Calculates the PR2 value for a given frequency
int calculate_PR2(int frequency) {
    return (40000000 / (2 * frequency)) - 1;
}

// Sets the buzzer frequency to a given value
void sound(int frequency) {
    int high_time = (calculate_PR2(frequency) + 1) * PWM_DUTY_CYCLE / 100;

    PR2 = calculate_PR2(frequency);
    OC1RS = high_time;
}

// Initializes the PIC32 peripherals
void initialize_peripherals(void) {
    ANSELB &= ~(1 << 15);
    ANSELB &= ~(1 << 5);
    TRISB |= (1 << 5); // Set Port B5 as input
    LATA = 0;
    LATB = 0;
    LATC = 0xF;
    RPB15R = 5; // B15 -> OC1
    T2CONbits.ON = 0;
    T2CONbits.TCKPS = 0; // Prescaler 1:1
    TMR2 = 0;
    T2CONbits.ON = 1;
    OC1CONbits.ON = 0;
    OC1CONbits.OCM = 6; // PWM mode on OC1
    OC1CONbits.OCTSEL = 0; // Use Timer2 as clock source for OC1
    OC1CONbits.ON = 1;
}

// Checks if the button in Port B5 is pressed
void check_button(void) {
    if ((PORTB & (1 << 5)) == 0) {
        button_state = 1;
    } else {
        button_state = 0;
    }
}

void InicializarTimer1()
{
    T1CON = 0;
    TMR1 = 0;
    PR1 = 49999; // 10ms period
    IPC1bits.T1IP = 2; // Priority 2
    IPC1bits.T1IS = 0; // Subpriority 0
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CON = 0x8000;
}

void __attribute__((vector(_TIMER_1_VECTOR), interrupt(IPL2SOFT), nomips16)) InterrupcionTimer1(void)
{
    IFS0bits.T1IF = 0;
    LATCINV = 1<<PIN_LED;
    
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

int main(void) {
    TRISC = 0; // EL puerto C es de salida
    LATC = ~0;
    initialize_peripherals();
    InicializarTimer1();
    INTCONbits.MVEC = 1; // Modo multivector 
    asm("ei"); // Interrupciones habilitadas
    
    frequency = MIN_FREQ;
    STEP_UP = (MAX_FREQ-MIN_FREQ)/TIME_FREQ_UP;
    STEP_DOWN = (MAX_FREQ-MIN_FREQ)/TIME_FREQ_DOWN;

    while(1) {
        
        sound(frequency);
    }

    return (EXIT_SUCCESS);
}
