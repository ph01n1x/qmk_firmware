//Sets up what the OLED screens display.

#ifdef OLED_ENABLE

#include "images.c"

// Draw an image
// x is specified as pixels
// y is specified as OLED cursor line position. Each line has height of 8.
// Height must be of multiple of 8
static void draw_image(const char *image, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool mirror)
{
    uint8_t maxLine = height / 8;
    uint16_t offset = 0;
    
    if (mirror)
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

// Draw an image
// x is specified as pixels
// y is specified as OLED cursor line position. Each line has height of 8.
// Height must be of multiple of 8
static void clear_image(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    uint8_t maxLine = height / 8;
    uint16_t offset = 0;
    
    for (uint8_t currentLine = 0; currentLine < maxLine; currentLine++)
    {
        offset = (y + currentLine) * OLED_DISPLAY_HEIGHT + x;
        
        for (uint8_t currentColumn = 0; currentColumn < width; currentColumn++)
        {
           oled_write_raw_byte(0x00, offset + currentColumn);
        }
    }        
}

uint32_t animation_timer = 0;
uint16_t frame_duration = 400;
uint8_t current_frame = 0;

uint8_t current_animation_runs = 0;

int8_t current_x = 0;
int8_t current_y = 8;
int8_t last_x = -1;
int8_t last_y = -1;
bool movement_reverse = false;
bool flipped = false;

uint32_t last_action_timer = 0;

bool mouse_chase = false;
uint32_t mouse_chase_timer = 0;
int8_t mouse_chase_random = 0;
int8_t mouse_chase_x = 0;
int8_t mouse_chase_y = 0;

bool mouse_is_active = false;
uint32_t mouse_timer = 0;
int8_t mouse_x = 0;
int8_t mouse_y = 0;
int8_t mouse_last_x = -1;
int8_t mouse_last_y = -1;
bool mouse_movement_reverse = false;

#define CHARACTER_NEKO 0
#define CHARACTER_SAKURA 1
    
// Draw the top logo
static void render_logo(bool reverse) 
{
    draw_image(phoenix, 16, 0, 32, 32, reverse);
}

// If the mouse is active, draw the mouse running across the screen
void render_mouse(void)
{
    if (!mouse_is_active)
    {
        return;
    }

    if (mouse_x < 0 || mouse_x > 48)
    {
        if (mouse_last_x >= 0 && mouse_last_y >= 0)
        {
            clear_image(mouse_last_x, mouse_last_y, 16, 8);
        }
        
        mouse_is_active = false;
        mouse_x = 0;
        mouse_last_x = -1;
        mouse_last_y = -1;
        
        return;
    }
        
    // Draw the new content
    if (mouse_last_x >= 0 && mouse_last_y >= 0)
    {
        clear_image(mouse_last_x, mouse_last_y, 16, 8);
    }
    
    draw_image(mouse, mouse_x, mouse_y, 16, 8, mouse_movement_reverse);
    
    mouse_last_x = mouse_x;
    mouse_last_y = mouse_y;
    
    if (mouse_movement_reverse)
    {
        mouse_x -= 8;
        
        // Wake up neko 
        if (mouse_x <= 32 && !mouse_chase && timer_elapsed32(mouse_chase_timer) > 5000)
        {
            target_animation = ANIMATION_AWAKE;
            target_animation_runs = 1;
            new_animation_set = true;
            
            mouse_chase_x = 0;
            mouse_chase_y = mouse_y;
            mouse_chase = true;
            mouse_chase_timer = timer_read32();
        }
    }
    else
    {
        mouse_x += 8;
        
        // Wake up neko
        if (mouse_x >= 16 && !mouse_chase && timer_elapsed32(mouse_chase_timer) > 5000)
        {
            target_animation = ANIMATION_AWAKE;
            target_animation_runs = 1;
            new_animation_set = true;
            
            mouse_chase_x = 31;
            mouse_chase_y = mouse_y;
            mouse_chase = true;
            mouse_chase_timer = timer_read32();
        }
    }
}

// Draws one frame of the animation sequence
void render_animation_frame(int y, int character) 
{
    // Handles when a new animation is set
    if (current_animation != target_animation || new_animation_set)
    {
        // First animation after sleep will always be awake
        if (current_animation != target_animation 
            && current_animation == ANIMATION_SLEEP)
        {
            target_animation = ANIMATION_AWAKE;
            target_animation_runs = 2;
        }
        
        if (current_animation != target_animation)
        {
            current_animation = target_animation; 
            current_frame = 0;
        }
        
        current_animation_runs = 0;
        new_animation_set = false;
    }
    
    // Generate random mouse event
    if (character == CHARACTER_NEKO)
    {
        if (mouse_timer == 0)
        {
            mouse_timer = timer_read32();
        }
        else if (mouse_is_active)
        {
            mouse_timer = timer_read32();
        }
        else if (mouse_chase)
        {
            mouse_timer = timer_read32();
        }
        else if ((mouse_triggered || timer_elapsed32(mouse_timer) > 15000 + mouse_chase_random * 10000)
            && !mouse_is_active && !mouse_chase)
        {
            mouse_triggered = false;
            
            // Activate mouse on the same side of the neko
            if (current_x <= 15)
            {
                mouse_x = 0;
                mouse_movement_reverse = false;
            }
            else
            {
                mouse_x = 48;
                mouse_movement_reverse = true;
            }
            
            // Mouse can be drawn from y of line 4 to 15 
            // Exclude the neko height of 4 lines
            // Randomly place the mouse at a blank line
            int8_t random_y = rand() % 8 + 4;
            if (random_y >= current_y && random_y < current_y + 4)
            {
                random_y += 4;
            }
            
            if (random_y > 15)
            {
                random_y = 15;
            }
            
            mouse_y = random_y;
            
            mouse_is_active = true;
            mouse_chase_random = rand() % 10;
            
            dprintf("Mouse activated @ %d,%d with neko @ %d,%d\n", mouse_x, mouse_y, current_x, current_y);
        }
    }
    
    // Neko animation switch eventually to sleep
    uint32_t neko_activity_timer = keyevent_timer;
    if (mouse_timer > neko_activity_timer)
    {
        neko_activity_timer = mouse_timer;
    }
    
    if (character == CHARACTER_NEKO
        && current_animation != ANIMATION_SLEEP 
        && timer_elapsed32(neko_activity_timer) > 10000
        && !mouse_chase)
    {
        if (timer_elapsed32(neko_activity_timer) > 26000)
        {
            if (current_animation != ANIMATION_SLEEP)
            {
                current_frame = 0;
            }
            current_animation = ANIMATION_SLEEP;
            target_animation = current_animation;
            target_animation_runs = 0;
        }
        else if (timer_elapsed32(neko_activity_timer) > 22000)
        {
            if (current_animation != ANIMATION_YAWN)
            {
                current_frame = 0;
            }
            current_animation = ANIMATION_YAWN;
            target_animation = current_animation;
            target_animation_runs = 0;
        }   
        else if (timer_elapsed32(neko_activity_timer) > 18000)
        {
            if (current_animation != ANIMATION_KAKI)
            {
                current_frame = 0;
            }
            current_animation = ANIMATION_KAKI;
            target_animation = current_animation;
            target_animation_runs = 0;
        }              
        else if (timer_elapsed32(neko_activity_timer) > 10000)
        {
            if (current_animation != ANIMATION_NORMAL)
            {
                current_frame = 0;
            }
            current_animation = ANIMATION_NORMAL;
            target_animation = current_animation;
            target_animation_runs = 0;
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
          
        case ANIMATION_UPRIGHT:
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_up[frameIndex];
            }
            else
            {
                image = neko_upright[frameIndex];
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
            
        case ANIMATION_DOWNRIGHT:
            current_frame = (current_frame + 1) % FRAMES;
            frameIndex = abs((FRAMES - 1) - current_frame);
            image = neko_down[frameIndex];
            if (character == CHARACTER_SAKURA)
            {
                image = sakura_down[frameIndex];
            }
            else
            {
                image = neko_downright[frameIndex];
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
                    targetY = y - 1;
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
        clear_image(last_x, last_y, 32, 32);
        
        // Draw the new content
        bool mirror = movement_reverse;
        if (flipped)
        {
            mirror = !mirror;
        }
        
        draw_image(image, current_x, targetY, 32, 32, mirror);
        
        last_x = current_x;
        last_y = targetY;
    }
    
    // Toggle movement at the edge of the screen
    if (runCompleted
        && !mouse_chase    
        && current_animation == ANIMATION_RTOGIL
        && (current_x <= 0 || current_x >=31))
    {
        toggleMovement = true;
        
        target_animation = ANIMATION_RUN;
        target_animation_runs = 2;
        new_animation_set = true;
    }
             
    // Move the position if running or jumping
    if (current_animation == ANIMATION_RUN
        || (character == CHARACTER_NEKO && current_animation == ANIMATION_JUMP)
        || (character == CHARACTER_NEKO && current_animation == ANIMATION_UPRIGHT)
        || (character == CHARACTER_NEKO && current_animation == ANIMATION_DOWNRIGHT))
    {
        if (movement_reverse)
        {
            current_x -= 3;
        }
        else
        {
            current_x += 3;
        }
        
        if (((movement_reverse && current_x <= 0)
            || (!movement_reverse && current_x >= 31))
            && !mouse_chase)
        {
            target_animation = ANIMATION_RTOGIL;
            target_animation_runs = 1;
            
            if (character == CHARACTER_NEKO && mouse_chase)
            {
                target_animation_runs = 3;
            }
            else if (character == CHARACTER_SAKURA)
            {
                target_animation_runs = 3;
            }
            
            new_animation_set = true;
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
    
    // Character specific state transitions
    if (character == CHARACTER_SAKURA)
    {
        if (runCompleted)
        {
            if (current_animation == ANIMATION_RTOGIL && toggleMovement)
            {
                // Do nothing
            }
            else if (current_animation == ANIMATION_NORMAL)
            {
                target_animation = ANIMATION_RTOGIL;
                target_animation_runs = 2;
                flipped = false;
                new_animation_set = true;
            }
            else if (current_animation == ANIMATION_RTOGIL)
            {
                if (flipped)
                {
                    flipped = false;
                    
                    target_animation = ANIMATION_NORMAL;
                    target_animation_runs = 3;
                    new_animation_set = true;
                }
                else
                {
                    flipped = true;
                    
                    target_animation = ANIMATION_RTOGIL;
                    target_animation_runs = 2;
                    new_animation_set = true;
                }
            }
            else
            {
                target_animation = ANIMATION_NORMAL;
                target_animation_runs = 3;
                flipped = false;
                new_animation_set = true;
                
                last_action_timer = timer_read32();
            }
        
            if (current_animation == ANIMATION_NORMAL)
            {
                if (last_action_timer == 0)
                {
                    last_action_timer = timer_read32();
                }
                else if (timer_elapsed32(last_action_timer) > 10000)
                {
                    int8_t random_action_number = rand() % 55;
                    
                    if (random_action_number > 35)
                    {
                        target_animation = ANIMATION_RUN;
                        target_animation_runs = 2;
                        new_animation_set = true;
                    }
                    else if (random_action_number > 15)
                    {
                        target_animation = ANIMATION_JUMP;
                        target_animation_runs = 3;
                        new_animation_set = true;
                    }
                    else if (random_action_number > 10)
                    {
                        target_animation = ANIMATION_UP;
                        target_animation_runs = 3;
                        new_animation_set = true;
                    }
                    else if (random_action_number > 5)
                    {
                        target_animation = ANIMATION_DOWN;
                        target_animation_runs = 3;
                        new_animation_set = true;
                    }  
                    else
                    {
                        target_animation = ANIMATION_AWAKE;
                        target_animation_runs = 2;
                        new_animation_set = true;
                    }

                    last_action_timer = timer_read32();                    
                }
            }
         
            dprintf("Skura random action %d [%c|%c] @ %d\n", target_animation, movement_reverse ? 'L' : 'R', flipped ? 'F' : ' ', current_x);
        }
        
        if (target_animation_runs <= 1)
        {
            target_animation_runs = 1;
        }
    }
    else if (character == CHARACTER_NEKO)
    {
        // Handle vertical movements
        if (runCompleted)
        {
            if (current_animation == ANIMATION_DOWN || current_animation == ANIMATION_DOWNRIGHT)
            {
                current_y = current_y + 1;
                if (current_y > 12)
                {
                    current_y = 12;
                }
            }
            else if (current_animation == ANIMATION_UP || current_animation == ANIMATION_UPRIGHT)
            {
                current_y = current_y - 1;
                if (current_y < 5)
                {
                    current_y = 5;
                }
            }
        }
            
        // Handle neko's mouse chase mode
        if (mouse_chase)
        {      
            uint8_t target_chase_y = mouse_chase_y - 3;
            if (target_chase_y < 5)
            {
                target_chase_y = 5;
            }
            
            if (target_chase_y > 12)
            {
                target_chase_y = 12;
            }
            
            if (runCompleted)
            {
                // Sometimes, neko decides not to chase the mouse after seeing it
                if (current_animation == ANIMATION_AWAKE
                    && rand() % 10 >= 7)
                {
                    target_animation = ANIMATION_SLEEP;
                    target_animation_runs = 0;
                    new_animation_set = true;
                    
                    mouse_chase = false;
                }
                else
                {
                    if (current_y < target_chase_y)
                    {
                        target_animation = ANIMATION_DOWN;
                        target_animation_runs = 1;
                        new_animation_set = true;
                        
                        if (current_x < mouse_chase_x)
                        {
                            target_animation = ANIMATION_DOWNRIGHT;
                            new_animation_set = true;
                            
                            if (movement_reverse)
                            {
                                toggleMovement = true;
                            }
                        }
                        else if (current_x > mouse_chase_x)
                        {
                            target_animation = ANIMATION_DOWNRIGHT;
                            
                            if (!movement_reverse)
                            {
                                toggleMovement = true;
                            }
                        }
                    }
                    else if (current_y > target_chase_y)
                    {
                        target_animation = ANIMATION_UP;
                        target_animation_runs = 1;
                        new_animation_set = true;
                        
                        if (current_x < mouse_chase_x)
                        {
                            target_animation = ANIMATION_UPRIGHT;
                            
                            if (movement_reverse)
                            {
                                toggleMovement = true;
                            }
                        }
                        else if (current_x > mouse_chase_x)
                        {
                            target_animation = ANIMATION_UPRIGHT;
                            
                            if (!movement_reverse)
                            {
                                toggleMovement = true;
                            }
                        }
                    }
                    else
                    {
                        if (current_x < mouse_chase_x)
                        {
                            target_animation = ANIMATION_RUN;
                            target_animation_runs = 1;
                            new_animation_set = true;
                            
                            if (movement_reverse)
                            {
                                toggleMovement = true;
                            }
                        }
                        else if (current_x > mouse_chase_x)
                        {
                            target_animation = ANIMATION_RUN;
                            target_animation_runs = 1;
                            new_animation_set = true;
                            
                            if (!movement_reverse)
                            {
                                toggleMovement = true;
                            }
                        }
                        else
                        {
                            if (current_animation != ANIMATION_RTOGIL)
                            {
                                target_animation = ANIMATION_RTOGIL;
                                target_animation_runs = 3;
                            }
                            else
                            {
                                mouse_chase = false;
                                target_animation = ANIMATION_KAKI;
                                target_animation_runs = 3;
                                new_animation_set = true;      
                                
                                // Set the correct movement direction once the chase is completed
                                if ((mouse_chase_x == 0 && movement_reverse)
                                    || (mouse_chase_x > 0 && !movement_reverse))
                                {
                                    toggleMovement = true;
                                }       
                            }
                        }
                    }
                }
                
                if (mouse_chase)
                {
                    dprintf("Mouse chase %d,%d -> %d,%d (Action %d => %d)\n", current_x, current_y, mouse_chase_x, target_chase_y, current_animation, target_animation);
                }
                else
                {
                    dprintf("Mouse chase done @ %d,%d (Action %d => %d)\n", current_x, current_y, current_animation, target_animation);
                }
            }
            
            if (current_animation != ANIMATION_AWAKE
                && current_animation != ANIMATION_UP
                && current_animation != ANIMATION_UPRIGHT
                && current_animation != ANIMATION_DOWN
                && current_animation != ANIMATION_DOWNRIGHT
                && current_animation != ANIMATION_RUN
                && current_animation != ANIMATION_RTOGIL
                && current_animation != ANIMATION_KAKI)
            {
                target_animation = ANIMATION_RUN;
                target_animation_runs = 1;
                new_animation_set = true;
            }
        }
        else if (runCompleted)
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
                new_animation_set = true;
            }
            else
            {
                target_animation = ANIMATION_NORMAL;
                target_animation_runs = 0;
                new_animation_set = true;
            }
        }
    }
    
    if (toggleMovement)
    {
        movement_reverse = !movement_reverse;
    }
    
    // Draw mouse for neko
    if (character == CHARACTER_NEKO)
    {
        render_mouse();
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
            render_animation_frame(y, character);
        }        
        else if(timer_elapsed32(animation_timer) > frame_duration) 
        {
            animation_timer = timer_read32();
            render_animation_frame(y, character);
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

// OLED display
bool oled_task_user(void) 
{
    if (is_keyboard_master()) 
    {
        render_logo(false);
        
        render_animation(current_y, CHARACTER_NEKO);
    } 
    else 
    {
        oled_display_slave_side();
        
        render_animation(12, CHARACTER_SAKURA);
    }
    
    return false;
}

#endif
