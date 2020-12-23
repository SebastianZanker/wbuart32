/*  Description:
    A simple software interface for the WBUART32 core
    developed by Dan Gisselquist, Ph.D.
    
    Copyright (C) 2020  Sebastian Zanker

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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

// parity settings
void axiluart_set_parity(uint32_t base_address, parity_e par)
{
    reg_setup_t reg;
    // read-modify-write
    uint32_t volatile * const p_reg = (uint32_t *)(base_address + REG_SETUP_ADDR);
    reg.value = *p_reg;
    switch(par) {
        case ODD:
            reg.field.P = 1;
            reg.field.F = 0;
            reg.field.T = 0;
            break;
        case EVEN:
            reg.field.P = 1;
            reg.field.F = 0;
            reg.field.T = 1;
            break;
        case SPACE:
            reg.field.P = 1;
            reg.field.F = 1;
            reg.field.T = 0;
            break;
        case MARK:
            reg.field.P = 1;
            reg.field.F = 1;
            reg.field.T = 1;
            break;
        case NO_PARITY:
            reg.field.P = 0;
            reg.field.F = 0;
            reg.field.T = 0;
            break;
        default:;
    }
    // save the register
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

// read the RX FIFO
void axiluart_read_rxfifo(uint32_t base_address, char* str, uint16_t num)
{
    uint16_t i = 0;
    uint8_t volatile * const p_reg = (uint8_t *)(base_address + REG_RX_DATA_ADDR);
    reg_rxdata_t reg;
    for (i = 0; i < num; i++) {
        reg.value = *p_reg;
        str[i] = reg.field.rword;
    }
    return;
}