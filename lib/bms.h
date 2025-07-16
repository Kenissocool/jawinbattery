/*
 * bms.h
 *
 *  Created on: Jun 18, 2025
 *      Author: USER
 */

#ifndef __BMS_H
#define __BMS_H

#include "main.h"  // Includes FDCAN_HandleTypeDef and HAL
#include <stdint.h>


typedef struct {
    uint16_t batterycell[18];       // Voltage of each battery cell (up to 18 cells)
    uint16_t no_of_cell;            // Number of cells in the battery pack
    uint16_t temp;                  // Battery temperature
    uint16_t batterypercentage;     // State of charge (SOC) percentage

    uint16_t sumvoltage;            // Total pack voltage

    int32_t current;                 // Calculated actual current (signed)
} BatteryMsg; 						//struct for holding data


extern uint32_t last_update_ms; 	//variable for holding data

//buuff


/**
 * @brief		Parses FDCAN messages and updates the battery data struct.
 * 					This function should be called regularly inside the main loop or CAN RX callback.
 * 					It receives CAN frames and decodes battery information including voltage,
 * 					current, temperature, and cell data based on CAN message identifiers.
 * @param 		hfdcan Pointer to the FDCAN handle used for receiving CAN messages.
 * @retval 		None
 */
void BMS_Update(FDCAN_HandleTypeDef *hfdcan);

/**
 * @brief 		Returns a pointer to the latest decoded battery data.
 * 				Use this to access current battery readings after BMS_Update() has been called.
 * @param		None
 * @retval 		Pointer to a BatteryMsg structure containing the latest battery data.
 */
BatteryMsg* BMS_GetData(void);

#endif /* __BMS_H */
