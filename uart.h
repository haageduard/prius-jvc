#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

// Software UART delay for 9600Kbps (104us for 9600kbps)
#define SW_UART_DELAY 104

// Software UART transmiter port definition
#define TX_PORTX PORTB
#define TX_DDRX DDRB
#define TX_PIN PB1

void init_uart();
void uart_tx_pin_high();
void uart_tx_pin_low();
void send_uart(const unsigned char c);
void send_uart_str(const char *s);
void send_uart_pstr(const char *pstr);

#endif
