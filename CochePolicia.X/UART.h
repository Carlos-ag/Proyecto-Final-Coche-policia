#ifndef UART_H
#define	UART_H
//#define <stdio.h>
//#define <stdlib.h>

void InterruptUART1(void);
//void enviarUART(char *p_enviar);
char getChar(void);
void inicializarUART(int baudios);


#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

