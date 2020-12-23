// Author: Sebastian Zanker

#include "axiluart.h"

void axiluart_set_baudrate(uint32_t base_address, uint32_t divider)
{
    reg_setup_t reg;
    // read-modify-write
    uint32_t volatile * const p_reg = (uint32_t *)(base_address + REG_SETUP_ADDR);
    reg.value = *p_reg;
    reg.field.baud_clks = divider;
    *p_reg = reg.value;
}

// send a whole string without interrupts (blocking)
void axiluart_tx_str_blocking(uint32_t base_address, char *str)
{
    uint32_t volatile * const p_reg = (uint32_t *)(base_address + REG_FIFO_ADDR);
    reg_fifo_t reg;
    while (*str != '\0') {
        // check if there is space for one more byte
        while (1) {
            reg.value = *p_reg;
            if (reg.field.tx_z == 1) {
                break;
            }
        }
        axiluart_tx_byte_blocking(base_address, *str);
        str++;
    }
}

// transmit a single byte
void axiluart_tx_byte_blocking(uint32_t base_address, char c)
{
    uint8_t volatile * const p_reg = (uint8_t *)(base_address + REG_TX_DATA_ADDR);
    *p_reg = c;
}

// get RX FIFO fill level
uint16_t axiluart_get_rxlevel(uint32_t base_address)
{
    uint8_t volatile * const p_reg = (uint8_t *)(base_address + REG_FIFO_ADDR);
    reg_fifo_t reg;
    reg.value = *p_reg;
    return (uint16_t)reg.field.rx_fill;
}