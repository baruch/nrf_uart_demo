#include <stdint.h>
#include <stdio.h>

#include "gpio.h"
#include "delay.h"
#include "uart.h"

void flash_led(int t)
{
	gpio_pin_val_set(GPIO_PIN_ID_P0_0);
	delay_ms(t); 
	gpio_pin_val_clear(GPIO_PIN_ID_P0_0);
	delay_ms(t); 
}

void main()
{
	int i;

	// Setup pin P0.0 for led
	gpio_pin_configure(GPIO_PIN_ID_P0_0,
			GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT |
			GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR |
			GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH);

	// Setup UART pins
	gpio_pin_configure(GPIO_PIN_ID_FUNC_RXD,
			GPIO_PIN_CONFIG_OPTION_DIR_INPUT |
			GPIO_PIN_CONFIG_OPTION_PIN_MODE_INPUT_BUFFER_ON_NO_RESISTORS);

	gpio_pin_configure(GPIO_PIN_ID_FUNC_TXD,
			GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT |
			GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET |
			GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH);

	uart_configure_8_n_1_19200();

	puts("Starting up\r\n");

	// Startup LED flashing to indicate program startup
	for (i = 0; i < 10; i++) {
		flash_led(1000);
	}

	while(1)
	{
		// Read from UART
		if (uart_rx_data_ready()) {
			uint8_t ch = uart_wait_for_rx_and_get();
			int t;
			// Easy method to indicate proper receive, if character received is '1', flash led longer
			if (ch == '1') {
				t = 1000;
			} else
				t = 100;

			flash_led(t);
			putchar(ch);
		}
		delay_ms(1000);

		// To succeed to send disabling RX is needed
		uart_rx_disable();
		puts("Hello World!\r\n"); // puts uses putchar defined in the sdk
		uart_rx_enable();
	}
}
