/*****************************************************************************
 * File: twi.h
 * Author: DigitalLivesock
 * Date: 2026-02-07
 * Description: Two Wire Interface abstraction related definitions and 
 *              methods.
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


#ifndef TWI_H
#define TWI_H

#include <Wire.h>
#include <stdint.h>
#include <Arduino.h>

/* TWI related functions: */
void twi_init();
void twi_write(byte addr, byte reg, byte cmd);
uint8_t twi_read(int addr, int reg);

#endif