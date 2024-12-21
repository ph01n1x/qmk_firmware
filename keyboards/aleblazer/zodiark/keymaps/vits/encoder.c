#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) 
{
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
