/*  Description:
    A simple software interface for the WBUART32 core
    developed by Dan Gisselquist, Ph.D.
    This module provides the interrupt service routines
    
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

// interrupts in an object-oriented manner
static rx_data_t rx_data = {.wr_ptr = 0, .rd_ptr = 0};


// interrupt if a byte is received
void uart_rx_int()
{   
    // read a single byte from RX FIFO
    uint8_t volatile * const p_reg = (uint8_t *)(AXILUART_BASE_ADDRESS + REG_RX_DATA_ADDR);
    reg_rxdata_t reg;
    reg.value = *p_reg;
    // save the byte in the ringbuffer and increase write pointer
    rx_data.rxbytes[rx_data.wr_ptr] = reg.field.rword;
    rx_data.wr_ptr++;
    if (rx_data.wr_ptr == MAX_DATA_SIZE) {
        rx_data.wr_ptr = 0;
    }
}

// interrupt if a byte is transmitted
void uart_tx_int()
{
}

// interrupt if the RX FIFO is half full
void uart_rxfifo_int()
{
    uint16_t i = 0;
    // read number of bytes in FIFO
    uint16_t num_bytes = axiluart_get_rxlevel(AXILUART_BASE_ADDRESS);
    for (i = 0; i < num_bytes; i++) {
        uart_rx_int();
    }
}

// interrupt if the TX FIFO is half empty
void uart_txfifo_int()
{
}

// return a pointer to the RX DATA
rx_data_t* get_rx_data()
{
    return &rx_data;
}