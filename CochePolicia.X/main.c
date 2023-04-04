#include <xc.h>
#include <sys/attribs.h>

#define PIN_TRIG_ULTRASOUND 4
#define DESPL_TRIG_ULTRASOUND 1<<PIN_TRIG_ULTRASOUND

#define PIN_ECHO_ULTRASOUND 5
#define DESPL_ECHO_ULTRASOUND 1<<PIN_ECHO_ULTRASOUND

#define PIN_LED_0 0
#define PIN_LED_1 1 
#define PIN_LED_2 2 
#define PIN_LED_3 3 

#define DESPL_LED_0 (1 << PIN_LED_0)
#define DESPL_LED_1 (1 << PIN_LED_1)
#define DESPL_LED_2 (1 << PIN_LED_2)
#define DESPL_LED_3 (1 << PIN_LED_3)

int ticks_echo = 0;
char has_sent_trig = 0;
char has_return_trig_zero = 0;
float distance = 10000;

void inicializar_timer1()
{
    T1CON = 0;
    TMR1 = 0;
    PR1 = 49; //10 us
    IPC1bits.T1IP = 2; // Prioridad 2 
    IPC1bits.T1IS = 0; // Subprioridad 0 
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;

    T1CON = 0x8000;    
}

void parar_timer1()
{
    IEC0bits.T1IE = 0;
    T1CON = 0;
}

void reiniciar_variables_trig() {
    has_sent_trig = 0;
    has_return_trig_zero = 0;
}


void inicializar_timer2()
{
    T2CON = 0;
    TMR2 = 0;
    PR2 = 1953; // 1 SEGUNDOS 19531
    IPC2bits.T2IP = 3; // Prioridad 2 
    IPC2bits.T2IS = 0; // Subprioridad 0 
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    T2CON = 0x8070;
}

void parar_timer2() {
    T2CON = 0;
}

void __attribute__((vector(_TIMER_2_VECTOR), interrupt(IPL3SOFT), nomips16))
InterrupcionTimer2(void)
{
    
    IFS0bits.T2IF = 0;
    reiniciar_variables_trig();
    inicializar_timer1();
    parar_timer2();

    
}


void enable_interrupciones_echo() {
        

    // Disable Change Notice (CN) functionality before configuration
    CNCONCbits.ON = 0;

    // Enable CN interrupt for RC5
    CNENCbits.CNIEC5 = 1;

    // Clear CN interrupt status flag
    IFS1CLR = _IFS1_CNCIF_MASK;

    // Set priority and sub-priority of CN interrupt
    IPC8bits.CNIP = 4;
    IPC8bits.CNIS = 0;

    // Enable Change Notice (CN) functionality
    CNCONCbits.ON = 1;

    // Enable CN interrupts
    IEC1bits.CNCIE = 1;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL4AUTO) CN_Interrupt(void) {

    if ((PORTC >> PIN_ECHO_ULTRASOUND) & 1) {
        ticks_echo = 0;
        inicializar_timer1();
    }   
    else {
        parar_timer1();
        distance = ticks_echo * (10.0 / 58.0);
        inicializar_timer2();
    }

    // Clear the CN interrupt status flag
    IFS1CLR = _IFS1_CNCIF_MASK;
}

void __attribute__((vector(_TIMER_1_VECTOR), interrupt(IPL2SOFT), nomips16)) InterrupcionTimer1(void)
{ 
    
    IFS0bits.T1IF = 0;
    
    // genera el pulso de 10us
    if (has_sent_trig == 0) {
        LATCSET = DESPL_TRIG_ULTRASOUND;
        has_sent_trig = 1;
    }
    else if (has_return_trig_zero == 0) {
        LATCCLR = DESPL_TRIG_ULTRASOUND;
        has_return_trig_zero = 1;
        parar_timer1();
        enable_interrupciones_echo();
    }
    // aqui termina la generacion del pulso de 10us

    // si vuelve a entrar en el timer es porque ha recibido el pulso de echo
    else {
        ticks_echo++;
    }   
}








void main(void) {
    
    TRISC &= ~(DESPL_TRIG_ULTRASOUND); // Set trigger pin as output
    TRISC |= DESPL_ECHO_ULTRASOUND; // Set echo pin as input

    ANSELC &= ~(DESPL_TRIG_ULTRASOUND); // Set trigger pin as digital
    ANSELC &= ~(DESPL_ECHO_ULTRASOUND); // Set echo pin as digital


    TRISC &= ~(DESPL_LED_0 | DESPL_LED_1 | DESPL_LED_2 | DESPL_LED_3); // definimos los puertos como salidas
    ANSELC &= ~(DESPL_LED_0 | DESPL_LED_1 | DESPL_LED_2 | DESPL_LED_3); // definimos las salidas como digitales
    LATCSET = (DESPL_LED_0 | DESPL_LED_1 | DESPL_LED_2 | DESPL_LED_3);
    



    INTCONbits.MVEC = 1; // Modo multivector 
    asm("ei"); // Interrupciones habilitadas
    

    inicializar_timer2();

    while(1) {
        int i = distance;
        if (i < 5) {
            LATCCLR = DESPL_LED_0;
            LATCSET = DESPL_LED_1 | DESPL_LED_2 | DESPL_LED_3;
        }
        else if (i < 10) {
            LATCCLR =DESPL_LED_1;
            LATCSET = DESPL_LED_0 | DESPL_LED_2 | DESPL_LED_3;
        }
        else if (i < 15) {
            LATCCLR = DESPL_LED_2;
            LATCSET = DESPL_LED_0 | DESPL_LED_1 | DESPL_LED_3;
        }
        else {
            LATCCLR = DESPL_LED_3;
            LATCSET = DESPL_LED_0 | DESPL_LED_1 | DESPL_LED_2;
        }
    }
}