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

//Setting up what encoder rotation does. If your encoder can be pressed as a button, that function can be set in Via.

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) 
{
    keyevent_timer = timer_read32();
    
    if (index == 0) 
    {
        if (clockwise) 
        {
            tap_code(KC_VOLU);
        } 
        else 
        {
            tap_code(KC_VOLD);
        }
        
        target_animation = ANIMATION_MATI;
        target_animation_runs = 3;
    } 
    else if (index == 1) 
    {
        if (encoder2_mode == ENCODER_WIN)
        {
            if (clockwise) 
            {
                if (!is_alt_tab_active) 
                {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                
                super_tab_timer = timer_read();
                tap_code(KC_TAB);           
            } 
            else 
            {
                if (!is_alt_tab_active) 
                {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                
                super_tab_timer = timer_read();
                tap_code16(S(KC_TAB));  
            }
        }
        else if (encoder2_mode == ENCODER_TAB)
        {
            if (clockwise) 
            {
                if (!is_ctrl_tab_active) 
                {
                    is_ctrl_tab_active = true;
                    register_code(KC_LCTL);
                }
                
                super_tab_timer = timer_read();
                tap_code(KC_TAB);           
            } 
            else 
            {
                if (!is_ctrl_tab_active) 
                {
                    is_ctrl_tab_active = true;
                    register_code(KC_LCTL);
                }
                
                super_tab_timer = timer_read();
                tap_code16(S(KC_TAB));  
            }
        }
        else if (encoder2_mode == ENCODER_PAGE)
        {
            if (clockwise) 
            {
                tap_code(KC_PGDN);
            }
            else
            {
                tap_code(KC_PGUP);
            }
        }
        else if (encoder2_mode == ENCODER_BOOKMARK)
        {
            if (clockwise) 
            {
                tap_code16(C(KC_B));
                tap_code16(C(KC_N));
            }
            else
            {
                tap_code16(C(KC_B));
                tap_code16(C(KC_P));
            }
        }
        
        target_animation = ANIMATION_JUMP;
        target_animation_runs = 2;
    }
    return false;
}

#endif
