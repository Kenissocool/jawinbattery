/*
 ******************************************************************************
 * @file    bms.c
 * @brief   This file provides code for setting and reading data
 *          from Battery Management System (BMS)
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bms.h"

/* Private define ------------------------------------------------------------*/
// No private defines

/* Private variables ---------------------------------------------------------*/
uint32_t last_update_ms = 0;
static uint16_t batterycell[18];
static uint16_t no_of_cell;
static uint16_t temp;
static uint16_t batterypercentage;
static uint16_t sumvoltage;
static int32_t current;

/* Private function prototypes -----------------------------------------------*/
// None

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Parses FDCAN messages and updates battery data.
 * @param  hfdcan Pointer to the FDCAN handle.
 * @retval None
 */
void BMS_Update(FDCAN_HandleTypeDef *hfdcan)
{
    FDCAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];

    // Check if a new CAN message is available in RX FIFO0
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
        last_update_ms = HAL_GetTick();

        // Decode total pack voltage and current
        if (RxHeader.Identifier == 0x002E0951)
        {
            sumvoltage = RxData[3] << 8 | RxData[2];
            int16_t raw_current = (int16_t)(RxData[5] << 8 | RxData[6]);
            current = (int32_t)raw_current;
        }

        // Decode temperature and battery percentage
        if (RxHeader.Identifier == 0x002E0942)
        {
            temp = RxData[1] << 8 | RxData[0];
            batterypercentage = RxData[5] << 8 | RxData[4];
        }

        // Decode number of cells and cell voltage 1–2
        if (RxHeader.Identifier == 0x002E0943)
        {
            no_of_cell = RxData[3] << 8 | RxData[2];
            batterycell[0] = RxData[5] << 8 | RxData[4];
            batterycell[1] = RxData[7] << 8 | RxData[6];
        }

        // Decode cell voltages 3–6
        if (RxHeader.Identifier == 0x002E0944)
        {
            for (int i = 0; i <= 3; i++)
            {
                batterycell[i + 2] = RxData[(2 * i) + 1] << 8 | RxData[2 * i];
            }
        }

        // Decode cell voltages 7–10
        if (RxHeader.Identifier == 0x002E0945)
        {
            for (int i = 0; i <= 3; i++)
            {
                batterycell[i + 6] = RxData[(2 * i) + 1] << 8 | RxData[2 * i];
            }
        }

        // Decode cell voltages 11–14
        if (RxHeader.Identifier == 0x002E0946)
        {
            for (int i = 0; i <= 3; i++)
            {
                batterycell[i + 10] = RxData[(2 * i) + 1] << 8 | RxData[2 * i];
            }
        }

        // Decode cell voltages 15–18
        if (RxHeader.Identifier == 0x002E094F)
        {
            for (int i = 0; i <= 3; i++)
            {
                batterycell[i + 14] = RxData[(2 * i) + 1] << 8 | RxData[2 * i];
            }
        }
    }
}

/**
 * @brief  Get voltage of a specific battery cell.
 * @param  index Cell index (0-17).
 * @retval Cell voltage (uint16_t)
 */
uint16_t BMS_GetBatteryCell(uint8_t index) {
    if (index < 18) return batterycell[index];
    return 0;
}

/**
 * @brief  Get number of cells in the battery pack.
 * @retval Number of cells (uint16_t)
 */
uint16_t BMS_GetNoOfCell(void) { return no_of_cell; }

/**
 * @brief  Get battery temperature.
 * @retval Temperature (uint16_t)
 */
uint16_t BMS_GetTemp(void) { return temp; }

/**
 * @brief  Get battery state of charge (percentage).
 * @retval SOC percentage (uint16_t)
 */
uint16_t BMS_GetBatteryPercentage(void) { return batterypercentage; }

/**
 * @brief  Get total pack voltage.
 * @retval Total voltage (uint16_t)
 */
uint16_t BMS_GetSumVoltage(void) { return sumvoltage; }

/**
 * @brief  Get actual battery current.
 * @retval Current (int32_t)
 */
int32_t BMS_GetCurrent(void) { return current; }
