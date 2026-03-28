# STM32 Peripheral Driver Framework

A modular STM32 microcontroller driver framework, developed using register-level programming **(no HAL).**
This project demonstrates peripheral driver development, hardware abstraction, and a menu-driven testing approach using UART.

# Project Overview
This project is designed to build a reusable and scalable driver library for STM32 MCUs.
It follows a layered architecture similar to industry firmware design.

1. Target Board: STM32 NUCLEO-F446RE
2. MCU: STM32F446RE
3. Language: Embedded C
4. Development Style: Bare-metal (No HAL / No CubeMX)

# Hardware Used
  1. STM32 NUCLEO-F446RE Board
  2. Onboard LED (PA5)
  3. User Button (PC13)
  4. USB UART (Virtual COM Port)
  5. USB Logic Analyzer

# Tools & Setup
  1. STM32CubeIDE / arm-gcc
  2. Serial Terminal (PuTTY / TeraTerm / minicom) => TeraTerm is used Here

# Drivers Implemented

# 1. GPIO Driver 
 1. Pin configuration (Input / Output)
 2. Read / Write operations
 3. Toggle functionality

# 2. LED Driver
 1. LED ON / OFF / TOGGLE control
 2. Built on top of GPIO driver

# 3. Button Driver
 1. Button state detection (Pressed / Released)
 2. Polling-based input
 3. Button Interrupt

# 4. UART Driver
 1. UART initialization
 2. Transmit / Receive functions

# 5. I2C Driver
 1. I2C Send Data
 2. I2C Receive Data
