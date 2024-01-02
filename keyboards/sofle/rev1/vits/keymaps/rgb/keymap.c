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
  XXXXXXX, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
  KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_MUTE, KC_MPLY, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
            KC_LGUI, KC_LALT, LCTL_T(KC_BSPC), TT(1),   KC_ENT,  KC_SPC,  TT(2),   RCTL_T(KC_DEL), KC_RALT, KC_RGUI
),

/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    | Desk>|      | S(U) |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  %   |  *   |  !   |  (   |  )   |  +   |                    | PgUp | Home | (U)  | End  |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  $   |  @   |  &   |  {   |  }   |  =   |-------.    ,-------| S(L) | (L)  | (D)  | (R)  | S(R) |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |  _   |  [   |  [   |  ]   |  -   |-------|    |-------| PgDn | S(H) | S(D) | s(E) |  \   |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      | /       /       \      \  |      |      |      | Menu |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[1] = LAYOUT(
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     G(S(KC_LEFT)), XXXXXXX,    S(KC_UP),   XXXXXXX,   XXXXXXX,    _______,
  KC_PERC, KC_PAST, KC_EXLM, KC_LPRN, KC_RPRN, KC_PLUS,                   KC_PGUP,       KC_HOME,    KC_UP,      KC_END,    XXXXXXX,    _______,
  KC_DLR,  KC_AT,   KC_AMPR, KC_LCBR, KC_RCBR, KC_EQL,                    S(KC_LEFT),    KC_LEFT,    KC_DOWN,    KC_RGHT,   S(KC_RGHT), _______,
  _______, KC_UNDS, KC_PIPE, KC_LBRC, KC_RBRC, KC_PMNS, _______, _______, KC_PGDN,       S(KC_HOME), S(KC_DOWN), S(KC_END), KC_BSLS,    _______,
                    _______, _______, _______, _______, _______, _______, _______,       _______,    _______,    KC_APP
),

/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |      |      |      |      |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      | A-Tab| Cut  | Copy | Paste|      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      | /       /       \      \  |      |      |      |      |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[2] = LAYOUT(
  _______, KC_F1,     KC_F2,   KC_F3,    KC_F4,    KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  _______, _______,   _______, A(KC_F4), C(KC_F4), C(KC_T),                   KC_PGUP, KC_WBAK, _______, KC_WFWD, _______, KC_F12,
  _______, _______,   _______, _______,  _______,  _______,                   _______, _______, _______, _______, _______, _______,
  _______, A(KC_TAB), C(KC_X), C(KC_C),  C(KC_V),  _______, _______, _______, KC_PGDN, _______, _______, _______, _______, _______,
                      _______, _______,  _______,  _______, _______, _______, _______, _______, _______, _______
),

[3] = LAYOUT(
  KC_NLCK, RGB_MOD, RGB_M_SW, RGB_M_P, _______, _______,                   _______, _______, _______, _______, _______, _______,
  KC_SLCK, RGB_HUD, RGB_VAI,  RGB_HUI, _______, _______,                   _______, _______, _______, _______, KC_PSCR, _______,
  KC_CAPS, RGB_SPD, RGB_VAD,  RGB_SPI, _______, _______,                   _______, _______, _______, _______, _______, _______,
  _______, _______, RGB_TOG,  KC_CALC, _______, _______, _______, _______, KC_ENCT, _______, _______, _______, _______, _______,
                    _______,  _______, _______, _______, _______, _______, _______, KC_INS,  _______, _______
)
};

#define ANIMATION_SLEEP 0
#define ANIMATION_NORMAL 1
#define ANIMATION_RUN 2
#define ANIMATION_JUMP 3
#define ANIMATION_RTOGIL 4

uint8_t target_animation = 0;
uint8_t current_animation = 0;
uint8_t target_animation_runs = 0;

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

// RGB light layers
#define INDICATOR_BRIGHTNESS 30

#define HSV_OVERRIDE(h, s, v, override) h, s , override

#define SET_INDICATOR(hsv) \
	{0,    1, HSV_OVERRIDE(hsv, INDICATOR_BRIGHTNESS)}, \
    {36+0, 1, HSV_OVERRIDE(hsv, INDICATOR_BRIGHTNESS)}

#define SET_LAYERID(hsv) \
	{0,     1, HSV_OVERRIDE(hsv, INDICATOR_BRIGHTNESS)}, \
    {36+0,  1, HSV_OVERRIDE(hsv, INDICATOR_BRIGHTNESS)}, \
    {1,     6, hsv}, \
    {36+1,  6, hsv}, \
	{26,    2, hsv}, \
    {36+26, 2, hsv}
    
#define SET_FROW(hsv) \
	{12,    1, hsv}, \
    {36+10, 3, hsv}, \
	{21,    2, hsv}, \
    {36+21, 2, hsv}, \
	{31,    2, hsv}, \
    {36+31, 2, hsv}

#define SET_ARROW(hsv) \
    {36+23, 2, hsv}, \
    {36+29, 1, hsv}, \
    {36+19, 1, hsv}

const rgblight_segment_t PROGMEM layer_lights_default[] = RGBLIGHT_LAYER_SEGMENTS
(
    SET_INDICATOR(HSV_GREEN)
);

const rgblight_segment_t PROGMEM layer_lights_lower[] = RGBLIGHT_LAYER_SEGMENTS
(
    SET_LAYERID(HSV_BLUE),
    SET_ARROW(HSV_RED),
    
    // Shift+Arrow
    {36+22, 1, HSV_PINK},
    {36+25, 1, HSV_PINK},
    {36+14, 1, HSV_PINK},
    {36+34, 1, HSV_PINK},
    
    // Home, End
    {36+30, 1, HSV_BLUE},
    {36+20, 1, HSV_BLUE},    
    {36+28, 1, HSV_AZURE},
    {36+18, 1, HSV_AZURE},
    
    // Page Up, Page Down
    {36+33, 1, HSV_PURPLE},
    {36+35, 1, HSV_PURPLE},
    
    // Action
    {36+32, 1, HSV_RED},
    
    {36+31, 1, HSV_OFF},
    {36+21, 1, HSV_OFF},
    {36+12, 2, HSV_OFF},
    {36+8,  4, HSV_OFF}
);

const rgblight_segment_t PROGMEM layer_lights_raise[] = RGBLIGHT_LAYER_SEGMENTS
(
    SET_LAYERID(HSV_PURPLE),
    SET_FROW(HSV_RED)
);

const rgblight_segment_t PROGMEM layer_lights_special[] = RGBLIGHT_LAYER_SEGMENTS
(
    SET_LAYERID(HSV_RED),
    
    // RGB control
    {12,    3, HSV_PINK},
    {21,    1, HSV_RED},
    {22,    1, HSV_PINK},
    {13,    1, HSV_AZURE},
    {23,    1, HSV_AZURE},
    {19,    1, HSV_BLUE},
    {20,    1, HSV_BLUE},
    {14,    1, HSV_BLUE},
    {24,    1, HSV_BLUE},
    {18,    1, HSV_RED},
    
    // Action
    {25,    1, HSV_RED},
    {36+13, 1, HSV_RED},
    {36+35, 1, HSV_RED},
    {36+17, 1, HSV_RED}
);

const rgblight_segment_t* const PROGMEM rgb_lights_layers[] = RGBLIGHT_LAYERS_LIST
(
    layer_lights_default,
    layer_lights_lower,
    layer_lights_raise,
    layer_lights_special
);

void keyboard_post_init_user(void) 
{
    // Enable the LED layers
    rgblight_layers = rgb_lights_layers;
	rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL+1);
}

// Handling of the special layer
layer_state_t layer_state_set_user(layer_state_t state) 
{
    state = update_tri_layer_state(state, _LOWER, _RAISE, _SPECIAL);
    
    // Update RGB light layer
	rgblight_set_layer_state(0, layer_state_cmp(state, _DEFAULT));
	rgblight_set_layer_state(1, layer_state_cmp(state, _LOWER));
	rgblight_set_layer_state(2, layer_state_cmp(state, _RAISE));
    
    if (layer_state_cmp(state, _SPECIAL))
    {
        rgblight_set_layer_state(0, false);
        rgblight_set_layer_state(1, false);
        rgblight_set_layer_state(2, false);
        rgblight_set_layer_state(3, true);
    }
    else
    {
        rgblight_set_layer_state(3, false);
    }
    
    
    return state;
}

// Process key handling
bool process_record_user(uint16_t keycode, keyrecord_t *record) 
{
    keyevent_timer = timer_read32();
    target_animation = ANIMATION_RUN;
    
    switch (keycode) 
    {
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