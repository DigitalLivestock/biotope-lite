/*****************************************************************************
 * File: lcd.cpp
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
 ******************************************************************************/

#include "lcd.h"

void lcd_enable()
{
  digitalWrite(LCD_ENABLE_PIN, LOW);
  delay(4);
  digitalWrite(LCD_ENABLE_PIN, HIGH);
  delay(4);
  digitalWrite(LCD_ENABLE_PIN, LOW);
  delay(4);
}

void lcd_cmd(byte addr, byte reg, byte cmd, bool rs)
{
  digitalWrite(LCD_RS_PIN, rs);
  twi_write(addr, reg, cmd);
  lcd_enable();
}

void put_char_at(char c, int col)
{
  cursor_set(col);
  put_char(c);
}

void cursor_set(uint8_t line, uint8_t col)
{
  if (line > 0) {
    lcd_cmd(IOEADDR, GPIOA, 0b11000000 | col, LOW);
  } else {
    lcd_cmd(IOEADDR, GPIOA, 0b10000000 | col, LOW);
  }
}

void put_char(char c)
{
  lcd_cmd(IOEADDR, GPIOA, c, HIGH);
}

void put_string_at(char *s, uint8_t len, uint8_t line, uint8_t col)
{
  cursor_set(line, col);
  for (uint8_t i = 0; i < len; i++) {
    put_char(s[i]);
  }
}

void lcd_init()
{
  delay(400); // Wait for LCD to start

  digitalWrite(LCD_RS_PIN, LOW);
  digitalWrite(LCD_RW_PIN, LOW);

  delay(40);

  /* FUNCSET x3: */
  lcd_cmd(IOEADDR, GPIOA, FUNCSET, LOW);
  lcd_enable();
  delay(5);
  lcd_enable();

  delay(40);

  lcd_cmd(IOEADDR, GPIOA, FUNCSET | 0b00001000, LOW);

  /* Configure LCD rules: */
  display_switch |= LCDCONF;
  lcd_cmd(IOEADDR, GPIOA, display_switch, LOW);
  lcd_cmd(IOEADDR, GPIOA, ENTRYMODE, LOW);

  /* Clear the screen: */
  lcd_cmd(IOEADDR, GPIOA, LCDCLR, LOW);

  /* Set cursor on line 0 column 0: */
  lcd_cmd(IOEADDR, GPIOA, 0b10000000, LOW);
}

/* Clear LCD screen: */
void lcd_clear() { lcd_cmd(IOEADDR, GPIOA, LCDCLR, L); }