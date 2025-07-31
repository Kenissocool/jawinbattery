/*
 ******************************************************************************
 * @file    bms.h
 * @brief   This file provides code for setting and reading data
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
/**
 * @brief  Parses FDCAN messages and updates battery data.
 * @param  hfdcan Pointer to the FDCAN handle.
 * @retval None
 */
void BMS_Update(FDCAN_HandleTypeDef *hfdcan);

/**
 * @brief  Get voltage of a specific battery cell.
 * @param  index Cell index (0-17).
 * @retval Cell voltage (uint16_t)
 */
uint16_t BMS_GetBatteryCell(uint8_t index);

/**
 * @brief  Get number of cells in the battery pack.
 * @retval Number of cells (uint16_t)
 */
uint16_t BMS_GetNoOfCell(void);

/**
 * @brief  Get battery temperature.
 * @retval Temperature (uint16_t)
 */
uint16_t BMS_GetTemp(void);

/**
 * @brief  Get battery state of charge (percentage).
 * @retval SOC percentage (uint16_t)
 */
uint16_t BMS_GetBatteryPercentage(void);

/**
 * @brief  Get total pack voltage.
 * @retval Total voltage (uint16_t)
 */
uint16_t BMS_GetSumVoltage(void);

/**
 * @brief  Get actual battery current.
 * @retval Current (int32_t)
 */
int32_t BMS_GetCurrent(void);

#endif /* __BMS_H */
