#ifndef PORTMANIPULATION_INCLUDED
#define PORTMANIPULATION_INCLUDED

/*
Basic macro definitions to manipulate ports, pins and ddr.
Autor: Filipe Coelho
*/

#define bv(pin) (1<<pin) //Shift 1 to needed bit location ex: bv(5) = 0b00100000, bv(1) = 0b00000010

#define setPin(byte,pin) (byte|=bv(pin))    //Set selected bit
#define clearPin(byte,pin) (byte&=~(bv(pin))) //Clear selected bit
#define togglePin(byte,pin) (byte^=bv(pin)) //toggle selected bit

#define inputPin(byte,pin) (byte&=~(bv(pin))//Set the pin as input
#define outputPin(byte,pin) (byte|=bv(pin)) //Set the pin as output

#define readPin(byte,pin) (byte&bv(pin))    //Read the selected pin

#endif // PORTMANIPULATION_INCLUDED
