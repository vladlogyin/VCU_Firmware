#include <main.h>

/**
 * VCU main function
 */
int main()
{
  clocksetup();
  // Enable USART1
  usartsetup();

  vcu_init_task initTask;
  // Start the RTOS
  rtos::bringup(&initTask);

  return 0;
}

void clocksetup()
{
  // Set up the PLL to generate 180MHz from the external oscillator
  rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);

}

void usartsetup(void)
{
  // Enable USART1 clock
  rcc_periph_clock_enable(RCC_USART1);

  // Setup GPIO pin GPIO_USART1_TX on GPIO port A for transmit.
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO9 | GPIO10);
  // Setup UART parameters.
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  usart_set_mode(USART1, USART_MODE_TX);

  // Finally enable the USART
  usart_enable(USART1);
}
