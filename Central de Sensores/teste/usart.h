#ifndef USART_INCLUDED
#define USART_INCLUDED

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef BAUD
#define BAUD 9600
#endif
#define UBRR F_CPU/16/BAUD-1

#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

#include <stdlib.h>

void serialStart(void);

void serialSend(uint8_t data);

uint8_t serialReceive();

void serialString(const char stg[]);

void serialStringLN(const char stg[]);

void serialSendInt(long int data,uint8_t base, uint8_t linefeed);

void serialStart(void){
	UBRR0H=((UBRR)>>8);
	UBRR0L=UBRR;        //baud rate
	UCSR0B|=0b10000000; //interrupção em RX
	UCSR0B|=0b00011000; //ativa tx e rx
	UCSR0C|=0b00000110; //8bits e 1bits de parada no parity
}

void serialSend(uint8_t data){
	while(!(UCSR0A&(1<<UDRE0)));//Verifica se ainda há envio pendente
	UDR0=data;
}

uint8_t serialReceive(){
	while(!(UCSR0A&(1<<RXC0))); //verifica se ainda há recebimento pendente
	return UDR0;
}

void serialString(const char stg[]){
	for(uint8_t x=0;stg[x]!=0;x++){ //Enquanto não for o null terminator
		serialSend(stg[x]);         //Envia o caractere
	}

}

void serialStringLN(const char stg[]){
    serialString(stg); //Envia string
    serialSend('\r');  //Return carriage
    serialSend('\n');  //New line
}

void serialSendInt(long int data,uint8_t base,uint8_t linefeed){
    char buffer[10];
    switch (base){
        case DEC:
            sprintf(buffer,"%ld",data);
            break;
        case OCT:
            sprintf(buffer,"0x%lo",data);
            break;
        case HEX:
            sprintf(buffer,"%lX",data);
            break;
        default:
            break;
    }

    if(linefeed){
        serialStringLN(buffer);
    }else{
        serialString(buffer);
    }
}


#endif
