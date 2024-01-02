/* Copyright 2019 Keebio
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
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif

#include QMK_KEYBOARD_H

bool is_key1_down = false;
bool is_key2_down = false;

bool ignore_next_key1_up = false;

uint32_t lastTime = 0;
uint32_t delayTime = 3000;

float imperial_march[][2] = SONG(IMPERIAL_MARCH_U);
float mario_gameover[][2] = SONG(MARIO_GAMEOVER_U);
float mario_theme[][2] = SONG(MARIO_THEME_U);

static const char* const no_text[] = 
{
    "Not possible\n",
    "Not likely\n",
    "Not going to happen\n",
    "Not going to work\n",
    "Not the right fit\n",
    "Not at this time\n",
    "Not today\n",
    "No can do\n",
    "No way\n"
    "Nope\n",
    "No\n"
};
#define NO_ENTRIES 11

static const char* const yes_text[] = 
{
    "Under consideration\n",
    "Potentially doable\n",
    "May be possible\n",
    "Might be feasible\n",
    "Not impossible\n",
    "Maybe\n",
    "Perhaps\n",
    "Possibly\n",
    "Conceivably\n"
};
#define YES_ENTRIES 9

enum layers 
{
    _DEFAULT,
    _SPECIAL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = 
{
    [0] = LAYOUT(KC_MUTE, KC_MPLY),
    [1] = LAYOUT(KC_MUTE, KC_MPLY),
};

const rgblight_segment_t PROGMEM layer_lights_special[] = RGBLIGHT_LAYER_SEGMENTS
(
    {0, 1, HSV_RED},
    {2, 1, HSV_BLUE}
);

const rgblight_segment_t PROGMEM layer_lights_special_red[] = RGBLIGHT_LAYER_SEGMENTS
(
    {0, 3, HSV_RED}
);

const rgblight_segment_t PROGMEM layer_lights_special_blue[] = RGBLIGHT_LAYER_SEGMENTS
(
    {0, 3, HSV_BLUE}
);

const rgblight_segment_t* const PROGMEM rgb_lights_layers[] = RGBLIGHT_LAYERS_LIST 
(
    layer_lights_special,
    layer_lights_special_red,
    layer_lights_special_blue
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
    // Update RGB light layer
	rgblight_set_layer_state(0, layer_state_cmp(state, _SPECIAL));
    
    return state;
}

// Process key handling
bool process_record_user(uint16_t keycode, keyrecord_t *record) 
{
    if (lastTime > 0 && timer_elapsed32(lastTime) <= delayTime)
    {
        return false;
    }
    
    switch (keycode) 
    {   
        case KC_MUTE:
            if (record->event.pressed)
            {
                is_key1_down = true;
                
                if (IS_LAYER_ON(_SPECIAL))
                {
                    return false;
                }
            }
            else 
            {
                is_key1_down = false;
                
                if (ignore_next_key1_up)
                {
                    ignore_next_key1_up = false;
                    return false;
                }

                if (is_key2_down)
                {
                    return false;
                }

                if (IS_LAYER_ON(_SPECIAL))
                {
                    rgblight_set_layer_state(1, true);
                    
                    int index = rand() / (RAND_MAX / NO_ENTRIES + 1);
                    
                    send_string(no_text[index]);
                    PLAY_SONG(mario_gameover);
                    
                    lastTime = timer_read32();
                    delayTime = 3000;
                    
                    return false;
                }               
            }
            break;

        case KC_MPLY:
            if (record->event.pressed)
            {
                is_key2_down = true;
                
                if (IS_LAYER_ON(_SPECIAL))
                {
                    return false;
                }
            }
            else 
            {
                is_key2_down = false;
                
                // Handle layer switching
                if (is_key1_down)
                {
                    if (IS_LAYER_ON(_SPECIAL))
                    {
                        layer_off(_SPECIAL);
                    }
                    else
                    {
                        layer_on(_SPECIAL);
                    }
                    
                    ignore_next_key1_up = true;
                    return false;
                }                

                if (IS_LAYER_ON(_SPECIAL))
                {
                    rgblight_set_layer_state(2, true);
                    
                    int index = rand() / (RAND_MAX / YES_ENTRIES + 1);
                    send_string(yes_text[index]);
                    
                    PLAY_SONG(mario_theme);
                    
                    lastTime = timer_read32();
                    delayTime = 1750;
                    
                    return false;
                }
            }
            break;
    }
    
    return true;
}

// Reset RGB
void matrix_scan_user(void) 
{
    if (lastTime > 0 && timer_elapsed32(lastTime) > delayTime) 
    {
        lastTime = 0;
        rgblight_set_layer_state(0, IS_LAYER_ON(_SPECIAL));
        rgblight_set_layer_state(1, false);
        rgblight_set_layer_state(2, false);
    }
}
