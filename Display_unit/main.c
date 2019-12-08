/*
 * main.c
 *
 *  Created on: 26-08-2013
 *      Author: Karol
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define cyfra4_pin (1<<PB6)
#define cyfra4_ON PORTB &= ~cyfra4_pin
#define cyfra4_OFF PORTB |= cyfra4_pin

#define cyfra3_pin (1<<PD1)
#define cyfra3_ON PORTD &= ~cyfra3_pin
#define cyfra3_OFF PORTD |= cyfra3_pin

#define cyfra2_pin (1<<PD0)
#define cyfra2_ON PORTD &= ~cyfra2_pin
#define cyfra2_OFF PORTD |= cyfra2_pin

#define cyfra1_pin (1<<PC5)
#define cyfra1_ON PORTC &= ~cyfra1_pin
#define cyfra1_OFF PORTC |= cyfra1_pin

#define pwm_pin (1<<PB1)
#define pwm_ON PORTB &= ~pwm_pin
#define pwm_OFF PORTB |= pwm_pin

#define segA_pin (1<<PD4)
#define segA_ON PORTD &= ~segA_pin
#define segA_OFF PORTD |= segA_pin

#define segB_pin (1<<PD2)
#define segB_ON PORTD &= ~segB_pin
#define segB_OFF PORTD |= segB_pin

#define segC_pin (1<<PD7)
#define segC_ON PORTD &= ~segC_pin
#define segC_OFF PORTD |= segC_pin

#define segD_pin (1<<PD5)
#define segD_ON PORTD &= ~segD_pin
#define segD_OFF PORTD |= segD_pin

#define segE_pin (1<<PB7)
#define segE_ON PORTB &= ~segE_pin
#define segE_OFF PORTB |= segE_pin

#define segF_pin (1<<PD3)
#define segF_ON PORTD &= ~segF_pin
#define segF_OFF PORTD |= segF_pin

#define segG_pin (1<<PD6)
#define segG_ON PORTD &= ~segG_pin
#define segG_OFF PORTD |= segG_pin

#define segDP_pin (1<<PB0)
#define segDP_ON PORTB &= ~segDP_pin
#define segDP_OFF PORTB |= segDP_pin

static volatile uint8_t cyfra=1, c_1=0, c_2=0, c_3=0, c_4=0, pomiar,kalibracja=0;
uint16_t liczba;

void wysw_cyfre(uint8_t x)
{


	switch(x)
	{
	 case 0:
		 segA_ON;
		 segB_ON;
		 segC_ON;
		 segD_ON;
		 segE_ON;
		 segF_ON;
		 segG_OFF;
		 segDP_OFF;
	 break;

	 case 1:
		 segB_ON;
		 segC_ON;
		 segA_OFF;
		 segD_OFF;
		 segE_OFF;
		 segF_OFF;
		 segG_OFF;
		 segDP_OFF;
	 break;

	 case 2:
		 segA_ON;
		 segB_ON;
		 segD_ON;
		 segE_ON;
		 segG_ON;
		 segC_OFF;
		 segF_OFF;
		 segDP_OFF;
	 break;

	 case 3:
		 segA_ON;
		 segB_ON;
		 segC_ON;
		 segD_ON;
		 segG_ON;
		 segE_OFF;
		 segF_OFF;
		 segDP_OFF;
	 break;

	 case 4:
		 segB_ON;
		 segC_ON;
		 segF_ON;
		 segG_ON;
		 segA_OFF;
		 segD_OFF;
		 segE_OFF;
		 segDP_OFF;
	 break;

	 case 5:
		 segA_ON;
		 segC_ON;
		 segD_ON;
		 segF_ON;
		 segG_ON;
		 segB_OFF;
		 segE_OFF;
		 segDP_OFF;
	 break;

	 case 6:
		 segA_ON;
		 segC_ON;
		 segD_ON;
		 segE_ON;
		 segF_ON;
		 segG_ON;
		 segB_OFF;
		 segDP_OFF;
	 break;

	 case 7:
		 segA_ON;
		 segB_ON;
		 segC_ON;
		 segF_OFF;
		 segD_OFF;
		 segE_OFF;
		 segG_OFF;
		 segDP_OFF;
	 break;

	 case 8:
	 	 segA_ON;
	 	 segB_ON;
	 	 segC_ON;
	 	 segF_ON;
	 	 segD_ON;
	 	 segE_ON;
	 	 segG_ON;
	 	 segDP_OFF;
	 break;

	 case 9:
	 	 segA_ON;
	 	 segB_ON;
	 	 segC_ON;
	 	 segF_ON;
	 	 segD_ON;
	 	 segG_ON;
	 	 segE_OFF;
	 	 segDP_OFF;
	 break;

	 case 10:
		 segA_ON;
		 segD_ON;
		 segE_ON;
		 segF_ON;
		 segB_OFF;
		 segC_OFF;
		 segG_OFF;
		 segDP_OFF;
	 break;

	 case 11:
		segA_ON;
		segD_OFF;
		segE_ON;
		segF_ON;
		segB_ON;
		segC_ON;
		segG_ON;
		segDP_OFF;
	 break;

	 case 12:
		segA_OFF;
		segD_ON;
		segE_ON;
		segF_ON;
		segB_OFF;
		segC_OFF;
		segG_OFF;
		segDP_OFF;
	 break;


	}
}

void wysw_liczbe(uint16_t liczba)
{
	c_1 = liczba%10;
	liczba-=c_1;

	c_2 = (liczba%100)/10;
	liczba-=c_2;

	c_3 = (liczba%1000)/100;
	liczba-=c_3;

	c_4 = (liczba%10000)/1000;

}

int main(void) {

	DDRB |= cyfra4_pin | segE_pin| segDP_pin;
	DDRD |= segA_pin|segB_pin|segC_pin|segD_pin|cyfra3_pin|cyfra2_pin|segF_pin| segG_pin;
	DDRC |= cyfra1_pin;


	cyfra1_OFF;
    cyfra2_OFF;
    cyfra3_OFF;
    cyfra4_OFF;


    //PWM
    TCCR1A |= (1<<COM1A1)|(1<<COM1A0);
    TCCR1A |= (1<<WGM10);
    TCCR1B |= (1<<WGM12)|(1<<CS11);
    DDRB   |=(1<<PB1);
    OCR1A = 0;


    //ADC
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1); // preskaler
    ADMUX |= (1<<REFS0); // nap odniesienia
    ADMUX |= (1<<ADLAR); // pom 8-bit
    ADCSRA |=(1<<ADEN); // w³ przetwornika

    //timer0
    TCCR2 |= (1<<WGM21);
    TCCR2 |= (1<<CS22)|(1<<CS21);
    OCR2 = 40;
    TIMSK |= (1<<OCIE2);
    sei();


    // SPI slave
    SPCR |=(1<<SPE);
    DDRB |=(1<<PB4);
    SPCR |=(1<<SPIE);


	while(1)
	{

		if(!(ADCSRA & (1<<ADSC)))
		{
			pomiar = ADCH;
			OCR1A = pomiar;
			ADCSRA |= (1<<ADSC);


		}

	}

}

ISR(TIMER2_COMP_vect)
{
	switch (cyfra)
	{
	case 1:
		cyfra4_OFF;
		if(kalibracja==0)wysw_cyfre(c_4);
		else wysw_cyfre(10);

		cyfra1_ON;
	break;

	case 2:
		cyfra1_OFF;
		if(kalibracja==0)wysw_cyfre(c_3);
		else wysw_cyfre(11);

		cyfra2_ON;
	break;

	case 3:
		cyfra2_OFF;
		if(kalibracja==0)wysw_cyfre(c_2);
		else wysw_cyfre(12);
		cyfra3_ON;
	break;

	case 4:
		cyfra3_OFF;
		if(kalibracja==0)wysw_cyfre(c_1);
		else wysw_cyfre(1);
		cyfra4_ON;
	break;
	}

	cyfra++;
	if(cyfra>4)cyfra=1;

}
ISR(SPI_STC_vect)
{
	uint8_t bajt;
	bajt = SPDR;

	if(bajt==255)kalibracja=1;
	else kalibracja = 0;

	if(bajt<100) liczba = bajt;
	else
	{
		liczba += bajt*100;
		wysw_liczbe(liczba);
	}
	SPDR = pomiar;
}
