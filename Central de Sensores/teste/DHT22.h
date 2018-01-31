/*
 *
 *
 * Created: 25/12/2015 10:29:54
 * Author : Filipe
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "portManipulation.h"

#define DHT22 7
#define DHTPORT PORTD
#define DHTDDR DDRD
#define DHTPIN PIND

#define output setPin(DHTDDR,DHT22)
#define input clearPin(DHTDDR,DHT22)
#define read  ((DHTPIN)&(1<<DHT22))
#define high setPin(DHTPORT,DHT22)
#define low clearPin(DHTPORT,DHT22)

volatile uint16_t HM=0;
volatile uint16_t TP=0;
volatile uint8_t SUM;


void dhtStart(void){
	output;
	high;
}

uint8_t dhtRead(void){
	HM=0;
	TP=0;
	SUM=0;
	uint8_t counter2=0;
	low;
	_delay_ms(2);
	high;
	input;
	_delay_us(20);
	uint8_t control=0;
	while(read&&control<8){
        _delay_us(5);
        control++;
	}
	if(control>7){
        return 0;
	}
	while(!read);
	while(read);

   for(int counter=15;counter>=0;counter--){
	   while(!read);
	   while(read){
		   _delay_us(2);
		   counter2++;
		   }
		   if(counter2>15){
			   HM|=1<<counter;
			   }counter2=0;
			   }

   counter2=0;

   for(int counter=15;counter>=0;counter--){
	   while(!read);
	   while(read){
		   _delay_us(2);
		   counter2++;
		   }
		   if(counter2>15){
		   TP|=1<<counter;
		   }counter2=0;
		   }

   counter2=0;

   for(int counter=7;counter>=0;counter--){
	   while(!read);
	   while(read){
		   _delay_us(2);
		   counter2++;
		   }
		   if(counter2>15){
			   SUM|=1<<counter;
			   }counter2=0;
			   }
   output;
   high;

   return 1;
}


uint16_t humidity(){
	return HM;
}

uint16_t temperature(){
	return TP;
}

uint8_t checkSum(){
	return SUM;
}
