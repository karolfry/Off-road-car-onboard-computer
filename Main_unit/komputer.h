/*
 * komputer.h
 *
 *  Created on: 21-10-2013
 *      Author: K@ROL
 */

#ifndef KOMPUTER_H_
#define KOMPUTER_H_


//Wejcia binarne 12v
#define DIN_1_PIN (1<<PC1)
#define DIN_1 (PINC & DIN_1_PIN)

#define DIN_2_PIN (1<<PC2)
#define DIN_2 (PINC & DIN_2_PIN)

//przycisk reset
#define SW_Reset_Pin (1<<PD7)
#define SW_Reset_Down (!(PIND & SW_Reset_Pin))

//prêdkosæ UART
#define UART_BAUD 9600
#define __UBRR F_CPU/16/UART_BAUD-1


//void USART_Init (uint16_t baud);




#endif /* KOMPUTER_H_ */
