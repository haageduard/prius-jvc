#ifndef __INCLUDE_MAIN_H__
#define __INCLUDE_MAIN_H__

#include <stdbool.h>

// Ports definition
#define SW1_PIN 2

#define BTN_NONE_ID 0
#define BTN_VOLUP_ID 1
#define BTN_VOLDN_ID 2
#define BTN_MUTE_ID 3
#define BTN_BACK_ID 4
#define BTN_FWD_ID 5
#define BTN_BACK_HOLD_ID 6
#define BTN_FWD_HOLD_ID 7
#define BTN_SOURCE_ID 8
#define BTN_SOUND_ID 9
#define BTN_POWER_ID 10

// SW1 resistance values
#define SW1_NONE 535
#define SW1_VOLUP 198
#define SW1_VOLDN 340
#define SW1_DOWN 87
#define SW1_UP 0 // 1 - 5
#define DIFF 30

// prescaler / Fcpu = v * 1000 = v2 = m / v2 = TCNT0_VAL (m - time in milliseconds)
#define TCNT0_VAL 255 - 9 // (1024 / 9600000 = 0,000106667 * 1000 = 0,106666667 = 1 / 0,106666667 = 9,374999971)

#define BTN_LONG_DELAY 1000
#define BTN_LONG_PAUSE 1000

// Procedures definition
void init_ports();
void adc_init();
uint16_t adc_read();

// Variables space
volatile uint16_t millis = 0;
uint16_t sw1_adc_val = 0;
volatile bool long_press = false;
bool int_enabled = false;
uint8_t long_press_count = 0;

uint8_t btn_id = BTN_NONE_ID;
uint8_t prev_btn_id = BTN_NONE_ID;

#endif

