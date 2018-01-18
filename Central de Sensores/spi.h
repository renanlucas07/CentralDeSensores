#ifndef SPI_INCLUDED
#define SPI_INCLUDED

#define DDR_SPI  DDRB  //Arduino UNO DDRB,
#define PORT_SPI PORTB //Arduino UNO PORTB,
#define SS       2     //Arduino UNO pino 10
#define MOSI     3     //Arduino UNO pino 11
#define MISO     4     //Arduino UNO pino 12
#define SCK      5     //Arduino UNO pino 13

//Divisores do clock principal para o spi
#define SPICLOCKPRESCALER4   0
#define SPICLOCKPRESCALER16  1
#define SPICLOCKPRESCALER64  2
#define SPICLOCKPRESCALER128 3

//Atenção ao uso do pino SS/CS do avr pois seu uso como entrada quando em uso do protocolo SPI
//pode acarretar problemas impossibilitando a comunicação então sempre usar esse pino para ao menos
//um dispositivo SPI


//Para um correto funcionamento do SPI devemos antes de charmar spiSend ativar o CHIP SELECT do
//dispositivo alvo o levando a baixo.

void static inline spiStart(uint8_t SPICLOCKPRESCALER); //Incia o barramento SPI, recebe o prescaler que ira dividir o clock
                                                        //principal para funcionamento do SPI

uint8_t static inline spiSend(uint8_t data); //Envia um byte de dado e retorna um byte seguindo o funcionamento do SPI


void static inline spiStart(uint8_t SPICLOCKPRESCALER){
    outputPin(DDR_SPI,SS);    //USAMOS O SS SEMPRE PARA EVITAR PROBLEMAS DE INCOMPATIBILIDADE
    setPin(PORTB,SS);         //ATENÇÃO SE USAR SPI JAMAIS DEIXAR DE USAR O PINO SS PRINCIPALMENTE SE FOR USADO COMO ENTRADA
    outputPin(DDR_SPI,MOSI);  //MOSI SAIDA
    outputPin(DDR_SPI,SCK);   //SCK SAIDA
    setPin(SPCR,MSTR);        //MODO MASTER
    SPCR|=SPICLOCKPRESCALER;  //SETA PRESCALER
    setPin(SPCR,SPE);         //ATIVA SPI
}

uint8_t static inline spiSend(uint8_t data){
    SPDR=data;                  //ESCREVE UM BYTE NO REGISTRADOR
    while(!readPin(SPSR,SPIF)); //LE A INTERRUPT FLAG ENQUANTO NÃO TERMINA
    return SPDR;                //RETORNA O SPDR
}

#endif // PORTMANIPULATION_INCLUDED
