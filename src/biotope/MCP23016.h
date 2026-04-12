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
 *****************************************************************************/

#ifndef MCP23016_H
#define MCP23016_H

/* MCP23017 registers: BANK 1 */

/* Port A */
#define IODIRA    0x00
#define IPOLA     0x01
#define GPINTENA  0x02
#define DEFVALA   0x03
#define INTCONA   0x04
#define IOCON     0x05
#define GPPUA     0x06
#define INTFA     0x07
#define INTCAPA   0x08
#define GPIOA     0x09
#define OLATA     0x0A

/* Port B */
#define IODIRB    0x10
#define IPOLB     0x11
#define GPINTENB  0x12
#define DEFVALB   0x13
#define INTCONB   0x14
//#define IOCON     0x15
#define GPPUB     0x16
#define INTFB     0x17
#define INTCAPB   0x18
#define GPIOB     0x19
#define OLATB     0x1A

#endif