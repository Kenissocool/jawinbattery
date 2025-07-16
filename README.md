# 🔋 STM32 BMS Library for Jawin 30000mAh 18S Battery

<span style="font-size:smaller;">
This library allows an STM32 microcontroller to interface with a <strong>Jawin 30000mAh 18S smart battery</strong> over <strong>CAN bus (1 Mbps)</strong>. It reads and decodes battery metrics such as voltage, current, temperature, SOC (state of charge), and per-cell voltages. This example is tested with stm32 g474 can node so i use FDCAN as CAN2.0B for receiving data.
</span>

## Table of Contents
- [File Structure](#file-structure)
- [Features](#features)
- [Hardware Setup](#hardware-setup)
- [Battery Information](#battery-information)
- [CAN Data Frame Format & Decoding](#can-data-frame-format--decoding)
- [STM32 Integration Steps](#stm32-integration-steps)
- [API Reference](#api-reference)


---
## File Structure

| File      | Description                          |
|-----------|--------------------------------------|
| `bms.h`   | Header file with API and data struct |
| `bms.c`   | Source file with CAN parsing logic   |

---
## How to Use This Library

1. **Enable CAN** in STM32CubeMX (1 Mbps)
2. **Copy `bms.h` and `bms.c`** into your project
3. **Initialize and start** CAN in `main.c`
4. Call `BMS_Update()` regularly inside your main loop
5. Use `BMS_GetData()` to access live battery data

To integrate the BMS library into your STM32 project, simply call the following two functions in your main loop 

```c
BMS_Update(&hfdcan1);                    // Update internal battery data from CAN frame
BatteryMsg* data = BMS_GetData();        // Access the latest battery data
```

## Features

- ✅ Compatible with **Jawin 18S CAN battery**
- ⚡ Reads:
  - Total voltage
  - Battery current (signed)
  - Battery temperature
  - State of charge (percentage)
  - Per-cell voltages (up to 18 cells)
- 🧠 Automatically decodes known CAN message IDs
- ⏱️ Tracks last update timestamp via `last_update_ms`

---

## Hardware setup

- **MCU**: STM32 with CAN/FDCAN peripheral (e.g., STM32G4)
- **Wiring**:
  - CAN High and Low connected to STM32 CAN transceiver
  - Optional 120Ω termination resistor at each CAN end
  - Shared ground between battery and STM32

---

## Battery information

- **Name**: Jawin 30000mAh 18S smart battery 
- **Protocol**: CAN2.0B
- **CAN Speed**: **1 Mbps**
- **Wiring**:
  - 4AWG Red Wire: VBAT
  - 4AWG Black Wire: GND
  - 26AWG Red Wire: 5V (BUS Logic)
  - 26AWG Black Wire: GND
  - 26AWG Blue Wire: CANH
  - 26AWG White Wire: CANL

---

##  CAN Data Frame Format & Decoding

Each CAN message from the battery includes:

- **29-bit CAN ID** to identify message type
- **8-byte data frame** with specific structure depending on ID
- **CAN Speed**:1Mbps
- **Message Type**:Little Endian

|   CAN ID    |       1-2nd byte       |            3-4th byte           |       5-6th byte       |     7-8th byte     |
|-------------|------------------------|---------------------------------|------------------------|--------------------|
| 0x002E0951  | Unknown                | Voltage of battery/100          | Current/1000           | Unknown            |
| 0x002E0942  | Battery Temperature/10 | Always 0x0401                   | Battery Percentage/100 | Always 0           |
| 0x002E0943  | Always 0               | Amount of cell in battery (18s) | 1st Cell Volatage      | 2nd Cell Volatage  |
| 0x002E0944  | 3rd Cell Voltage       | 4th Cell Voltage                | 5th Cell Volatage      | 6th Cell Volatage  |
| 0x002E0945  | 7th Cell Voltage       | 8th Cell Voltage                | 9th Cell Volatge       | 10th Cell Volatage |
| 0x002E0945  | 11th Cell Voltage      | 12th Cell Voltage               | 13th Cell Volatge      | 14th Cell Volatage |
| 0x002E0945  | 15th Cell Voltage      | 16th Cell Voltage               | 17th Cell Volatge      | 18th Cell Volatage |


###  **Remarks**

- 🔄 **Current** is a **signed** value:  
  - Positive → battery is **charging**  
  - Negative → battery is **discharging**

- 🔋 **Cell Voltage** values are raw `uint16_t`:
  - Divide by **1000** to get **volts (V)**
  ---


## API Reference

### `void BMS_Update(FDCAN_HandleTypeDef *hfdcan);`

Polls and processes a CAN message from the battery (non-blocking).

- **Parameters**:
  - `hfdcan`: Pointer to your active FDCAN handle (e.g., `&hfdcan1`)
- **Effect**:
  - Processes one message from FIFO0 (if available)
  - Updates internal battery data
  - Updates `last_update_ms` with current time

---

### `BatteryMsg* BMS_GetData(void);`

Returns a pointer to the most recent battery data.

- **Returns**: `BatteryMsg*` — pointer to internal struct with latest values

---

### `extern uint32_t last_update_ms;`

Global variable tracking the time (in milliseconds) when the last CAN message was processed.

---








