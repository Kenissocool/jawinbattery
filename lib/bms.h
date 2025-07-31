/*
 ******************************************************************************
 * @file    bms.h
 *          This file provides code for setting and reading data
 *          from Battery Management System (BMS)
 ******************************************************************************
 */

#ifndef __BMS_H
#define __BMS_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern uint32_t last_update_ms;

/* Exported functions ------------------------------------------------------- */

void BMS_Update(FDCAN_HandleTypeDef *hfdcan);
uint16_t BMS_GetBatteryCell(uint8_t index);
uint16_t BMS_GetNoOfCell(void);
uint16_t BMS_GetTemp(void);
uint16_t BMS_GetBatteryPercentage(void);
uint16_t BMS_GetSumVoltage(void);
int32_t BMS_GetCurrent(void);
int32_t BMS_GetCurrent(void);

#endif /* __BMS_H */
