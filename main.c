#include <stdint.h>
#include <stdio.h>

#include "gpio.h"
#include "src/gpio/src/gpio_pin_configure.c"
#include "src/gpio/src/gpio_pin_val_clear.c"
#include "src/gpio/src/gpio_pin_val_set.c"

#include "delay.h"
#include "src/delay/src/delay_us.c"
#include "src/delay/src/delay_s.c"
#include "src/delay/src/delay_ms.c"

#include "uart.h"
#include "src/uart/src/uart_configure_manual_baud_calc.c"
#include "src/uart/src/uart_calc_actual_baud_rate_from_s0rel.c"
#include "src/uart/src/uart_calc_s0rel_value.c"
#include "src/uart/src/uart_calc_actual_baud_rate_from_th1.c"
#include "src/uart/src/uart_calc_th1_value.c"
#include "src/uart/src/uart_send_wait_for_complete.c"
#include "src/uart/src/uart_wait_for_rx_and_get.c"
#include "src/uart/src/putchar.c"

#include "timer1.h"
#include "src/timer1/src/timer1_configure.c"

#include "pwr_clk_mgmt.h"
#include "src/pwr_clk_mgmt/src/pwr_clk_mgmt_get_cclk_freq_in_hz.c"

#include "interrupt.h"

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
		}
		delay_ms(100);

		// To succeed to send disabling RX is needed
		uart_rx_disable();
		puts("Hello World!\r\n"); // puts uses putchar defined in the sdk
		uart_rx_enable();
	}
}
