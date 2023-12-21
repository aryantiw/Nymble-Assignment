#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#define F_CPU 16000000UL
#define BAUD_RATE 2400
#define DATA_SIZE 1000

void initUART() {
    // Set baud rate
    UBRR0 = F_CPU / 16 / BAUD_RATE - 1;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void transmitChar(char data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    // Put data into buffer, sends the data
    UDR0 = data;
}

char receiveChar() {
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)))
        ;

    // Get and return received data from buffer
    return UDR0;
}

void storeDataToEEPROM(const char *data) {
    for (int i = 0; i < DATA_SIZE; ++i) {
        eeprom_write_byte((uint8_t *)i, data[i]);
        _delay_ms(5); // Delay to allow EEPROM write operation to complete
    }
}

void transmitDataFromEEPROM() {
    for (int i = 0; i < DATA_SIZE; ++i) {
        char data = eeprom_read_byte((uint8_t *)i);
        transmitChar(data);
        _delay_ms(5); // Delay for data transmission
    }
}

int main() {
    initUART();

    char data[DATA_SIZE] = "Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for predicting that the next banking crisis would be triggered by MSME lending, saying postmortem is easier than taking action when it was required. Rajan, who had as the chief economist at IMF warned of impending financial crisis of 2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, saying that they could be the sources of next banking crisis. Government should focus on sources of the next crisis, not just the last one."
                           "In particular, government should refrain from setting ambitious credit targets or waiving loans. Credit targets are sometimes achieved by abandoning appropriate due diligence, creating the environment for future NPAs,\" Rajan said in the note.\" Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined more closely for potential credit risk. Rajan, who was RBI governor for three years till September 2016, is currently.";

    // Transmit data from PC to MCU
    storeDataToEEPROM(data);

    // Transmit data from MCU to PC
    transmitDataFromEEPROM();

    return 0;
}
