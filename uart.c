#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        char ch = uart_getc(UART_ID);
        putchar(ch); // In ra terminal USB
    }
}

int main() {
    stdio_init_all();

    
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_ID, true);

    
    irq_set_exclusive_handler(UART0_IRQ, on_uart_rx);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);

    printf("Pico chat: \n");

    while (true) {
        int ch = getchar_timeout_us(0);
        if (ch != PICO_ERROR_TIMEOUT) {
            uart_putc(UART_ID, (char)ch);
        }

        sleep_ms(10); // 🔁 
    }
}
