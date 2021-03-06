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

uint8_t mac[6]=    {0x48,0x22,0x22,0x10,0x20,0x40};
uint8_t ip[4]=     {10,1,100,222};
uint8_t gtw[4]=    {10,1,100,200};
uint8_t server[4]= {10,1,100,200};
uint8_t submask[4]={255,255,255,0};
uint16_t port=8080;
char buffer[MAX_BUF];
char bufferTemp[10]="";
char ipString[13] = "";
char token[5][16] = { "5a6604559b3f38",
					  "5a660476cec889",
					  "5a6604841c62610",
					  "5a66049763a3911",
					  "5a66055576b2612"
					};
uint16_t size;

int main(void){
    serialStart();
    serialString("Serial Iniciada @ ");
    serialSendInt(BAUD,DEC,1);
	dhtStart();
    if(dhtRead()){
        serialStringLN("DHT22 iniciado");
        serialString("Temperatura: ");
        serialSendInt(temperature()/10,DEC,0);
        serialString(".");
        serialSendInt(temperature()%10,DEC,0);
        serialStringLN("C");
        serialString("Umidade: ");
        serialSendInt(humidity()/10,DEC,0);
        serialSend('.');
        serialSendInt(humidity()%10,DEC,0);
        serialStringLN("%");
        _delay_ms(200);
    }else{
        serialStringLN("ERRO NO DHT22");
    }
	/*
	if(!begin(ULTRAHIGHRES)) {
		_delay_ms(200);
		serialStringLN("BMP085 Iniciado");
		serialString("Pressão: ");
		serialSendInt(readPressure(),DEC,0);
		serialStringLN("Pa");
		serialString("Temperatura: ");
		serialSendInt(readTemperature()/10,DEC,0);
		serialString(".");
		serialSendInt(readTemperature()%10,DEC,0);
		serialStringLN("C");
		serialString("Altitude: ");
		serialSendInt(readAbsAltitude(),DEC,0);
		serialStringLN("m");
	} else {
		serialStringLN("ERRO NO BMP085");
	}*/

    spiStart(SPICLOCKPRESCALER4);
    serialStringLN("SPI iniciado");

    if(wiz5200Init(ip,mac,gtw,submask)){
        serialStringLN("WIZ5200 iniciado");
    }
	sprintf(ipString, "%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);

    for(;;){
        
		serialStringLN("Entrou no for");
        if(connect(server,SERVER_PORT,port)) {
			// Temperatura DHT22
			//dhtRead();
			serialStringLN("temp dht");
            strcat(buffer,"POST /API/dados/real/");
			strcat(buffer, token[0]);
			strcat(buffer, "?valor=");
            sprintf(bufferTemp,"%d.%d",temperature()/10,temperature()%10);
            strcat(buffer,bufferTemp);
            strcat(buffer," HTTP/1.1\r\n");
            strcat(buffer,"Host: ");
			strcat(buffer, ipString);
			strcat(buffer, "\r\n");
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
        } else{
            serialStringLN("Erro de conexão em execução");
            disconnect();
        }
		_delay_ms(5000);
		if (connect(server,SERVER_PORT,port)) {
			// Umidade DHT22
			//dhtRead();
			serialStringLN("Um dht");
			strcat(buffer,"POST /API/dados/real/");
			strcat(buffer, token[1]);
			strcat(buffer, "?valor=");
			sprintf(bufferTemp,"%d.%d",humidity(),((humidity() - ((int) humidity()))*10) );
			strcat(buffer,bufferTemp);
			strcat(buffer," HTTP/1.1\r\n");
			strcat(buffer,"Host: ");
			strcat(buffer, ipString);
			strcat(buffer, "\r\n");
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
		} else {
			serialStringLN("Erro de conexão em execução");
			disconnect();
		}
		_delay_ms(5000);/*
		if(connect(server,SERVER_PORT,port)) {
			// Pressão BMP085
			serialStringLN("press bmp");
			strcat(buffer,"POST /API/dados/real/");
			strcat(buffer, token[2]);
			strcat(buffer, "?valor=");
			sprintf(bufferTemp,"%ld",readPressure());
			strcat(buffer,bufferTemp);
			strcat(buffer," HTTP/1.1\r\n");
			strcat(buffer,"Host: ");
			strcat(buffer, ipString);
			strcat(buffer, "\r\n");
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
		} else {
			serialStringLN("Erro de conexão em execução");
			disconnect();
		}
		_delay_ms(5000);
		if(connect(server,SERVER_PORT,port)) {
			// Temperatura BMP85
			serialStringLN("temp bmp");
			strcat(buffer,"POST /API/dados/real/");
			strcat(buffer, token[3]);
			strcat(buffer, "?valor=");
			sprintf(bufferTemp,"%d.%d",readTemperature()/10,readTemperature()%10);
			strcat(buffer,bufferTemp);
			strcat(buffer," HTTP/1.1\r\n");
			strcat(buffer,"Host: ");
			strcat(buffer, ipString);
			strcat(buffer, "\r\n");
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
		} else {
			serialStringLN("Erro de conexão em execução");
			disconnect();
		}
		_delay_ms(5000);
		if (connect(server,SERVER_PORT,port)) {
			// Altitude BMP85
			serialStringLN("alt bmp");
			strcat(buffer,"POST /API/dados/real/");
			strcat(buffer, token[4]);
			strcat(buffer, "?valor=");
			sprintf(bufferTemp,"%d.%d",((int) readAbsAltitude()), ((int) readAbsAltitude())%10);
			strcat(buffer,bufferTemp);
			strcat(buffer," HTTP/1.1\r\n");
			strcat(buffer,"Host: ");
			strcat(buffer, ipString);
			strcat(buffer, "\r\n");
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
		} else {
			serialStringLN("Erro de conexão em execução");
			disconnect();
		}
        _delay_ms(5000);*/
        if(port>4000)
            port=100;
    }
    return 0;
}

