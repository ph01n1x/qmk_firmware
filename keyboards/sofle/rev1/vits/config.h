#pragma once

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFC32
#define PRODUCT_ID      0x0287
#define DEVICE_VER      0x0001
#define MANUFACTURER    JosefAdamcik
#define PRODUCT         Sofle

/* key matrix size */
// Rows are doubled-up
#define MATRIX_ROWS 10
#define MATRIX_COLS 6

// wiring of each half
#define MATRIX_ROW_PINS { C6, D7, E6, B4, B5 }
#define MATRIX_COL_PINS { F6, F7, B1, B3, B2, B6 }
#define DIODE_DIRECTION COL2ROW

#ifdef TAPPING_TERM
    #undef TAPPING_TERM
    #define TAPPING_TERM 150
#endif
#define IGNORE_MOD_TAP_INTERRUPT
#define TAP_CODE_DELAY 10

#define DEBOUNCE 5

/* encoder support */
#define ENCODERS_PAD_A { F5 }
#define ENCODERS_PAD_B { F4 }
#define ENCODERS_PAD_A_RIGHT { F4 }
#define ENCODERS_PAD_B_RIGHT { F5 }

#ifdef ENCODER_RESOLUTION
    #undef ENCODER_RESOLUTION
    #define ENCODER_RESOLUTION 4
#endif

/* communication between sides */
#define USE_SERIAL
#define SERIAL_USE_MULTI_TRANSACTION
#define SOFT_SERIAL_PIN D2

// #define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_OLED_ENABLE

#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#ifndef NO_DEBUG
    #define NO_DEBUG
#endif
#if !defined(NO_PRINT) && !defined(CONSOLE_ENABLE)
    #define NO_PRINT
#endif