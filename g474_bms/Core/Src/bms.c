/*
 * BMS_read.c
 *
 */

#include "main.h"
#include "bms.h"

// Timestamp of the last received CAN message (in milliseconds)
uint32_t last_update_ms = 0;

// Internal battery message structure to store decoded values
static BatteryMsg batterymsg;

// Return a pointer to the current battery data structure
BatteryMsg* BMS_GetData(void)
{
    return &batterymsg;
}

// Read and decode CAN messages from the battery via FDCAN
void BMS_Update(FDCAN_HandleTypeDef *hfdcan)
{
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[8];

	// Check if a new CAN message is available in RX FIFO0
	if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
	{
		// Store current timestamp for timeout or freshness check
		last_update_ms = HAL_GetTick();

		// Decode total pack voltage and current
		if (RxHeader.Identifier == 0x002E0951)
		{
		    // Total voltage (16-bit, little endian)
		    batterymsg.sumvoltage = RxData[3] << 8 | RxData[2];

		    // Convert raw 2 bytes directly to signed 16-bit int
		    int16_t raw_current = (int16_t)(RxData[5] << 8 | RxData[6]);

		    // Store as double for precision or scaling later
		    batterymsg.current = (double)raw_current;
		}

		// Decode temperature and battery percentage
		if (RxHeader.Identifier == 0x002E0942)
		{
			// Temperature (16-bit, little endian)
			batterymsg.temp = RxData[1] << 8 | RxData[0];

			// Battery state of charge (percentage)
			batterymsg.batterypercentage = RxData[5] << 8 | RxData[4];
		}

		// Decode number of cells and cell voltage 1–2
		if (RxHeader.Identifier == 0x002E0943)
		{
			batterymsg.no_of_cell = RxData[3] << 8 | RxData[2];
			batterymsg.batterycell[0] = RxData[5] << 8 | RxData[4];
			batterymsg.batterycell[1] = RxData[7] << 8 | RxData[6];
		}

		// Decode cell voltages 3–6
		if (RxHeader.Identifier == 0x002E0944)
		{
			for (int i = 0; i <= 3; i++)
			{
				batterymsg.batterycell[i + 2] = RxData[(2 * i) + 1] << 8 | RxData[2 * i];
			}
		}

		// Decode cell voltages 7–10
		if (RxHeader.Identifier == 0x002E0945)
		{
			for (int i = 0; i <= 3; i++)
			{
				batterymsg.batterycell[i + 6] = RxData[(2 * i) + 1] << 8 | RxData[2 * i];
			}
		}

		// Decode cell voltages 11–14
		if (RxHeader.Identifier == 0x002E0946)
		{
			for (int i = 0; i <= 3; i++)
			{
				batterymsg.batterycell[i + 10] = RxData[(2 * i) + 1] << 8 | RxData[2 * i];
			}
		}

		// Decode cell voltages 15–18
		if (RxHeader.Identifier == 0x002E094F)
		{
			for (int i = 0; i <= 3; i++)
			{
				batterymsg.batterycell[i + 14] = RxData[(2 * i) + 1] << 8 | RxData[2 * i];
			}
		}
	}
}
