typedef struct _master_to_slave_t 
{
    uint8_t encoder2_mode;
} master_to_slave_t;

// Handles when master send data to slave
void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) 
{
    const master_to_slave_t *m2s = (const master_to_slave_t*)in_data;
    encoder2_mode = m2s->encoder2_mode;
}

uint32_t datasync_timer = 0;

void user_sync_a_master_to_slave(void) 
{
    if (!is_keyboard_master()) 
    {
        return;
    }
    
    // Synchronize data to slave
    if (timer_elapsed32(datasync_timer) > 1000) 
    {
        master_to_slave_t m2s = { encoder2_mode };
        
        if(transaction_rpc_send(USER_SYNC_A, sizeof(m2s), &m2s))
        {
            datasync_timer = timer_read32();
        } 
        else 
        {
            dprint("Slave sync failed!\n");
        }
    }       
}