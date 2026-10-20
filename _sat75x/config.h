// Copyright 2023 Andrew Kannan (@awkannan)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Ensure we jump to bootloader if the QK_BOOT keycode was pressed */
#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP TRUE

/* LSE clock */
#define STM32_LSECLK 32768

#define ENCODER_RESOLUTION 2

// I2C config
#define I2C_DRIVER I2CD1
#define I2C1_SCL_PIN B6
#define I2C1_SDA_PIN B7
#define I2C1_SCL_PAL_MODE 1
#define I2C1_SDA_PAL_MODE 1

#define DEFAULT_TIMINGS 1
//#define STANDARD_TIMINGS 1
//#define FAST_TIMINGS 1

#if defined(DEFAULT_TIMINGS)
// ???
#define I2C1_TIMINGR_PRESC 0x00U
#define I2C1_TIMINGR_SCLDEL 0x03U
#define I2C1_TIMINGR_SDADEL 0x01U
#define I2C1_TIMINGR_SCLH 0x03U
#define I2C1_TIMINGR_SCLL 0x09U
#elif defined(FAST_TIMINGS)
// Fast timings (400-100-100)
#define I2C1_TIMINGR_PRESC 0x00U
#define I2C1_TIMINGR_SCLDEL 0x00U
#define I2C1_TIMINGR_SDADEL 0x01U
#define I2C1_TIMINGR_SCLH 0x02U
#define I2C1_TIMINGR_SCLL 0x0AU
#elif defined(STANDARD_TIMINGS)
// Standard timings (100-100-100)
#define I2C1_TIMINGR_PRESC 0x00U
#define I2C1_TIMINGR_SCLDEL 0x02U
#define I2C1_TIMINGR_SDADEL 0x00U
#define I2C1_TIMINGR_SCLH 0x1DU
#define I2C1_TIMINGR_SCLL 0x2BU
#endif

// OLED timeout reimplemented in the keyboard-specific code
#define CUSTOM_OLED_TIMEOUT 60000

// Custom config Usage:
// 1 for enabled encoder modes (1 byte)
// 1 for OLED default mode (1 byte)
// 6 for 3x custom encoder settings, left, right, and press (18 bytes)
#define EECONFIG_KB_DATA_SIZE 20

// VIA lighting is handled by the keyboard-level code
#define VIA_CUSTOM_LIGHTING_ENABLE

// QP
#define QP_SCANRATE_THRESHOLD 250 // Turn off display if we're under this amount of scans
#define QP_TIMEOUT 30000
#define QP_UPDATE_INTERVAL 66 // ~15fps
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 0 // no auto timeout

#define DEBUG_MATRIX_SCAN_RATE