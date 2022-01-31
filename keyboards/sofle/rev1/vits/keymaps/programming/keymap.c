 /* Copyright 2020 Josef Adamcik
  * Modification for VIA support and RGB underglow by Jens Bonk-Wiltfang
  * 
  * This program is free software: you can redistribute it and/or modify 
  * it under the terms of the GNU General Public License as published by 
  * the Free Software Foundation, either version 2 of the License, or 
  * (at your option) any later version. 
  * 
  * This program is distributed in the hope that it will be useful, 
  * but WITHOUT ANY WARRANTY; without even the implied warranty of 
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
  * GNU General Public License for more details. 
  * 
  * You should have received a copy of the GNU General Public License 
  * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
  */ 

#include QMK_KEYBOARD_H

enum custom_keycodes 
{
    KC_ENCT = SAFE_RANGE,  // Toggle encoder mode
};

// Default keymap. This can be changed in Via. Use oled.c and encoder.c to change beavior that Via cannot change.
enum sofle_layers {
    _DEFAULT,
    _LOWER,
    _RAISE,
    _SPECIAL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = 
{
/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  =   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|  MUTE |    | PLAY  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      | BSpc |      |/       /         \      \ |      | Del  |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[0] = LAYOUT(
  KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
  XXXXXXX,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_MUTE, KC_MPLY, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
            KC_LGUI, KC_LALT, LCTL_T(KC_BSPC), TT(1),   KC_ENT,  KC_SPC,  TT(2),   RCTL_T(KC_DEL), KC_RALT, KC_RGUI
),

/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |      |      | S(U) |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  %   |  *   |  !   |  (   |  )   |  +   |                    | PgUp | Home | (U)  | End  |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  $   |  @   |  &   |  {   |  }   |  =   |-------.    ,-------| S(L) | (L)  | (D)  | (R)  | S(R) |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |  _   |  [   |  [   |  ]   |  -   |-------|    |-------| PgDn | S(H) | S(D) | s(E) |  \   |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      | /       /       \      \  |      |S(Del)|      | Menu |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[1] = LAYOUT(
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     XXXXXXX,    XXXXXXX,    S(KC_UP),   XXXXXXX,   XXXXXXX,    _______,
  KC_PERC, KC_PAST, KC_EXLM, KC_LPRN, KC_RPRN, KC_PLUS,                   KC_PGUP,    KC_HOME,    KC_UP,      KC_END,    XXXXXXX,    _______,
  KC_DLR,  KC_AT,   KC_AMPR, KC_LCBR, KC_RCBR, KC_EQL,                    S(KC_LEFT), KC_LEFT,    KC_DOWN,    KC_RGHT,   S(KC_RGHT), _______,
  _______, KC_UNDS, KC_PIPE, KC_LBRC, KC_RBRC, KC_PMNS, _______, _______, KC_PGDN,    S(KC_HOME), S(KC_DOWN), S(KC_END), KC_BSLS,    _______,
                    _______, _______, _______, _______, _______, _______, _______,    S(KC_DEL),  _______,    KC_APP
),

/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |      |      |      |      |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      | /       /       \      \  |      |      |      |      |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[2] = LAYOUT(
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, KC_F12,
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[3] = LAYOUT(
  KC_NLCK, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  KC_SLCK, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, KC_PSCR, _______,
  KC_CAPS, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, KC_ENCT, _______, _______, _______, _______, _______,
                    _______, _______, _______, _______, _______, _______, _______, KC_INS,  _______, _______
)
};

#define ANIMATION_SLEEP 0
#define ANIMATION_NORMAL 1
#define ANIMATION_MATI 2
#define ANIMATION_KAKI 3
#define ANIMATION_RUN 4
#define ANIMATION_JUMP 5
#define ANIMATION_RTOGIL 6

uint8_t target_animation = 0;
uint8_t current_animation = 0;
uint8_t target_animation_runs = 0;

uint32_t keyevent_timer = 0;

uint8_t current_wpm = 0;

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
    target_animation = ANIMATION_RUN;
    
    switch (keycode) 
    {
        case KC_ESCAPE:
        case KC_GESC:
            target_animation = ANIMATION_KAKI;
            target_animation_runs = 3;
            break;
            
        case KC_LGUI:
        case KC_RGUI:
        case KC_MUTE:
        case KC_VOLU:
        case KC_VOLD:
            target_animation = ANIMATION_MATI;
            target_animation_runs = 3;
            break;
            
        case KC_TAB:           
        case KC_SPACE:
        case KC_LEFT:
        case KC_RIGHT:
        case KC_UP:
        case KC_DOWN:
        case KC_PGUP:
        case KC_PGDN:
        case KC_HOME:
        case KC_END:
            if (current_animation != ANIMATION_JUMP)
            {
                target_animation = ANIMATION_JUMP;
                target_animation_runs = 1;
            }
            break;

        case MT(MOD_LCTL, KC_BSPACE):
        case MT(MOD_RCTL, KC_DELETE):
        case KC_BSPACE:
        case KC_DELETE:
        case KC_ENTER:
            target_animation = ANIMATION_RTOGIL;
            target_animation_runs = 3;
            break;

        case KC_F24:
        case KC_ENCT:
            if (record->event.pressed) 
            {
                encoder2_mode = (encoder2_mode + 1) % 4;
            }
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