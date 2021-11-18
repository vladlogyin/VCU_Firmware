#ifndef VCU_CAN_FIREWALL_TASK_H
#define VCU_CAN_FIREWALL_TASK_H

#include <vcu/vcu_tasks.h>

struct can_tx_packet {
  uint32_t std_id;
  uint32_t ext_id;
  uint8_t ide;
  uint8_t rtr;
  uint8_t dlc;
  uint8_t data[8];
};

struct can_rx_packet
{
  uint32_t ID;
  /**
   * true if extended ID is used
   */
  bool extendedID;
  /**
   * true if the packet is a request of requestTransmission
   */
  bool requestTransmission;
  /**
   * Packet length
   */
  uint8_t length;
  /**
   * Packet data
   */
  uint8_t data[8];
  /**
   * Index of the matched ID filterIndex
   */
  uint8_t filterIndex;
};

/**
 * CAN firewall subsystem
 *
 * TODO this is currently untested
 * TODO add IRQs to cherrypick the sensor packets needed
 * TODO add some sort of a function to queue and send packets
 */
class can_firewall_task : public rtos_task
{
public:
  can_firewall_task();

  task_status start() override;

  task_status loop() override;

  task_status stop() override;

};


#endif
