#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

void inicializarUART(int);
void enviarUART(char[]);
char getChar(void);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

