/*****************************************************************************
 * Author: DigitalLivesock
 * Date: 2026-02-07
 * Description:
 ****************************************************************************/

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

#ifndef BIOTOPE_APP_H
#define BIOTOPE_APP_H

#include "config.h"

#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <Wire.h>
#include <avr/eeprom.h>

#include "bme280.h"
#include "twi.h"
#include "lcd.h"
#include "ntc.h"
#include "queue.h"

#if SERIAL_PRINT_DEBUG 
  #define PRINT(s) Serial.print(s)
  #define PRINTLN(s) Serial.println(s)
#else
  #define PRINT(s)
  #define PRINTLN(s)
#endif

typedef struct state_
{
  char *line0;
  char *line1;

  state_ *prev_frame;
  state_ *next_frame;

  state_ *up;
  state_ *down;
  void (*enter)(void);

} state_t;

enum button_ev_t {DOWN, RIGHT, ENTER, UP, LEFT};

enum event_t {TEMP, PRES, HUM, BUTTON, NONE};

void biotope_app_main(void);
void biotope_app_init(void);

void frame_update_temperature(const float temp_in, const float temp_out);
void frame_update_humidity(const float hum);
void frame_update_pressure(const float pres);

void state_init();
void put_curr_frame();
void check_button();
void sample_temp();
void lcd_idle_mode();
void clear_int_reg();
void cursor_off();
void wdt_init();

void store_data_to_eeprom();
void restore_data_from_eeprom();
void reset_data_on_eeprom();
void NOP(void);

#endif