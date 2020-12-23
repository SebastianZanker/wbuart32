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

#ifndef AXILUART_H
#define AXILUART_H

#include "stdint.h"

// AXI base address for the core => change to your needs
#define AXILUART_BASE_ADDRESS   0x00010000

// Register addresses
#define REG_SETUP_ADDR          0x00
#define REG_FIFO_ADDR           0x04
#define REG_RX_DATA_ADDR        0x08
#define REG_TX_DATA_ADDR        0x0C

// Parity
typedef enum {ODD, EVEN, SPACE, MARK, NO_PARITY} parity_e;

// typedefs for the 4 registers

// setup register
typedef union {
	uint32_t value;
	struct {
		uint32_t baud_clks   : 24;
		uint32_t T           : 1;
		uint32_t F           : 1;
		uint32_t P           : 1;
		uint32_t S           : 1;
		uint32_t N           : 2;
		uint32_t H           : 1;
		uint32_t unused      : 1;
	} field;
} reg_setup_t;


// FIFO register
typedef union {
    uint32_t value;
    struct {
        uint32_t rx_z        : 1;
        uint32_t rx_h        : 1;
        uint32_t rx_fill     : 12;
        uint32_t tx_z        : 1;
        uint32_t tx_h        : 1;
        uint32_t tx_fill     : 12;
        uint32_t lgln        : 4;
    } field;
} reg_fifo_t;


// RXDATA register
typedef union {
    uint32_t value;
    struct {
        uint32_t rword       : 8;
        uint32_t S           : 1;
        uint32_t P           : 1;
        uint32_t F           : 1;
        uint32_t B           : 1;
        uint32_t E           : 1;
        uint32_t unused      : 19;
    } field;
} reg_rxdata_t;


// TXDATA register
typedef union {
    uint32_t value;
    struct {
        uint32_t tword       : 8;
        uint32_t S           : 1;
        uint32_t B           : 1;
        uint32_t O           : 1;
        uint32_t C           : 1;
        uint32_t E           : 1;
        uint32_t Z           : 1;
        uint32_t H           : 1;
        uint32_t R           : 1;
        uint32_t unused      : 16;
    } field;
} reg_txdata_t;


// functions (all functions with base address in case there is more than one UART core
void axiluart_set_baudrate(uint32_t base_address, uint32_t divider);
void axiluart_tx_str_blocking(uint32_t base_address, char *str);
void axiluart_tx_byte(uint32_t base_address, char str);
uint16_t axiluart_get_rxlevel(uint32_t base_address);
uint16_t axiluart_get_txlevel(uint32_t base_address);
void axiluart_read_rxfifo(uint32_t base_address, char* str, uint16_t num);
void axiluart_set_parity(uint32_t base_address, parity_e par);

#endif
