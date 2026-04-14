/*****************************************************************************
 * File: biotope_lite_app.cpp
 * Author: DigitalLivesock
 * Date: 2026-02-07
 * Description: Biotope Lite MAIN
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

#include "biotope_lite_app.h"

state_t *curr_state;

state_t f1, f2, f2_1, f2_2, f3, f3_1, f3_2, f4, f4_1, f4_2;

volatile uint8_t button_pressed = false;

/* Counters acting as timers for sampling and lcd idle: */
volatile uint32_t lcd_idle_timer_sec = 0;
volatile uint32_t sampling_timer_sec = 0;

/* Environmental measurements: */
/* TODO: NEED TO BE ABLE TO TRACK WHERE CURR ADDRESS SPACE IS BETWEEN POWER ON/OFF - a bit more complex than anticipated!) */
volatile float max_temp_in = 0.00;
volatile float min_temp_in = 99.00;

volatile float max_temp_out = 0.00;
volatile float min_temp_out = 99.00;

volatile float max_pres = 0.00;
volatile float min_pres = 9999.00;

volatile float max_hum = 0.00;
volatile float min_hum = 100.00;

char frame_line_text[20][17] = {
  "BIOTOPE(LITE)   ",
  "Rev: 1.2        ",

  "TEMP IN: 00.00 C",
  "TEMP OU: 00.00 C",

  "TEMP HI: 00.00 C",
  "TEMP HO: 00.00 C",

  "TEMP LI: 00.00 C",
  "TEMP LO: 00.00 C",

  "HUMI:          %",
  "PRES:        hPa",

  "HUMI H:        %",
  "HUMI L:        %",

  "PRE H:       hPa",
  "PRE L:       hPa",

  "SETTINGS        ",
  "                ",

  "BACKUP?         ",
  "                ",

  "RESET?          ",
  "                "
};

/* Setup event queue: */
uint8_t queue_buf[50];

void biotope_app_init(void)
{
  #if SERIAL_PRINT_DEBUG
    Serial.begin(9600);
  #endif

  queue_init(queue_buf, 20);

  /* Fetch history from biotope_eeprom: */
  restore_data_from_eeprom();

  /* Set transition paths for all states: */
  state_init();
  
  /* Initialize TWI and wait: */
  twi_init();
  delay(50);

  /* Configure LCD LED pin: */
  pinMode(LCD_ACTIVE_PIN, OUTPUT);

  /* Configure interrupt pin for buttons: */
  pinMode(BUTTON_INT_PIN, INPUT_PULLUP);

  /* Configure LCD control pins: */
  pinMode(LCD_ENABLE_PIN, OUTPUT);
  pinMode(LCD_RS_PIN, OUTPUT);
  pinMode(LCD_RW_PIN, OUTPUT);

  digitalWrite(LCD_ENABLE_PIN, LOW);
  digitalWrite(LCD_RS_PIN, LOW);
  digitalWrite(LCD_RW_PIN, LOW);

  /* Enable BANK = 1 on IOE: */
  twi_write(IOEADDR, 0x0A, 0b10000000);

  /* Set Port A to output on IOE: */
  twi_write(IOEADDR, IODIRA, 0x00);

  /* Set Port B to out/input on IOE: */
  twi_write(IOEADDR, IODIRB, 0b00011111);

  /* Configure pull-up on IOE B port: */
  twi_write(IOEADDR, GPPUB, 0b00011111);
  
  /* Configure interrupt on IOE B port: */
  twi_write(IOEADDR, GPINTENB, 0b00011111);
  twi_write(IOEADDR, DEFVALB, 0b00011111);
  twi_write(IOEADDR, INTCONB, 0b00011111);

  /* Setup LCD and defaults: */
  lcd_init();
  cursor_off();
  digitalWrite(LCD_ACTIVE_PIN, HIGH);
  curr_state = &f1;
  put_curr_frame();

  /* Initialize BME280 sensor over: TWI */
  bme_init();

  /* Initialize NTC sensor: ADC */
  ntc_init(NTC_PIN);

  /* Setup Watchdog as sampling timer: */
  wdt_init();
  
  /* Configure button interrupts: */
  attachInterrupt(digitalPinToInterrupt(BUTTON_INT_PIN), check_button, FALLING);

  /* Configure sleep mode: power-down */
  SMCR |= (0<<SM2) | (1<<SM1) | (0<<SM0);
  
  /* Clear button interrupts in IO-expander: */
  clear_int_reg();
  
  /* Enable global interrupts: */
  sei();

  /* Do initial sampling of all: */
  queue_put(TEMP);
  queue_put(HUM);
  queue_put(PRES);

  PRINTLN(SRC_VERSION);
  PRINTLN("SETUP COMPLETE");
}

void frame_update_temperature(const float temp_in, const float temp_out)
{
  String curr_temp_in_str = String(temp_in);
  String max_temp_in_str = String(max_temp_in);
  String min_temp_in_str = String(min_temp_in);

  String curr_temp_out_str = String(temp_out);
  String max_temp_out_str = String(max_temp_out);
  String min_temp_out_str = String(min_temp_out);

  /* Clear old data between description and unit: */
  for (int i = 0; i < 6; i++)
  {
    f2.line0[i+9] = ' ';
    f2_1.line0[i+8] = ' ';
    f2_2.line0[i+8] = ' ';

    f2.line1[i+9] = ' ';
    f2_1.line1[i+8] = ' ';
    f2_2.line1[i+8] = ' ';
  }

  for (int i = 0; i < curr_temp_in_str.length(); i++) {
    f2.line0[i+9] = curr_temp_in_str[i];
    f2_1.line0[i+9] = max_temp_in_str[i];
    f2_2.line0[i+9] = min_temp_in_str[i];
  }

  for (int i = 0; i < curr_temp_out_str.length(); i++) {
    f2.line1[i+9] = curr_temp_out_str[i];
    f2_1.line1[i+9] = max_temp_out_str[i];
    f2_2.line1[i+9] = min_temp_out_str[i];
  }
  
  if (curr_state == &f2) {
    put_string_at(&curr_temp_in_str[0], curr_temp_in_str.length(), 0, 9);
    put_string_at(&curr_temp_out_str[0], curr_temp_out_str.length(), 1, 9);
  }
}

void frame_update_humidity(const float hum)
{
  String curr_hum_str = String(hum);
  String max_hum_str = String(max_hum);
  String min_hum_str = String(min_hum);

  /* Clear old data between description and unit: */
  for (int i = 0; i < 6; i++)
  {
    f3.line1[i+6] = ' ';
    f3_1.line0[i+8] = ' ';
    f3_1.line1[i+8] = ' ';
  }

  for (int i = 0; i < curr_hum_str.length(); i++) {
    f3.line0[i+6] = curr_hum_str[i];
    f3_1.line0[i+8] = max_hum_str[i];
    f3_1.line1[i+8] = min_hum_str[i];
  }

  if (curr_state == &f3)
    put_string_at(&curr_hum_str[0], curr_hum_str.length(), 0, 6);
}

void frame_update_pressure(const float pres)
{
  String curr_pres_str = String((int)pres);
  String max_pres_str = String((int)max_pres);
  String min_pres_str = String((int)min_pres); //TODO: does not look required...

  /* Clear old data between description and unit: */
  for (int i = 0; i < 4; i++)
  {
    f3.line1[i+6] = ' ';
    f3_2.line0[i+7] = ' ';
    f3_2.line1[i+7] = ' ';
  }

  for (int i = 0; i < curr_pres_str.length(); i++) {
    f3.line1[i+6] = curr_pres_str[i];
    f3_2.line0[i+7] = max_pres_str[i];
    f3_2.line1[i+7] = min_pres_str[i];
  }

  if (curr_state == &f3)
    put_string_at(&curr_pres_str[0], curr_pres_str.length(), 1, 6);
}

void biotope_app_main(void)
{
  float curr_temp_in, curr_temp_out, curr_hum, curr_pres;

  /* Handle queued events: */
  if (queue.units > 0) {

    PRINTLN("Sampling ALL");

    switch (queue_get()) {
      case TEMP: {

        bme_force_temp();
        delay(50);
        curr_temp_in = bme_get_temp();
        curr_temp_out = ntc_get_temperature_c();

        if (curr_temp_in > max_temp_in) max_temp_in = curr_temp_in;
        if (curr_temp_in < min_temp_in) min_temp_in = curr_temp_in;

        if (curr_temp_out > max_temp_out) max_temp_out = curr_temp_out;
        if (curr_temp_out < min_temp_out) min_temp_out = curr_temp_out;

        frame_update_temperature(curr_temp_in, curr_temp_out);

      } break;

      case HUM: {

        bme_force_hum();
        delay(50);
        curr_hum = bme_get_hum();

        if (curr_hum > max_hum) max_hum = curr_hum;
        if (curr_hum < min_hum) min_hum = curr_hum;

        frame_update_humidity(curr_hum);

      } break;

      case PRES: {

        bme_force_pres();
        delay(50);
        curr_pres = bme_get_pres()/100.00; // to hPa

        if (curr_pres > max_pres) max_pres = curr_pres;
        if (curr_pres < min_pres) min_pres = curr_pres;

        frame_update_pressure(curr_pres);
        
      } break;
      
      case NONE:
        break;
      
      default:
        break;

    }

  } else if (button_pressed) {

    PRINTLN("BUTTON INTERRUPT DETECTED!");

    /* Set LCD backlight to full (not idle) and reset idle timer:  */
    digitalWrite(LCD_ACTIVE_PIN, HIGH);
    cli();
    lcd_idle_timer_sec = 0;
    sei();

    /* Check what button was pressed and do transitions accordingly: */
    uint8_t value = ~(twi_read(IOEADDR, GPIOB));

    if (value & (1 << UP)) {
      PRINTLN("PRESSED: UP");
      curr_state = curr_state->up;

    } else if (value & (1 << DOWN)) {
      PRINTLN("PRESSED: DOWN");
      curr_state = curr_state->down;

    } else if (value & (1 << LEFT)) {
      PRINTLN("PRESSED: LEFT");
      curr_state = curr_state->prev_frame;
    
    } else if (value & (1 << RIGHT)) {
      PRINTLN("PRESSED: RIGHT");
      curr_state = curr_state->next_frame;

    } else if (value & (1 << ENTER)) {
      PRINTLN("PRESSED: ENTER");
      curr_state->enter();
    }

    put_curr_frame();

    /* "Debounce": */
    while (digitalRead(BUTTON_INT_PIN) == LOW) {
      clear_int_reg();
      delay(50);
    }

    /* Reset button flag: */
    button_pressed = false;
  }
  else
  {
    /* Go to sleep: */
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();

    PRINTLN("Awake!");
  }

  delay(10);
}

void state_init()
{
  f1.line0        = frame_line_text[0];
  f1.line1        = frame_line_text[1];
  f1.prev_frame   = &f2;
  f1.next_frame   = &f2;
  f1.up           = &f2;
  f1.down         = &f2;
  f1.enter        = NOP;

  f2.line0        = frame_line_text[2];
  f2.line1        = frame_line_text[3];
  f2.prev_frame   = &f4;
  f2.next_frame   = &f3;
  f2.up           = &f2_1;
  f2.down         = &f2_2;
  f2.enter        = sample_temp;

  f2_1.line0      = frame_line_text[4];
  f2_1.line1      = frame_line_text[5];
  f2_1.prev_frame = &f2;
  f2_1.next_frame = &f2;
  f2_1.up         = &f2_1;
  f2_1.down       = &f2;
  f2_1.enter      = NOP;

  f2_2.line0      = frame_line_text[6];
  f2_2.line1      = frame_line_text[7];
  f2_2.prev_frame = &f2;
  f2_2.next_frame = &f2;
  f2_2.up         = &f2;
  f2_2.down       = &f2_2;
  f2_2.enter      = NOP;

  f3.line0        = frame_line_text[8];
  f3.line1        = frame_line_text[9];
  f3.prev_frame   = &f2;
  f3.next_frame   = &f4;
  f3.up           = &f3_1;
  f3.down         = &f3_2;
  f3.enter        = sample_temp;

  f3_1.line0      = frame_line_text[10];
  f3_1.line1      = frame_line_text[11];
  f3_1.prev_frame = &f3;
  f3_1.next_frame = &f3;
  f3_1.up         = &f3_1;
  f3_1.down       = &f3;
  f3_1.enter      = NOP;

  f3_2.line0      = frame_line_text[12];
  f3_2.line1      = frame_line_text[13];
  f3_2.prev_frame = &f3;
  f3_2.next_frame = &f3;
  f3_2.up         = &f3;
  f3_2.down       = &f3_2;
  f3_2.enter      = NOP;

  f4.line0        = frame_line_text[14];
  f4.line1        = frame_line_text[15];
  f4.prev_frame   = &f3;
  f4.next_frame   = &f2;
  f4.up           = &f4_1;
  f4.down         = &f4_2;
  f4.enter        = NOP;

  /* Store prompt: */
  f4_1.line0      = frame_line_text[16];
  f4_1.line1      = frame_line_text[17];
  f4_1.prev_frame = &f4;
  f4_1.next_frame = &f4;
  f4_1.up         = &f4_1;
  f4_1.down       = &f4;
  f4_1.enter      = store_data_to_eeprom;

  /* Reset prompt: */
  f4_2.line0      = frame_line_text[18];
  f4_2.line1      = frame_line_text[19];
  f4_2.prev_frame = &f4;
  f4_2.next_frame = &f4;
  f4_2.up         = &f4;
  f4_2.down       = &f4_2;
  f4_2.enter      = reset_data_on_eeprom;
}

/* Updates the screen with the current frame: */
void put_curr_frame()
{
  lcd_clear();
  put_string_at(&curr_state->line0[0], 16, 0, 0);
  put_string_at(&curr_state->line1[0], 16, 1, 0);
}

/* Button ISR: */
void check_button()
{
  button_pressed = true;
}

/* Temperature sample ISR: */
void sample_temp()
{
  queue_put(TEMP);
  queue_put(PRES);
  queue_put(HUM);
}

void store_data_to_eeprom()
{
  PRINT("Storing data to EEPROM...");

  /* Wait for completion of previous write */
	while(!eeprom_is_ready());

  eeprom_write_float(MAX_TEMP_IN_ADDR, max_temp_in);
  eeprom_write_float(MIN_TEMP_IN_ADDR, min_temp_in);

  eeprom_write_float(MAX_TEMP_OUT_ADDR, max_temp_out);
  eeprom_write_float(MIN_TEMP_OUT_ADDR, min_temp_out);

  eeprom_write_float(MAX_PRES_ADDR, max_pres);
  eeprom_write_float(MIN_PRES_ADDR, min_pres);

  eeprom_write_float(MAX_HUM_ADDR, max_hum);
  eeprom_write_float(MIN_HUM_ADDR, min_hum);

  PRINTLN("DONE!");
}

void restore_data_from_eeprom()
{
  PRINT("Restoring data from EEPROM...");

  /* Wait for completion of previous write */
	while(!eeprom_is_ready());

  max_temp_in = eeprom_read_float(MAX_TEMP_IN_ADDR);
  min_temp_in = eeprom_read_float(MIN_TEMP_IN_ADDR);
  
  max_temp_out = eeprom_read_float(MAX_TEMP_OUT_ADDR);
  min_temp_out = eeprom_read_float(MIN_TEMP_OUT_ADDR);
  
  max_pres = eeprom_read_float(MAX_PRES_ADDR);
  min_pres = eeprom_read_float(MIN_PRES_ADDR);
  
  max_hum = eeprom_read_float(MAX_HUM_ADDR);
  min_hum = eeprom_read_float(MIN_HUM_ADDR);

  PRINTLN("DONE!");
}

void reset_data_on_eeprom()
{
  PRINT("Reset data on EEPROM...");

  /* Wait for completion of previous write */
	while(!eeprom_is_ready());


  eeprom_write_float(MAX_TEMP_IN_ADDR, 0.00);
  eeprom_write_float(MIN_TEMP_IN_ADDR, 999.00);

  eeprom_write_float(MAX_TEMP_OUT_ADDR, 0.00);
  eeprom_write_float(MIN_TEMP_OUT_ADDR, 999.00);

  eeprom_write_float(MAX_PRES_ADDR, 0.00);
  eeprom_write_float(MIN_PRES_ADDR, 9999.00);

  eeprom_write_float(MAX_HUM_ADDR, 0.00);
  eeprom_write_float(MIN_HUM_ADDR, 999.00);

  PRINTLN("DONE!");
}

/* LCD idle ISR: */
void lcd_idle_mode()
{
  digitalWrite(LCD_ACTIVE_PIN, LOW); //turn down the display until user activity
  //clear_int_reg(); //TODO: MOVE IT or not needed. HERE IS WHERE IT IS FREEZING AT IDLE. NEEDED? If there are issues with buttons or lcd, this is it.
}

/* Clear expander interrupt: */
void clear_int_reg() { twi_read(IOEADDR, INTCAPB); }

/* Turn off LCD cursor: */
void cursor_off() { lcd_cmd(IOEADDR, GPIOA, 0b00001100, LOW); }

/* Setup watchdog timer to trigger samplings and LCD idle timer: */
void wdt_init()
{
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDP2) | (1 << WDP1); // corresponds to ~1 seconds
}

/* ISR for Watchdog Timer */
ISR(WDT_vect)
{
  
  lcd_idle_timer_sec++;
  sampling_timer_sec++;

  if (lcd_idle_timer_sec >= LCD_IDLE_TIMER_S)
  {
    lcd_idle_mode();
  }

  if (sampling_timer_sec >= SAMPLING_PERIOD_S)
  {
    sample_temp();
    sampling_timer_sec = 0;
  }

}

void NOP(void)
{
  return;
}

