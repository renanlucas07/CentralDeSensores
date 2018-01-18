// Wiznet W5200 Op Code
#define WIZNET_WRITE_OPCODE 0x80
#define WIZNET_READ_OPCODE  0x00
// Wiznet W5200 Register Addresses
#define MR         0x0000      // Mode Register
#define GAR        0x0001      // Gateway Address: 0x0001 to 0x0004
#define SUBR       0x0005      // Subnet mask Address: 0x0005 to 0x0008
#define SHAR       0x0009      // Source Hardware Address (MAC): 0x0009 to 0x000E
#define SIPR       0x000F      // Source IP Address: 0x000F to 0x0012
#define RMSR       0xC000      // RX Memory Size Register
#define TMSR       0x8000      // TX Memory Size Register
// Wiznet W5200 Socket Register Addresses <<<<-----    MUDOU
#define S0_MR      0x4000      // Socket 0: Mode Register Address
#define S0_CR      0x4001      // Socket 0: Command Register Address
#define S0_IR      0x4002      // Socket 0: Interrupt Register Address
#define S0_SR      0x4003      // Socket 0: Status Register Address
#define S0_PORT    0x4004      // Socket 0: Source Port: 0x0404 to 0x0405
#define SO_TX_FSR  0x4020      // Socket 0: Tx Free Size Register: 0x0420 to 0x0421
#define S0_TX_RD   0x4022      // Socket 0: Tx Read Pointer Register: 0x0422 to 0x0423
#define S0_TX_WR   0x4024      // Socket 0: Tx Write Pointer Register: 0x0424 to 0x0425
#define S0_RX_RSR  0x4026      // Socket 0: Rx Received Size Pointer Register: 0x0425 to 0x0427
#define S0_RX_RD   0x4028      // Socket 0: Rx Read Pointer: 0x0428 to 0x0429
#define S0_DIPR    0x400C      // Socket 0: Destination IP 0x400C ate 0x400F
#define S0_DPORT   0x4010      // Socket 0: Destination PORT 0x4010 ate 0x4011

#define TXBUFADDR  0x8000      // W5100 Send Buffer Base Address
#define RXBUFADDR  0xC000      // W5100 Read Buffer Base Address

// S0_MR values
#define MR_CLOSE      0x00    // Unused socket
#define MR_TCP        0x01    // TCP
#define MR_UDP        0x02    // UDP
#define MR_IPRAW      0x03    // IP LAYER RAW SOCK
#define MR_MACRAW     0x04    // MAC LAYER RAW SOCK
#define MR_PPPOE      0x05    // PPPoE
#define MR_ND         0x20    // No Delayed Ack(TCP) flag
#define MR_MULTI      0x80    // support multicating
// S0_CR values
#define CR_OPEN          0x01     // Initialize or open socket
#define CR_LISTEN        0x02     // Wait connection request in tcp mode(Server mode)
#define CR_CONNECT       0x04     // Send connection request in tcp mode(Client mode)
#define CR_DISCON        0x08     // Send closing reqeuset in tcp mode
#define CR_CLOSE         0x10     // Close socket
#define CR_SEND          0x20     // Update Tx memory pointer and send data
#define CR_SEND_MAC      0x21     // Send data with MAC address, so without ARP process
#define CR_SEND_KEEP     0x22     // Send keep alive message
#define CR_RECV          0x40     // Update Rx memory buffer pointer and receive data
// S0_SR values
#define SOCK_CLOSED      0x00     // Closed
#define SOCK_INIT        0x13     // Init state
#define SOCK_LISTEN      0x14     // Listen state
#define SOCK_SYNSENT     0x15     // Connection state
#define SOCK_SYNRECV     0x16     // Connection state
#define SOCK_ESTABLISHED 0x17     // Success to connect
#define SOCK_FIN_WAIT    0x18     // Closing state
#define SOCK_CLOSING     0x1A     // Closing state
#define SOCK_TIME_WAIT   0x1B     // Closing state
#define SOCK_CLOSE_WAIT  0x1C     // Closing state
#define SOCK_LAST_ACK    0x1D     // Closing state
#define SOCK_UDP         0x22     // UDP socket
#define SOCK_IPRAW       0x32     // IP raw mode socket
#define SOCK_MACRAW      0x42     // MAC raw mode socket
#define SOCK_PPPOE       0x5F     // PPPOE socket
#define TX_BUF_MASK      0x07FF   // Tx 2K Buffer Mask:
#define RX_BUF_MASK      0x07FF   // Rx 2K Buffer Mask:
#define NET_MEMALLOC     0x05     // Use 2K of Tx/Rx Buffer

#define RTR              0x0017   //tempo de retry
#define RCR              0x0019   //tentativas
#define TIMEOUT          0x04     //

void wiz5200Write(uint16_t addr,uint8_t data){
    clearPin(PORTB,SS);
    spiSend((addr&0xFF00)>>8);
    spiSend(addr & 0x00FF);
    spiSend(WIZNET_WRITE_OPCODE);
    spiSend(0x01);
    spiSend(data);
    setPin(PORTB,SS);
}

uint8_t wiz5200Read(uint16_t addr){
    clearPin(PORTB,SS);
    spiSend((addr&0xFF00)>>8);
    spiSend(addr&0x00FF);
    spiSend(WIZNET_READ_OPCODE);
    spiSend(0x01);
    uint8_t val=spiSend(0xFF);
    setPin(PORTB,SS);
    return val;
}

void wiz5200SetTimeOut(uint16_t time){
    uint16_t var=time*10;
    wiz5200Write(RTR,0xFF00&var);
    wiz5200Write(RTR,(0x00FF&var)>>8);
}

void wiz5200SetRetry(uint8_t repeats){
    wiz5200Write(RCR,repeats);
}

uint8_t wiz5200Init(uint8_t ip[4],uint8_t mac[6],uint8_t gateway[4],uint8_t submask[4]){
    // Ethernet Setup
    wiz5200Write(MR,0x80);// Setting the Wiznet W5200 Mode Register: 0x0000 // MR = 0b10000000;

    for(uint8_t x=0;x<6;x++){
        wiz5200Write(SHAR+x,mac[x]); // Setting the Wiznet W5200 Source Address Register (SAR): 0x0009 to 0x000E
    }

    for(uint8_t x=0;x<4;x++){
        wiz5200Write(SIPR+x,ip[x]); // Setting the Wiznet W5200 IP Address (SIPR): 0x000F to 0x0012
    }

    for(uint8_t x=0;x<4;x++){
        wiz5200Write(GAR+x,gateway[x]); // Setting the Wiznet W5200 Gateway Address (GAR): 0x0001 to 0x0004
    }
    for(uint8_t x=0;x<4;x++){
        wiz5200Write(SUBR+x,submask[x]); // Setting the Wiznet W5100 Sub Mask Address (SUBR): 0x0005 to 0x0008
    }
    wiz5200Write(RMSR,NET_MEMALLOC);// Setting the Wiznet W5200 RX and TX Memory Size (2KB)
    wiz5200Write(TMSR,NET_MEMALLOC);
    return 1; //se tudo ok retorna 1
}

void close(){
    wiz5200Write(S0_CR,CR_CLOSE); // Send Close Command
    while(wiz5200Read(S0_CR));    // Waiting until the S0_CR is clear
}

void disconnect(){
    wiz5200Write(S0_CR,CR_DISCON);// Send Disconnect Command
    while(wiz5200Read(S0_CR));    // Wait for Disconecting Process
}

uint8_t socket(uint8_t eth_protocol,uint16_t tcp_port){

    if (wiz5200Read(S0_SR) == SOCK_CLOSED) {           // Make sure we close the socket first
        close();
    }
    wiz5200Write(S0_MR,eth_protocol);                  // Assigned Socket 0 Mode Register
    wiz5200Write(S0_PORT,((tcp_port & 0xFF00) >> 8 )); // Now open the Socket 0
    wiz5200Write(S0_PORT + 1,(tcp_port & 0x00FF));
    wiz5200Write(S0_CR,CR_OPEN);                       // Open Socket
    while(wiz5200Read(S0_CR));                         // Wait for Opening Process
    if (wiz5200Read(S0_SR) == SOCK_INIT)               // Check for Init Status
    return 1;
    else
    close();
    return 0;
}

uint8_t listen(){
    uint8_t retval = 0;
    if (wiz5200Read(S0_SR) == SOCK_INIT) {
        wiz5200Write(S0_CR,CR_LISTEN);         // Send the LISTEN Command
        while(wiz5200Read(S0_CR));             // Wait for Listening Process
        if (wiz5200Read(S0_SR) == SOCK_LISTEN) // Check for Listen Status
            retval=1;
        else
            close();
    }
    return retval;
}

uint16_t send(char *buf,uint16_t buflen){
    uint16_t ptr,offaddr,realaddr,txsize,timeout;
    if (buflen <= 0) return 0;      // Make sure the TX Free Size Register is available
    txsize=wiz5200Read(SO_TX_FSR);
    txsize=(((txsize & 0x00FF) << 8 )+wiz5200Read(SO_TX_FSR + 1));
    timeout=0;
    while (txsize < buflen){
        _delay_ms(1);
        txsize=wiz5200Read(SO_TX_FSR);
        txsize=(((txsize & 0x00FF) << 8 )+wiz5200Read(SO_TX_FSR + 1));// Timeout for approx 1000 ms
        if (timeout++>1000){
            disconnect();                                         // Disconnect the connection
            return 0;
        }
    }
    ptr = wiz5200Read(S0_TX_WR);                                      // Read the Tx Write Pointer
    offaddr=(((ptr & 0x00FF) << 8 )+wiz5200Read(S0_TX_WR + 1));
    while(buflen) {
        buflen--;
        realaddr = TXBUFADDR + (offaddr & TX_BUF_MASK);               // Calculate the real W5100 physical Tx Buffer Address
        wiz5200Write(realaddr,*buf);                                  // Copy the application data to the W5100 Tx Buffer
        offaddr++;
        buf++;
    }
    wiz5200Write(S0_TX_WR,(offaddr & 0xFF00) >> 8 );                  // Increase the S0_TX_WR value, so it point to the next transmit
    wiz5200Write(S0_TX_WR + 1,(offaddr & 0x00FF));
    wiz5200Write(S0_CR,CR_SEND);                                      // Now Send the SEND command
    while(wiz5200Read(S0_CR));                                        // Wait for Sending Process
    return 1;
}

uint16_t receive(char *buf,uint16_t buflen){
    uint16_t ptr,offaddr,realaddr;
    if (buflen <= 0) return 1;
    if (buflen > MAX_BUF)                                             // If the request size > MAX_BUF,just truncate it
    buflen=MAX_BUF - 2;
    ptr = wiz5200Read(S0_RX_RD);                                      // Read the Rx Read Pointer
    offaddr = (((ptr & 0x00FF) << 8 ) + wiz5200Read(S0_RX_RD + 1));
    while(buflen) {
        buflen--;
        realaddr=RXBUFADDR + (offaddr & RX_BUF_MASK);
        *buf = wiz5200Read(realaddr);
        offaddr++;
        buf++;
    }
    *buf='\0';        // String terminated character

    wiz5200Write(S0_RX_RD,(offaddr & 0xFF00) >> 8 );                  // Increase the S0_RX_RD value, so it point to the next receive
    wiz5200Write(S0_RX_RD + 1,(offaddr & 0x00FF));
    wiz5200Write(S0_CR,CR_RECV);                                     // Now Send the RECV command
    _delay_us(5);                                                    // Wait for Receive Process
    return 1;
}

uint16_t receiveSize(void){
    return ((wiz5200Read(S0_RX_RSR) & 0x00FF) << 8 ) + wiz5200Read(S0_RX_RSR + 1);
}

uint8_t connect(uint8_t destinationIP[4],uint16_t destinationPort, uint16_t localPort){
    wiz5200Write(S0_MR,MR_TCP);                  //iniciamos o socket como tcp
    wiz5200Write(S0_PORT,(localPort&0xFF00)>>8); //Configuramos a porta local, byte mais signigicativo primeiro
    wiz5200Write(S0_PORT+1,(localPort&0x00FF));

    wiz5200Write(S0_DIPR  ,destinationIP[0]);      //ip do destino
    wiz5200Write(S0_DIPR+1,destinationIP[1]);
    wiz5200Write(S0_DIPR+2,destinationIP[2]);
    wiz5200Write(S0_DIPR+3,destinationIP[3]);

    wiz5200Write(S0_DPORT,(destinationPort&0xFF00)>>8); //Porta de destino, byte mais significagtivo primeiro
    wiz5200Write(S0_DPORT+1,(destinationPort&0x00FF));

    wiz5200Write(S0_CR,CR_OPEN);                 //Abrimos o socket
    while(wiz5200Read(S0_SR)!=SOCK_INIT);

    wiz5200Write(S0_CR,CR_CONNECT);              //Tentamos conectar ao servidor


    uint8_t status=wiz5200Read(S0_SR);

    for(uint8_t x=0;status!=SOCK_ESTABLISHED;x++){
        wiz5200Write(S0_CR,CR_OPEN);
        while(wiz5200Read(S0_SR)!=SOCK_INIT);
        wiz5200Write(S0_CR,CR_CONNECT);
        _delay_ms(100);
        status=wiz5200Read(S0_SR);
        serialSendInt(x,DEC,1);
        if(x>99){
            disconnect();
            //close();
            return 0;
        }
    }


    return SOCK_ESTABLISHED;
}
