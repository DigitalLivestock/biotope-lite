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

#include "ntc.h"

/* Convert ADC reading to resistance: */
static float ntc_get_resistance(const int adc_value)
{
    float voltage = adc_value * (5.0 / 1023.0);
    float resistance = (series_resistor * (5.0 - voltage)) / voltage;

    return resistance;
}

/* Calculate temperature using Steinhart-Hart equation: */
static float ntc_calculate_temperature(float resistance)
{
    float log_r = log(resistance / 1000.0);  // Convert ohm to kohm
    float temperature_c = A + B * log_r + C * pow(log_r, 2);

    return temperature_c;
}

/* User callable: Set used pin: */
void ntc_init(const uint8_t pin)
{
  ntc_analog_pin = pin;
  pinMode(ntc_analog_pin, INPUT);
}

/* User callable: read temperature in Celsius */
float ntc_get_temperature_c(void)
{
  int adc_value = analogRead(ntc_analog_pin);
  float resistance = ntc_get_resistance(adc_value);
  float temperature_c = ntc_calculate_temperature(resistance);
  
  return temperature_c;
}