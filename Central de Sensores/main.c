#define F_CPU   16000000UL    //frequencia do processador
#define MAX_BUF 512           //Buffer para comunicação ethernet
#define BAUD    9600UL       //BAUD RATE para usart
#define SERVER_PORT 8000      //Porta do servidor

//AVR libs
#include <avr/io.h>
#include <util/delay.h>
//C libs
#include <string.h>
#include <stdio.h>
//low level libs
#include "portManipulation.h"
#include "spi.h"
#include "usart.h"
//sensors and modules libs
#include "wiz5200.h"
#include "DHT22.h"
#include "BMP085.h"

uint8_t mac[6]=    {0x48,0x22,0x22,0x10,0x00,0x40};
uint8_t ip[4]=     {10,1,100,188};
uint8_t gtw[4]=    {10,1,100,200};
uint8_t server[4]= {10,1,100,200};
uint8_t submask[4]={255,255,255,0};
char buffer[MAX_BUF];

uint16_t port=8080;

int main(void){
    serialStart();
    serialString("Serial Iniciada @ ");
    serialSendInt(BAUD,DEC,1);
	begin(ULTRAHIGHRES);
    dhtStart();
    if(dhtRead()){
        serialStringLN("DHT22 iniciado");
        serialString("Temperatura: ");
        serialSendInt(temperature()/10,DEC,0);
        serialSend('.');
        serialSendInt(temperature()%10,DEC,0);
        serialStringLN("C");
        serialString("Umidade: ");
        serialSendInt(humidity()/10,DEC,0);
        serialSend('.');
        serialSendInt(humidity()%10,DEC,0);
        serialStringLN("C");
        _delay_ms(2000);
    }else{
        serialStringLN("ERRO NO DHT22");
    }

    spiStart(SPICLOCKPRESCALER4);
    serialStringLN("SPI iniciado");

    if(wiz5200Init(ip,mac,gtw,submask)){
        serialStringLN("WIZ5200 iniciado");
    }


    for(;;){
        //port++;
        dhtRead();
        if(connect(server,SERVER_PORT,port)){
            strcat(buffer,"POST /API/dados/real/5a5f4f5980b416?valor=");
            char bufferTemp[10]="";
            sprintf(bufferTemp,"%d.%d",temperature()/10,temperature()%10);
            strcat(buffer,bufferTemp);
            strcat(buffer," HTTP/1.1\r\n");
            strcat(buffer,"Host: 10.1.100.188\r\n");
            strcat(buffer,"User-Agent: CentralSensores\r\n\r\n");
            serialStringLN(buffer);
            send(buffer,strlen(buffer));
            listen();
            while(receiveSize()<=0);
            uint16_t size=receiveSize();
            receive(buffer,size);
            //serialStringLN(buffer);
            serialStringLN("ok");
            strcpy(buffer,"");
            strcpy(bufferTemp,"");
            disconnect();
			
			connect(server,SERVER_PORT,port);
			strcat(buffer,"POST /API/dados/real/5a5f549861fea7?valor=");
			//char bufferTemp[10]="";
			sprintf(bufferTemp,"%ld",readPressure());
			strcat(buffer,bufferTemp);
			strcat(buffer," HTTP/1.1\r\n");
			strcat(buffer,"Host: 10.1.100.188\r\n");
			strcat(buffer,"User-Agent: CentralSensores\r\n\r\n");
			serialStringLN(buffer);
			send(buffer,strlen(buffer));
			listen();
			while(receiveSize()<=0);
			size=receiveSize();
			receive(buffer,size);
			//serialStringLN(buffer);
			serialStringLN("ok");
			strcpy(buffer,"");
			strcpy(bufferTemp,"");
			disconnect();
        }else{
            serialStringLN("Erro de conexão em execução");
            disconnect();

        }
        _delay_ms(5000);
        if(port>4000)
            port=100;
    }
    return 0;
}

