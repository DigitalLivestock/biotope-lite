/*****************************************************************************
 * File: config.h
 * Author: DigitalLivesock
 * Date: 2026-02-07
 * Description: Firmware configuration.
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

/* DEBUG: Enable or disable */
#define SERIAL_PRINT_DEBUG true

#define SRC_VERSION "Version: 1.2"

/* Connected pins: */
#define BUTTON_INT_PIN 2
#define LCD_ACTIVE_PIN 9

/* EEPROM save addresses */
#define MAX_TEMP_IN_ADDR  (float*)28
#define MIN_TEMP_IN_ADDR  (float*)24
#define MAX_TEMP_OUT_ADDR (float*)20
#define MIN_TEMP_OUT_ADDR (float*)16
#define MAX_PRES_ADDR     (float*)12
#define MIN_PRES_ADDR     (float*)8
#define MAX_HUM_ADDR      (float*)4
#define MIN_HUM_ADDR      (float*)32

#define LCD_IDLE_TIMER_S 5
#define SAMPLING_PERIOD_S 60

#define NTC_PIN A0