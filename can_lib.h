// can_lib.h
#ifndef CAN_LIB_H
#define CAN_LIB_H

#include <Arduino.h>
#include "driver/twai.h"

#define CAN_TX GPIO_NUM_5
#define CAN_RX GPIO_NUM_6
#define CAN_ID 0x5BF

void canInit();
void translateToCan(uint8_t *data, uint8_t len);

#endif