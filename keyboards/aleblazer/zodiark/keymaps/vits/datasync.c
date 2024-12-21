typedef struct _master_to_slave_t 
{
    uint8_t encoder2_mode;
    bool is_oled_enabled;
} master_to_slave_t;

// Handles when master send data to slave
void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) 
{
    const master_to_slave_t *m2s = (const master_to_slave_t*)in_data;
    encoder2_mode = m2s->encoder2_mode;
    is_oled_enabled = m2s->is_oled_enabled;
}

uint32_t datasync_timer = 0;

void user_sync_a_master_to_slave(void) 
{
    if (!is_keyboard_master()) 
    {
        return;
    }
    
    // Synchronize data to slave
    if (timer_elapsed32(datasync_timer) > 500) 
    {
        master_to_slave_t m2s = { encoder2_mode, is_oled_enabled };
        
        if (!transaction_rpc_send(USER_SYNC_A, sizeof(m2s), &m2s))
        {
             dprint("Slave sync failed!\n");
        }
        
        datasync_timer = timer_read32();
    }       
}