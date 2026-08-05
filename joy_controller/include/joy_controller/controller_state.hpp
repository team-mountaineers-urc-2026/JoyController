/**
 * @file controller_state.hpp
 * @author William Streck
 * @brief Definitions for keeping track of controller state.
 * @version 0.1
 * @date 2025-02-26
 * 
 */

#ifndef CONTROLLER_STATE_HPP
#define CONTROLLER_STATE_HPP

#include <stdint.h>

#define GREEN_PURPLE_CONTROLLERS ///< Define for green controller
// #define BLUEBERRY_USB ///< Define for blueberry controllers
// #define BLUEBERRY_BLUETOOTH ///< Define for blueberry bluetooth controllers

// Those only affect this header

/** Controller IDs **/

/** Button and Axis mapping.
 * Other than for confirming limits (which may be removed anyway), these are not used in the code.
 * Please use them as references when creating config files.
 */

#if defined(GREEN_PURPLE_CONTROLLERS) || defined(BLUEBERRY_BLUETOOTH)

    /** Button Event Numbers **/
    #define BUTTON_A 0 ///< Button A
    #define BUTTON_B 1 ///< Button B
    #ifdef BLUEBERRY_BLUETOOTH
        #define BUTTON_L4 2 ///< Button L4
    #endif
    #define BUTTON_X 3 ///< Button X
    #define BUTTON_Y 4 ///< Button Y
    #ifdef BLUEBERRY_BLUETOOTH
        #define BUTTON_R4 5 ///< Button R4
    #endif
    #define BUTTON_LB 6 ///< Button Left Bumper
    #define BUTTON_RB 7 ///< Button Right Bumper
    #define BUTTON_LT 8 ///< Button Left Trigger
    #define BUTTON_RT 9 ///< Button Right Trigger
    #define BUTTON_MINUS 10 ///< Button Minus
    #define BUTTON_PLUS 11 ///< Button Plus
    #define BUTTON_POWER 12 ///< Button Power
    #define BUTTON_LSTICK 13 ///< Button Left Stick
    #define BUTTON_RSTICK 14 ///< Button Right Stick
    // Empty - another rear trigger?

    /** Axis Event Numbers **/

    #define LEFT_HORIZONTAL_AXIS 0 ///< Left Axis - Positive right, negative left
    #define LEFT_VERTICAL_AXIS 1 ///< Right Axis - Positive down, negative up
    #define RIGHT_HORIZONTAL_AXIS 2 ///< Right Axis - Positive right, negative left
    #define RIGHT_VERTICAL_AXIS 3 ///< Right Axis - Positive down, negative up
    #define RIGHT_TRIGGER_AXIS 4 ///< Right Trigger Axis - Positive pressed, negative released (gradient - may still be negative when in)
    #define LEFT_TRIGGER_AXIS 5 ///< Left Trigger Axis - Positive pressed, negative released (gradient - may still be negative when in)
    #define DPAD_HORIZONTAL_AXIS 6 ///< D-Pad Horizontal Axis - Positive right, negative left, 0 released
    #define DPAD_VERTICAL_AXIS 7 ///< D-Pad Vertical Axis - Positive down, negative up, 0 released
#endif

#ifdef BLUEBERRY_USB
// These controllers do not have trigger buttons, only axis sensors

    /** Button Event Numbers **/
    #define BUTTON_A 0 ///< Button A
    #define BUTTON_B 1 ///< Button B
    #define BUTTON_X 2 ///< Button X
    #define BUTTON_Y 3 ///< Button Y
    #define BUTTON_LB 4 ///< Button Left Bumper
    #define BUTTON_RB 5 ///< Button Right Bumper
    #define BUTTON_MINUS 6 ///< Button Minus
    #define BUTTON_PLUS 7 ///< Button Plus
    #define BUTTON_POWER 8 ///< Button Home
    #define BUTTON_LSTICK 9 ///< Button Left Stick
    #define BUTTON_RSTICK 10 ///< Button Right Stick

    /** Axis Event Numbers **/

    #define LEFT_HORIZONTAL_AXIS 0 ///< Left Axis - Positive right, negative left
    #define LEFT_VERTICAL_AXIS 1 ///< Right Axis - Positive down, negative up
    #define LEFT_TRIGGER_AXIS 2 ///< Left Trigger Axis - Positive pressed, negative released (gradient - may still be negative when in)
    #define RIGHT_HORIZONTAL_AXIS 3 ///< Right Axis - Positive right, negative left
    #define RIGHT_VERTICAL_AXIS 4 ///< Right Axis - Positive down, negative up
    #define RIGHT_TRIGGER_AXIS 5 ///< Right Trigger Axis - Positive pressed, negative released (gradient - may still be negative when in)
    #define DPAD_HORIZONTAL_AXIS 6 ///< D-Pad Horizontal Axis - Positive right, negative left, 0 released
    #define DPAD_VERTICAL_AXIS 7 ///< D-Pad Vertical Axis - Positive down, negative up, 0 released
#endif

#define BUTTON_EVENT_PRESSED 1 ///< Button pressed event value
#define BUTTON_EVENT_RELEASED 0 ///< Button released event value

#define AXIS_MAX 32767 ///< Maximum value for an axis
#define AXIS_MIN -32767 ///< Minimum value for an axis - note 1 higher than int16 min

struct controller_state {
    uint16_t buttons; ///< Bitfield for buttons
    double axes[8]; ///< Array for axes
};

controller_state dev_state; ///< For keeping track of both controller states

void set_button_state(uint8_t button, int16_t value);

void set_axis_state(uint8_t axis, int16_t value);

#endif
 