//Sets up what the OLED screens display.

#ifdef OLED_ENABLE

#include "images.c"

// Draw an image
// x is specified as pixels
// y is specified as OLED cursor line position
// Image must be of height of multiple of s
static void draw_image(const char *image, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool reverse)
{
    uint8_t maxLine = height / 8;
    uint16_t offset = 0;
    
    if (reverse)
    {
        for (uint8_t currentLine = 0; currentLine < maxLine; currentLine++)
        {
            offset = (y + currentLine) * OLED_DISPLAY_HEIGHT + x;
            
            for (uint8_t currentColumn = 0; currentColumn < width; currentColumn++)
            {
                char data = pgm_read_byte(image + currentLine * width + (width - currentColumn - 1));
                oled_write_raw_byte(data, offset + currentColumn);
            }
        }        
    }
    else
    {
        for (uint8_t currentLine = 0; currentLine < maxLine; currentLine++)
        {
            offset = (y + currentLine) * OLED_DISPLAY_HEIGHT + x;
            
            for (uint8_t currentColumn = 0; currentColumn < width; currentColumn++)
            {
                char data = pgm_read_byte(image + currentLine * width + currentColumn);
                oled_write_raw_byte(data, offset + currentColumn);
            }
        }
    }
}

// Initialize OLED screen
oled_rotation_t oled_init_user(oled_rotation_t rotation) 
{
    if (is_keyboard_master()) 
    {
        return OLED_ROTATION_90;
    }
    else
    {
        return OLED_ROTATION_270;
    }
    return rotation;
}
    
// Render the top logo
static void render_logo(bool reverse) 
{
    draw_image(phoenix, 16, 0, 32, 32, reverse);
}

uint32_t animation_timer = 0;
uint16_t frame_duration = 400;
uint8_t current_frame = 0;

uint8_t current_animation_runs = 0;

int8_t current_x = 0;
bool movement_reverse = false;

#define CHARACTER_NEKO 0
#define CHARACTER_SAKURA 1

// Draws one frame of the animation sequence
void draw_animation_frame(int y, int character) 
{
    // First animation after sleep will always be awake
    if (current_animation != target_animation)
    {
        if (current_animation == ANIMATION_SLEEP)
        {
            target_animation = ANIMATION_AWAKE;
            target_animation_runs = 3;
        }
        
        current_animation = target_animation; 
        current_frame = 0;
        current_animation_runs = 0;
    }
    
    // Neko animation switch eventually to sleep
    if (character == CHARACTER_NEKO
        && current_animation !=  ANIMATION_SLEEP 
        && timer_elapsed32(keyevent_timer) > 10000)
    {
        if (timer_elapsed32(keyevent_timer) > 26000)
        {
            if (current_animation != ANIMATION_SLEEP)
            {
                current_frame = 0;
            }
            current_animation = ANIMATION_SLEEP;
            target_animation = current_animation;
        }
        else if (timer_elapsed32(keyevent_timer) > 22000)
        {
            if (current_animation != ANIMATION_YAWN)
            {
                current_frame = 0;
            }
            current_animation = ANIMATION_YAWN;
            target_animation = current_animation;
        }   
        else if (timer_elapsed32(keyevent_timer) > 18000)
        {
            if (current_animation != ANIMATION_KAKI)
            {
                current_frame = 0;
            }
            current_animation = ANIMATION_KAKI;
            target_animation = current_animation;
        }              
        else if (timer_elapsed32(keyevent_timer) > 10000)
        {
            if (current_animation != ANIMATION_NORMAL)
            {
                current_frame = 0;
            }
            current_animation = ANIMATION_NORMAL;
            target_animation = current_animation;
        }  
    }
    
    // Update the frame durations
    if (current_animation == ANIMATION_AWAKE 
        || current_animation == ANIMATION_NORMAL
        || current_animation == ANIMATION_YAWN)
    {
        frame_duration = 800;
    }
    else if (current_animation == ANIMATION_RUN || current_animation == ANIMATION_JUMP)
    {
        frame_duration = 400;
    }
    else
    {
        frame_duration = 400;
    }
    
    // Draw the animation
    bool toggleMovement = false;

    uint8_t targetY = y;
    uint8_t frameIndex = 0; 
    const char* image = 0;
    bool runCompleted = false;
    switch (current_animation)
    {
        case ANIMATION_AWAKE:               
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);      
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_awake[frameIndex];
            }   
            else
            {
                image = neko_awake[frameIndex];
            }
            break;
            
        case ANIMATION_NORMAL:               
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_mati[frameIndex];
            }
            else
            {
                image = neko_mati[frameIndex];
            }
            break;

        case ANIMATION_KAKI:
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_mati[frameIndex];
            }
            else
            {
                image = neko_kaki[frameIndex];
            }
            break;
            
        case ANIMATION_YAWN:
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_mati[frameIndex];
            }
            else
            {
                image = neko_yawn[frameIndex];
            }
            break;                
            
        case ANIMATION_RUN:
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_right[frameIndex];
            }
            else
            {
                image = neko_right[frameIndex];
            }
            break;
          
        case ANIMATION_UP:
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_up[frameIndex];
            }
            else
            {
                image = neko_up[frameIndex];
            }
            break;
            
        case ANIMATION_DOWN:
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            image = neko_down[frameIndex];
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_down[frameIndex];
            }
            else
            {
                image = neko_down[frameIndex];
            }
            break;

        case ANIMATION_JUMP:
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_jump[frameIndex];
            }
            else
            {
                image = neko_jump[frameIndex];
            }
            
            if (character == CHARACTER_NEKO)
            {
                if (frameIndex == 0)
                {
                    targetY = y - 2;
                }
                else
                {
                    targetY = y - 1;
                }
            }
            break;

        case ANIMATION_RTOGIL:
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_rtogil[frameIndex];
            }
            else
            {
                image = neko_rtogil[frameIndex];
            }
            break;

        default:
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_mati[frameIndex];
            }
            else
            {
                image = neko_sleep[frameIndex];
            }
            
            break;
    }
    
    // Increment the animation frame and handle repeated runs
    if (target_animation_runs > 0)
    {
        if (frameIndex == 1)
        {
            current_animation_runs++;
        }
    
        if (current_animation_runs >= target_animation_runs)
        {
            runCompleted = true;
        }
    }
 
    if (image != 0)
    {
        // Erase previous contents
        if (character == CHARACTER_NEKO)
        {
            for (uint8_t py = (y - 2); py < (y - 2) + 6; py++)
            {
                for (uint8_t px = 0; px < OLED_DISPLAY_HEIGHT; px++)
                {
                    uint16_t offset = py * OLED_DISPLAY_HEIGHT + px;
                    oled_write_raw_byte(0x00, offset);
                }
            }
        }
        
        draw_image(image, current_x, targetY, 32, 32, movement_reverse);
    }
    
    // Toggle movement at the edge of the screen
    if (runCompleted 
        && current_animation == ANIMATION_RTOGIL
        && (current_x <= 0 || current_x >=31))
    {
        toggleMovement = true;
        
        target_animation = ANIMATION_RUN;
        target_animation_runs = 0;
    }
                
    // Move the position if running or jumping
    if (current_animation == ANIMATION_RUN
        || (character == CHARACTER_NEKO && current_animation == ANIMATION_JUMP))
    {
        if (movement_reverse)
        {
            current_x -= 3;
        }
        else
        {
            current_x += 3;
        }
        
        if ((movement_reverse && current_x <= 0)
            || (!movement_reverse && current_x >= 31))
        {
            target_animation = ANIMATION_RTOGIL;
            target_animation_runs = 1;
            
            if (character == CHARACTER_SAKURA)
            {
                target_animation_runs = 3;
            }
        }
        
        if (current_x < 0)
        {
            current_x = 0;
        }
        else if (current_x > 31)
        {
            current_x = 31;
        }
    } 
    
    // Character specific state transitions
    if (character == CHARACTER_SAKURA)
    {
        if (runCompleted)
        {
            if (current_animation != ANIMATION_RUN
                && current_animation != ANIMATION_NORMAL)
            {
                target_animation = ANIMATION_RUN;
                target_animation_runs = 3;
            }                
            else            
            {
                int8_t random_action_number = rand() % 50;
                if (random_action_number > 30)
                {
                    target_animation = ANIMATION_RUN;
                    target_animation_runs = 3;
                }
                else if (random_action_number > 17)
                {
                    target_animation = ANIMATION_NORMAL;
                    target_animation_runs = 3;
                }
                else if (random_action_number > 8)
                {
                    target_animation = ANIMATION_JUMP;
                    target_animation_runs = 3;
                }
                else if (random_action_number > 5)
                {
                    target_animation = ANIMATION_UP;
                    target_animation_runs = 3;
                }
                else if (random_action_number > 2)
                {
                    target_animation = ANIMATION_DOWN;
                    target_animation_runs = 3;
                }
                else 
                {
                    target_animation = ANIMATION_AWAKE;
                    target_animation_runs = 3;
                }
            }
            
            dprintf("Skura random action @ %d\n", target_animation);
        }
    }
    else
    {
        if (runCompleted)
        {
            if (current_animation == ANIMATION_RTOGIL
                && toggleMovement)
            {
                // Do nothing when toggling movement
            }
            else if (current_animation == ANIMATION_JUMP
                || current_animation == ANIMATION_UP
                || current_animation == ANIMATION_DOWN)
            {
                target_animation = ANIMATION_RUN;
                target_animation_runs = 0;
            }
            else
            {
                target_animation = ANIMATION_NORMAL;
                target_animation_runs = 0;
            }
        }
    }
    
    if (toggleMovement)
    {
        movement_reverse = !movement_reverse;
    }
}

// Draw animation at the specified 
// y is specified as OLED cursor line position
static void render_animation(int y, int character) 
{    
    if (!is_oled_enabled && is_oled_on())
    {
        oled_off();
    }
    else if (is_oled_enabled && !is_oled_on())
    {
        oled_on();
    }
    
    if (is_oled_enabled)
    {        
        if(animation_timer == 0)
        {
            animation_timer = timer_read32();
            draw_animation_frame(y, character);
        }        
        else if(timer_elapsed32(animation_timer) > frame_duration) 
        {
            animation_timer = timer_read32();
            draw_animation_frame(y, character);
        }
    }
}

// Display layer
static void display_layer(void)
{
    // Layer information
    switch (get_highest_layer(layer_state)) {
        case _DEFAULT:
            oled_write_ln_P(PSTR("Meow"), false);
            break;
        case _LOWER:
            oled_write_ln_P(PSTR("Purr"), false);
            break;
        case _RAISE:
            oled_write_ln_P(PSTR("Gao!"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Hiss"), false);
    }
}

// Display keyboard cap / scroll / num lock status
static void display_status(void) 
{
    // LED state
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_ln_P(PSTR("CAP"), led_usb_state.caps_lock);
    oled_write_ln_P(PSTR("SCR"), led_usb_state.scroll_lock);
    oled_write_ln_P(PSTR("NUM"), led_usb_state.num_lock);

    oled_write_ln_P(PSTR(""), false);
}

// Display encoder state such as switch windows / tab / etc
static void display_encoder2_state(void)
{
    switch (encoder2_mode)
    {
        case ENCODER_WIN:
            oled_write("Win ", false);
            break;
            
        case ENCODER_TAB:
            oled_write("Tab ", false);
            break;

        case ENCODER_PAGE:
            oled_write("Page", false);
            break;
            
        case ENCODER_BOOKMARK:
            oled_write("BM  ", false);
            break;
         
         default:
            oled_write("      ", false);
            break;
    }    
}

// Display for the slave side
static void oled_display_slave_side(void)
{
    render_logo(true);
        
    oled_set_cursor(0, 5);
        
    display_status();
    
    display_layer();
    
    display_encoder2_state();
}

// OLED display
bool oled_task_user(void) 
{
    if (is_keyboard_master()) 
    {
        render_logo(false);
        
        oled_set_cursor(0, 5);

        render_animation(8, CHARACTER_NEKO);
    } 
    else 
    {
        oled_display_slave_side();
        
        render_animation(12, CHARACTER_SAKURA);
    }
    
    return false;
}

#endif