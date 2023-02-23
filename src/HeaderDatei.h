/**
 * \file			HeaderDatei.h
 * \brief			application-logic-header
 *
 * This file provides the application-logic for the program.
 *
 * \author			Seifeddine Makhlouf
 * \date			2023
 * \version			1.0
 */


/**
* \brief			Include necessary AVR header files
 *
*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/**
* \brief		 Define the clock frequency for the microcontroller
*
*/
#define F_CPU 16000000UL

/**
*\enum
* \brief		 
* Define an enumeration for the parity setting of the USART interface
*/
enum Parity { none,Reserved,even,odd};

/**
 *\fn initUsart(uint32_t baudrate, uint8_t databits, Parity parity, uint8_t stopbits)
* \brief		 
* Initialize the USART interface with the specified parameters
**/
void initUsart(uint32_t baudrate, uint8_t databits, Parity parity, uint8_t stopbits){
    // Convert the number of data bits to the appropriate value for the register
    switch(databits) {
        case 5: databits=0; break;
        case 6: databits=1; break;
        case 7: databits=2; break;
        default: databits=3; break;
    }
    
    // Calculate the UBRR value for the specified baud rate
    unsigned int ubrr0 = (F_CPU) / ((baudrate) * 16) - 1;
    // Set the UBRRH and UBRRL registers with the calculated value
    UBRR0H = (unsigned char)(ubrr0>>8);
    UBRR0L = (unsigned char)ubrr0;
    
    // Enable the receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    // Set the frame format (data bits, stop bits, parity)
    UCSR0C = ((stopbits-1)<<USBS0)|(databits<<UCSZ00)|(parity<< UPM00);
}

/**
 * \fn usartPutchar(unsigned char c)
* \brief		 
*  Send a single character over the USART interface
**/
void usartPutchar(unsigned char c)
{
    // Wait for the transmit buffer to be empty
    while (!(UCSR0A & (1<<UDRE0)))
    ;
    // Send the character by setting the UDR0 register
    UDR0 = c;
}


/**
 * \fn usart_putstring (char *s)
* \brief		 
*  Send a null-terminated string over the USART interface
**/
void usart_putstring (char *s)
{
    while (*s)
    {   // Keep sending characters until the null terminator is reached
        usartPutchar(*s);
        s++;
    }
}



/**
 * \fn analogRead(int pin)
* \brief		 
*  Read the analog value from the specified pin using the ADC
*\param	input		pin 
* \return				tge anlog Value
**/
uint16_t analogRead(int pin) {
    // Configure the ADC with the desired input channel and reference voltage
    ADMUX = (1 << REFS0) | (pin & 0x07);
    // Enable the ADC and set the prescaler to 128
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Start the conversion by setting the ADSC bit
    ADCSRA |= (1 << ADSC);
    
    // Wait for the conversion to complete by checking the ADSC bit
    while (ADCSRA & (1 << ADSC));
    
    // Return the result from the ADC register
    return ADC;
}