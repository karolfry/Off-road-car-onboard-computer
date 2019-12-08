/*
 * main.c
 *
 *  Created on: 11-08-2013
 *      Author: Karol
 */


#include <avr/io.h>			// podci¹gniêcie plików nag³ówkowych
#include <util/delay.h>
#include <avr/interrupt.h>
#include "1Wire/ds18x20.h"
#include "komputer.h"
#include <avr/eeprom.h>





float EEMEM przelicznik_eeprom;
uint8_t EEMEM termometr_eeprom;
uint8_t EEMEM impulsator_eeprom=2;


uint8_t pomiar, light, light_delay=0, kalibracja=0, kali_flag=0, odleglosc_kali, reset_flag=0, impulsator=0, rem_reset=0;
uint32_t impulsy=0 , odleglosc;
uint8_t imp_flag=0;
float przelicznik = 1;

void USART_Init (uint16_t baud)
{
	//ustawienie prêdkoci
	UBRRH = (uint8_t)(baud>>8);
	UBRRL = (uint8_t)baud;

	//za³¹czenie nadajnika i odbiornika
	UCSRB = (1<<RXEN)|(1<<TXEN);
	//ustawienie formatu ramki: 8bitów, 1bit stopu
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
	//przerwanie od odbioru
	UCSRB |= (1<<RXCIE);

}

void USART_Transmit(unsigned char data){

	while(!(UCSRA & (1<<UDRE)));

	UDR = data;
}


void BT_transmit_odleg(void)
{
	uint8_t mlodsza,starsza;
	odleglosc = (uint32_t)((float)impulsy/przelicznik);
	mlodsza = odleglosc%100;
	starsza = odleglosc/100;
	USART_Transmit(starsza);
	USART_Transmit(mlodsza);

}


uint8_t SPI_transmit(uint16_t x)
{
	uint8_t mlodsza,starsza;
	mlodsza = x%100;
	starsza = (x/100)+100;

	SPDR = mlodsza;
	while(!(SPSR&(1<<SPIF))) ;
	SPDR = starsza;
	while(!(SPSR&(1<<SPIF))) ;
	return SPDR;
}

uint8_t SPI_transmit_CALIBRATION()
{
	SPDR=255;
	while(!(SPSR&(1<<SPIF))) ;
	return SPDR;
}







uint8_t termometr, term1_id=101, term2_id=103;
uint8_t znak, cal, dzies,temp;
uint8_t znak1, cal1, dzies1;


int8_t enc_delta;

uint8_t wysw_temp()
{
	uint16_t temp;
	temp = cal*10 + dzies;
	return SPI_transmit(temp);
}

void Zamien_term()
{
	uint8_t temp= term1_id;
	term1_id = term2_id;
	term2_id = temp;

}

void ReadEncoder()
{
	static int8_t last;
	int8_t newpos, diff;

	newpos=0;
	if(!(PIND & (1<<PD3))) newpos=3;
	if(!(PIND & (1<<PD2))) newpos^=1;
	diff=last-newpos;
	if(diff & 1)
	{
		last=newpos;
		enc_delta+=(diff & 2)-1;
	}
}

int8_t Read2StepEncoder()
{
	ReadEncoder();
	int8_t val=enc_delta;
	enc_delta=val & 1;
	return val>>1;
}

int main(void) {


	//spi master
	DDRB |=(1<<PB3)|(1<<PB5);
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
	// DIN
	DDRC &= ~(DIN_1_PIN | DIN_2_PIN);
	DDRD &= ~(SW_Reset_Pin);
	//EXT Interrupts
	DDRD &= ~((1<<PD2)|(1<<PD3));
	PORTD |= (1<<PD2)|(1<<PD3);


	MCUCR |= (1<<ISC10)|(1<<ISC00);
	GICR |=(1<<INT1)|(1<<INT0);
	sei();
	//USART
	USART_Init(__UBRR);

	//odczyt EEPROM
	eeprom_busy_wait();
	przelicznik = eeprom_read_float(&przelicznik_eeprom);
	if(((uint8_t)przelicznik<1) | ((uint8_t)przelicznik >250))przelicznik=1;

	eeprom_busy_wait();
	termometr = eeprom_read_byte(&termometr_eeprom);
	if(termometr>1)termometr=0;
	if(termometr==1)Zamien_term();



	eeprom_busy_wait();
	impulsator = eeprom_read_byte(&impulsator_eeprom);
	SPI_transmit(impulsator);
	_delay_ms(2000);
	if((impulsator<1) || (impulsator > 2))impulsator=1;
	///////////////////////////////



	uint8_t liczba_sensorow, DS_1_flag=0,DS_2_flag=0;;

	liczba_sensorow = search_sensors();
	uint8_t light=0;
	float light_temp;

	SPI_transmit((uint16_t)przelicznik);
	_delay_ms(2000);

	while(1){
// Pomiar i wysy³anie temperatury
			if(DS_1_flag==0 && DS_2_flag==0)
			{
			DS18X20_start_meas(DS18X20_POWER_EXTERN,NULL);
			DS_1_flag=1;
			DS_2_flag=1;
			}
			else if(DS18X20_OK == DS18X20_read_meas(gSensorIDs[0], &znak, &cal, &dzies))
			{
				DS_1_flag=0;
				USART_Transmit(term1_id);
				USART_Transmit(znak);
				USART_Transmit(cal);
				USART_Transmit(dzies);

			}

			if(DS18X20_OK == DS18X20_read_meas(gSensorIDs[1], &znak1, &cal1, &dzies1))
				{
				DS_2_flag=0;
				USART_Transmit(term2_id);
				USART_Transmit(znak1);
				USART_Transmit(cal1);
				USART_Transmit(dzies1);
				}

// wyswietlanie odleg³osci
			if(kalibracja==0)light= SPI_transmit(impulsy/przelicznik);
			else light = SPI_transmit_CALIBRATION();


// wysy³anie poziomu jasnoci do telefonu

			light_delay++;
			if(light_delay>5)
			{
			light_temp = light/2.55;
			USART_Transmit((uint8_t)light_temp);
			light_delay=0;
			}


// wciniêty przycis reset odleg³oci
			if((SW_Reset_Down & (kali_flag==0)& (reset_flag==0)) || (rem_reset==1))
			{

				USART_Transmit(104);
				BT_transmit_odleg();
				impulsy = 0;
				rem_reset=0;
				reset_flag=1;

			}
			else reset_flag=0;


// zapis ust. impulsatora do eeprom;
			if(imp_flag==1)
			{
				imp_flag=0;
				eeprom_busy_wait();
				eeprom_write_byte(&impulsator_eeprom,impulsator);
			}

			_delay_ms(100);


	}


}

SIGNAL(SIG_INTERRUPT0)
{

if(kalibracja==1) kalibracja=0;

if(impulsator==1)
	{

		if(DIN_1)
		{
			impulsy++;
			if(impulsy>9999)impulsy=9999;
		}

		else if((!DIN_1) & (impulsy>0))impulsy--;

	}
else
	{

		switch(Read2StepEncoder())
		{
		case -1: if(impulsy > 0)impulsy--;break;
		case 0 : break;
		case 1 : if(impulsy < 9999)impulsy++;break;
		}

	}
}

SIGNAL(SIG_INTERRUPT1)
{

if(impulsator==2)
	{

	if(kalibracja==1) kalibracja=0;
	switch(Read2StepEncoder())
		{
		case -1: if(impulsy > 0)impulsy--;break;
		case 0 : break;
		case 1 : if(impulsy < 9999)impulsy++;break;
		}

	}
}

SIGNAL(SIG_UART_RECV)
{
if(kali_flag==0 || kali_flag==2)
{
	switch(UDR)
	{
	case 49:
		kalibracja = 1;
		impulsy = 0;
		przelicznik=1;
		kali_flag=1;
	break;

	case 50: // zamieñ na >250
		if(kali_flag==2)
		{
		kalibracja = 0;
		przelicznik = (float)impulsy/(float)odleglosc_kali;
		if(przelicznik<1)przelicznik=1;
		eeprom_busy_wait();
		eeprom_write_float(&przelicznik_eeprom, przelicznik);
		impulsy=0;
		kali_flag=0;
		}
	break;

	case 254:
			Zamien_term();
			if(termometr==1)termometr=0;
			else termometr=1;
			eeprom_busy_wait();
			eeprom_write_byte(&termometr_eeprom,termometr);
	break;

	case 251:
		impulsator=1;
		imp_flag=1;
	break;

	case 252:
		impulsator=2;
		imp_flag=1;
	break;

	case 253:
		rem_reset=1;
	break;


	}
}
else
	{

	odleglosc_kali=UDR;

	kali_flag=2;
	}
}
