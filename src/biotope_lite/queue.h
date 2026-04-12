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

#include <stdint.h>

typedef struct {
  uint8_t *buf;
  uint8_t size;
  uint8_t units;
  uint8_t first;
  uint8_t last;
} queue_t;

extern queue_t queue;

void queue_init(uint8_t *buf, uint8_t size);
void queue_put(uint8_t unit);
uint8_t queue_get(void);