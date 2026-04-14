/*****************************************************************************
 * File: lcd.h
 * Author: DigitalLivesock
 * Date: 2026-02-07
 * Description: LCD methods and definitions.
/*****************************************************************************
 * Copyright (C) 2026 DigitalLivestock
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "twi.h"
#include "MCP23016.h"

/* LCD JHD16xA commands: */
#define IOEADDR       0x20
#define FUNCSET       0b00111100
#define LCDCONF       0b00001111
#define ENTRYMODE     0b00000110
#define LCDCLR        0b00000001
#define CURSORSET     0b11000011
#define CURSORON      0b00000010
#define CURSORBLINKON 0b00000001
#define SHIFT         0b00010000

#define DISPLAYSHIFT  0b00001000
#define RIGHTSHIFT    0b00000100

#define L 0
#define H 1

/* Configuration file: */
#define LCD_RS_PIN      10
#define LCD_RW_PIN      11
#define LCD_ENABLE_PIN  12

static uint8_t display_switch = 0;

/* LCD related functions: */
void lcd_enable();
void lcd_cmd(byte addr, byte reg, byte cmd, bool rs);
void put_char_at(char c, int col);
void cursor_set(uint8_t col);
void put_char(char c);
void put_string_at(char *s, uint8_t len, uint8_t line, uint8_t col);
void lcd_init();
void lcd_clear();

#endif