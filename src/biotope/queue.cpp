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

#include "queue.h"

queue_t queue = {0, 0, 0, 0, 0};

void queue_init(uint8_t *buf, uint8_t size)
{
  queue.buf = buf;
  queue.size = size;
}

void queue_put(uint8_t unit)
{
  if (queue.units < queue.size) {
    queue.buf[++queue.last % queue.size] = unit;
    queue.last %= queue.size;
    queue.units++;
  }
}

uint8_t queue_get(void)
{
  if (queue.units > 0) {
    uint8_t index = queue.first;
    queue.first = (queue.first + 1) % queue.size;
    queue.units--;
    return queue.buf[index];
  }

  return 255;
}