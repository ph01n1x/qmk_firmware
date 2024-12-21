/*
Copyright 2021 Spencer Deven <splitlogicdesign@gmail.com>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H

#include "transactions.h"

enum custom_keycodes 
{
    KC_ENCT = SAFE_RANGE,  // Toggle encoder mode
};

// Default keymap. This can be changed in Via. Use oled.c and encoder.c to change beavior that Via cannot change.
enum layers {
    _DEFAULT,
    _LOWER,
    _RAISE,
    _SPECIAL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
      KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                           KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,  KC_LBRC,                       KC_RBRC,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
      MO(3),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,  KC_MINS,                        KC_EQL,  KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,  KC_GRV, KC_MUTE,      KC_ENCT, KC_DEL,KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT ,
      KC_LCTL, KC_LALT, KC_LGUI, KC_APP, MO(1),       KC_SPC,   KC_ENT,           KC_ENT,   KC_SPC,  MO(2),    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
    ),
		
	[1] = LAYOUT(
      _______, KC_F1, KC_F2, KC_F3, KC_F4,   KC_F5,                                KC_F6,   KC_F7,   KC_F8, KC_F9, KC_F10, KC_F11,
      KC_PSLS, KC_P7, KC_P8, KC_P9, _______, _______, _______,                   _______, _______, KC_PSLS, KC_P7, KC_P8, KC_P9, KC_F12,
      KC_CAPS, KC_P4, KC_P5, KC_P6, _______, _______, _______,                   _______, _______, _______, KC_P4, KC_P5, KC_P6, _______,
      _______, KC_P1, KC_P2, KC_P3, _______, _______, _______, _______,  _______, _______, _______, _______, KC_P1, KC_P2, KC_P3, _______,
      _______, KC_P0, KC_PDOT, KC_PENT, _______,     _______,  _______,   _______,    _______,   _______, KC_P0, KC_PDOT, KC_PENT, _______
      ),
	  
	[2] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                           KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
      KC_PSLS, KC_P7, KC_P8, KC_P9, _______, _______, _______,                    _______, _______, KC_PSLS, KC_P7, KC_P8, KC_P9, KC_F12,
      KC_CAPS, KC_P4, KC_P5, KC_P6, _______, _______, _______,                    _______, _______, _______, KC_P4, KC_P5, KC_P6, _______,
      _______, KC_P1, KC_P2, KC_P3, _______, _______, _______, _______,  _______, _______, _______, _______, KC_P1, KC_P2, KC_P3, _______,
      _______, KC_P0, KC_PDOT, KC_PENT, _______,    _______,   _______,  _______,    _______,    _______, KC_P0, KC_PDOT, KC_PENT, _______
      ),

	[3] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
      _______, _______, _______, _______, _______, _______, _______,                   _______, _______, QK_RBT, _______, _______, _______, KC_F12,
      _______, _______, _______, _______, _______, _______, _______,                   _______, _______, RGB_TOG, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_MOD, RGB_SPI, RGB_HUI, RGB_SAI, RGB_VAI,
      _______, _______, _______, _______, _______,      _______,     _______, _______,     _______,      RGB_RMOD, RGB_SPD, RGB_HUD, RGB_SAD, RGB_VAD
      )

};

#define ANIMATION_SLEEP 0
#define ANIMATION_AWAKE 1
#define ANIMATION_NORMAL 2
#define ANIMATION_KAKI 3
#define ANIMATION_YAWN 4
#define ANIMATION_RUN 5
#define ANIMATION_UP 6
#define ANIMATION_UPRIGHT 7
#define ANIMATION_DOWN 8
#define ANIMATION_DOWNRIGHT 9
#define ANIMATION_JUMP 10
#define ANIMATION_RTOGIL 11

bool new_animation_set = false;
uint8_t target_animation = 0;
uint8_t current_animation = 0;
uint8_t target_animation_runs = 0;

// For triggering mouse manually
bool mouse_triggered = false;

uint32_t keyevent_timer = 0;

// Handling of super Alt+Tab / Ctrl+Tab
bool is_alt_tab_active = false;
bool is_ctrl_tab_active = false;
uint16_t super_tab_timer = 0;

// Right encoder mode
#define ENCODER_WIN 0
#define ENCODER_TAB 1
#define ENCODER_PAGE 2
#define ENCODER_BOOKMARK 3

uint8_t encoder2_mode = ENCODER_WIN;

bool is_oled_enabled = true;

// Handling of the special layer
layer_state_t layer_state_set_user(layer_state_t state) 
{
    state = update_tri_layer_state(state, _LOWER, _RAISE, _SPECIAL);
    
    return state;
}

// Process key handling
bool process_record_user(uint16_t keycode, keyrecord_t *record) 
{
    keyevent_timer = timer_read32();
    is_oled_enabled = true;
    
    switch (keycode) 
    {
        case KC_UP:
        case KC_PGUP:
        if (current_animation != ANIMATION_DOWN)
            {
                target_animation = ANIMATION_UP;
            }
            
            target_animation_runs = 2;
            new_animation_set = true;
            break;
            
        case KC_DOWN:  
        case KC_PGDN:
            if (current_animation != ANIMATION_DOWN)
            {
                target_animation = ANIMATION_DOWN;
            }
            
            target_animation_runs = 2;
            new_animation_set = true;
            break;
        
        case KC_TAB:
        case KC_SPACE:
        case KC_LEFT:
        case KC_RIGHT:
        case KC_HOME:
        case KC_END:
            if (current_animation != ANIMATION_JUMP)
            {
                target_animation = ANIMATION_JUMP;
            }
            
            target_animation_runs = 1;
            new_animation_set = true;
            break;

        case MT(MOD_LCTL, KC_BSPC):
        case MT(MOD_RCTL, KC_DELETE):
        case KC_BSPC:
        case KC_DELETE:
        case KC_ENTER:
            if (current_animation != ANIMATION_RTOGIL)
            {
                target_animation = ANIMATION_RTOGIL;
            }
            
            target_animation_runs = 3;
            new_animation_set = true;
            break;

        case KC_F24:
        case KC_ENCT:
            if (record->event.pressed) 
            {
                encoder2_mode = (encoder2_mode + 1) % 4;
            }
            break;
            
        case KC_MPLY:
            // Media play / pause only triggers the mouse
            mouse_triggered = true;
            break;

        case RGB_M_R:
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_MOOD);
            break;

        case RGB_M_SW:
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL + 2);
            break;

        case RGB_SPI:
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL + 5);
            break;

        case RGB_SPD:
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL);
            break;

        default:
            if (current_animation != ANIMATION_RUN)
            {
                target_animation = ANIMATION_RUN;
            }
            
            target_animation_runs = 2;
            new_animation_set = true;
            break;
    }
    
    return true;
}

// Turn off super Alt-Tab
void matrix_scan_user(void) 
{ 
    if (timer_elapsed(super_tab_timer) > 1000)
    {
        if (is_alt_tab_active)
        {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
        
        if (is_ctrl_tab_active)
        {
            unregister_code(KC_LCTL);
            is_ctrl_tab_active = false;
        }
        
        super_tab_timer = 0;
    }
}

#include "oled.c"
#include "encoder.c"
#include "datasync.c"

// Run after keyboard is initialized
void keyboard_post_init_user(void) 
{
    transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
    
    if (is_keyboard_master()) 
    {
        current_animation = ANIMATION_SLEEP;
        target_animation = ANIMATION_SLEEP;
        target_animation_runs = 0;
    }
    else
    {
        current_animation = ANIMATION_AWAKE;
        target_animation = ANIMATION_AWAKE;
        target_animation_runs = 3;
    }
}

// House keeping after each processing
void housekeeping_task_user(void) 
{
    if (is_keyboard_master() 
        && timer_elapsed32(keyevent_timer) > OLED_TIMEOUT)     
    {
        is_oled_enabled = false;
    }
    
    if (is_keyboard_master()) 
    {
        user_sync_a_master_to_slave();
    }
}
    