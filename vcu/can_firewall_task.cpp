#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <vcu/can_firewall_task.h>
can_firewall_task::can_firewall_task() : rtos_task()
{
  const char task_name[] = "can firewall";
  std::strcpy(name, task_name);
}

rtos_task::task_status can_firewall_task::start()
{
  // TODO the GPIO peripheral clocks might already be enabled, maybe they can be centralized, though calling enable repeatedly shouldn't affect things
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  // Set CAN1 RX TX alternate function for pins PB8 and PB9
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8 | GPIO9);
  gpio_set_af(GPIOB, GPIO_AF9, GPIO8 | GPIO9);
  // Set CAN2 RX TX alternate function for pins PB5 and PB6
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO6);
  gpio_set_af(GPIOB, GPIO_AF9, GPIO5 | GPIO6);
  // Enable CAN1 and CAN2
  rcc_periph_clock_enable(RCC_CAN1);
  rcc_periph_clock_enable(RCC_CAN2);

  // Reset the CAN PHYs
  can_reset(CAN1);
  can_reset(CAN2);

  if(can_init(CAN1, false, true, false, false, false, false, CAN_BTR_SJW_1TQ, CAN_BTR_TS1_9TQ, CAN_BTR_TS2_6TQ, 2, false, false) || can_init(CAN2, false, true, false, false, false, false, CAN_BTR_SJW_1TQ, CAN_BTR_TS1_9TQ, CAN_BTR_TS2_6TQ, 2, false, false))
  {
    // TODO handle can init error by cleaning up using stop()
    this->stop();
    // Return an error code since CAN couldn't be initialized
    return rtos_task::RTOS_TASK_FAILED;
  }
  // Set filter 0 to accept all IDs
  can_filter_id_mask_32bit_init(0,0,0,0,true);
  // Enable CAN RX interrupts
  can_enable_irq(CAN1, CAN_IER_FMPIE0);
  can_enable_irq(CAN2, CAN_IER_FMPIE0);

  return rtos_task::RTOS_TASK_LOOP;
}

rtos_task::task_status can_firewall_task::loop()
{
  return rtos_task::RTOS_TASK_LOOP;
}

rtos_task::task_status can_firewall_task::stop()
{
  // TODO add GPIO de-init and cleanup code
  return rtos_task::RTOS_TASK_DONE;
}

// Firewall receive interrupt
void can1_rx0_isr()
{
  can_rx_packet rx;
  can_receive(CAN1, 0,false, &rx.ID, &rx.extendedID, &rx.requestTransmission, &rx.filterIndex, &rx.length, rx.data, nullptr);
  can_transmit(CAN2,rx.ID, rx.extendedID, rx.requestTransmission, rx.length, rx.data);
}

// Firewall receive interrupt
void can2_rx0_isr()
{
  can_rx_packet rx;
  can_receive(CAN2, 0,false, &rx.ID, &rx.extendedID, &rx.requestTransmission, &rx.filterIndex, &rx.length, rx.data, nullptr);
  can_transmit(CAN1,rx.ID, rx.extendedID, rx.requestTransmission, rx.length, rx.data);
}


