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

#pragma once

#ifdef DEVICE_VER
    #undef DEVICE_VER
    #define DEVICE_VER 0x0002
#endif

#ifdef PRODUCT
    #undef PRODUCT
    #define PRODUCT Sofle RGB
#endif

#ifdef DESCRIPTION
    #undef DESCRIPTION
    #define DESCRIPTION Sofle RGB
#endif

#define TAPPING_TOGGLE 4

#define IGNORE_MOD_TAP_INTERRUPT

#define NO_ACTION_ONESHOT

/* The way how "handedness" is decided (which half is which),
see https://docs.qmk.fm/#/feature_split_keyboard?id=setting-handedness
for more options.
*/

//Add RGB underglow
#define RGB_DI_PIN D3
#define RGBLED_NUM 73
#define RGBLED_SPLIT {36, 36}

#define RGBLIGHT_LAYERS
#define RGBLIGHT_SLEEP

#define RGBLIGHT_LIMIT_VAL 120
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17
    
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL