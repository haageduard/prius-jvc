/****
* Project: JVC remote control for Prius NHW20
* Used JVC RM-RK52 control codes
* Author: Eduard Haag
* Version: 0.2 (14/04/2017)
*
* History:
*
* 05/04/2017 initial version
* 14/05/2017 stable version (version up to 0.2)

****/

#include "config.h"

#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "main.h"
#ifdef SOFTWARE_UART_TX
#include "uart.h"
#endif
#include "jvc.h"

// init ports
void init_ports() {
  JVC_DDR |= 1 << JVC_DQ;
  JVC_LOW // Make PIN low to shut off transistor
}

// initialize adc
void adc_init() {
  // AREF = AVcc
  //ADMUX = (1 << REFS0);
  ADMUX = 0x02;
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// init timer interrupt
void init_timer_interrupt() {
  // set prescaler to 1024
  TCCR0B |= (_BV(CS02) | _BV(CS00));
  // set default value
  TCNT0 = TCNT0_VAL;
}

ISR(TIM0_OVF_vect) {
  millis++;
  if (millis == BTN_LONG_DELAY) {
    long_press = true;
    millis = 0;
  }
  TCNT0 = TCNT0_VAL;
}

// read adc value
uint16_t adc_read() {

  // select the corresponding channel 0~7
  // ANDing with '7' will always keep the value
  // of 'ch' between 0 and 7
  // ch &= 0b00000111; // AND operation with 7
  // ADMUX = (ADMUX & 0xF8) | ch; // clears the bottom 3 bits before ORing


  // start single conversion
  // write '1' to ADSC
  ADCSRA |= (1 << ADSC);

  // wait for conversion to complete
  // ADSC becomes '0' again
  // till then, run loop continuously
  while (ADCSRA & (1 << ADSC));

  return (ADC);

}

void handle_button(uint8_t id) {
  jvc_init();
  switch (id) {
    #ifdef BTN_VOLUP_ENABLE
    case BTN_VOLUP_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("VOL+");
      #endif
      jvc_volume_up();
      break;
    }
    #endif
    #ifdef BTN_VOLDN_ENABLE
    case BTN_VOLDN_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("VOL-");
      #endif
      jvc_volume_down();
      break;
    }
    #endif
    #ifdef BTN_MUTE_ENABLE
    case BTN_MUTE_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("MUTE");
      #endif
      jvc_mute();
      break;
    }
    #endif
    #ifdef BTN_BACK_ENABLE
    case BTN_BACK_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("BACK");
      #endif
      jvc_skip_back();
      break;
    }
    #endif
    #ifdef BTN_FWD_ENABLE
    case BTN_FWD_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("FWD");
      #endif
      jvc_skip_fwd();
      break;
    }
    #endif
    #ifdef BTN_BACK_HOLD_ENABLE
    case BTN_BACK_HOLD_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("BACK_HOLD");
      #endif
      jvc_back_hold();
      break;
    }
    #endif
    #ifdef BTN_FWD_HOLD_ENABLE
    case BTN_FWD_HOLD_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("FWD_HOLD");
      #endif
      jvc_fwd_hold();
      break;
    }
    #endif
    #ifdef BTN_SOURCE_ENABLE
    case BTN_SOURCE_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("SOURCE");
      #endif
      jvc_source();
      break;
    }
    #endif
    #ifdef BTN_SOUND_ENABLE
    case BTN_SOUND_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("SOUND");
      #endif
      jvc_sound();
      break;
    }
    #endif
    #ifdef BTN_POWER_ENABLE
    case BTN_POWER_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("POWER");
      #endif
      jvc_power_off();
      break;
    }
    #endif
    case BTN_NONE_ID: {
      #ifdef SOFTWARE_UART_TX
      send_uart_str("NONE");
      #endif
      jvc_none();
      break;
    }
    default: {      
    }
  }
}

uint8_t get_long_key(uint8_t id) {
  switch (id) {
    case BTN_VOLUP_ID: {
      #ifdef BTN_SOURCE_ENABLE
      return BTN_SOURCE_ID;
      #endif
      break;
    }
    case BTN_VOLDN_ID: {
      #ifdef BTN_MUTE_ENABLE
      return BTN_MUTE_ID;
      #endif
      break;
    }
    case BTN_BACK_ID: {
      #ifdef BTN_BACK_HOLD_ENABLE
      return BTN_BACK_HOLD_ID;
      #endif
      break;
    }
    case BTN_FWD_ID: {
      #ifdef BTN_FWD_HOLD_ENABLE
      return BTN_FWD_HOLD_ID;
      #endif
      break;
    }
    default: {
      return id;
      break;
    }
  }
}

// main loop
int main(void) {

  wdt_enable(WDTO_4S);
  init_ports();
  #ifdef SOFTWARE_UART_TX
  init_uart();
  send_uart_str("START");
  #endif
  init_timer_interrupt();
  adc_init();

  //_delay_ms(2000); // startup delay

  #ifdef DEBUG_LOGIC_ANALYZER
  jvc_init();
  jvc_volume_up();
  #endif

  sei();

  while (1) {

    // if (TIMSK0 & (1 << TOIE0)) {
    //   TIMSK0 |= (0 << TOIE0);
    //   int_enabled = true;
    // } else {
    //   int_enabled = false;
    // }
    sw1_adc_val = adc_read();
    // if (int_enabled) {
    //   TIMSK0 |= (1 << TOIE0);
    // }

     prev_btn_id = btn_id;
    if (sw1_adc_val > SW1_VOLUP - DIFF && sw1_adc_val < SW1_VOLUP + DIFF) {
      btn_id = BTN_VOLUP_ID;
    } else if (sw1_adc_val > SW1_VOLDN - DIFF && sw1_adc_val < SW1_VOLDN + DIFF) {
      btn_id = BTN_VOLDN_ID;
    } else if (sw1_adc_val > SW1_DOWN - DIFF && sw1_adc_val < SW1_DOWN + DIFF) {
      btn_id = BTN_BACK_ID;
    } else if (sw1_adc_val >= SW1_UP && sw1_adc_val < SW1_UP + DIFF) {
      btn_id = BTN_FWD_ID;
    } else if (sw1_adc_val > SW1_NONE - DIFF && sw1_adc_val < SW1_NONE + DIFF) {
      btn_id = BTN_NONE_ID;
    } else {
      btn_id = BTN_NONE_ID;
    }

    if (prev_btn_id == BTN_NONE_ID && btn_id != BTN_NONE_ID) {
      // key pressed
      TIMSK0 |= (1 << TOIE0);
      millis = 0;
    } else if (prev_btn_id != BTN_NONE_ID && btn_id == BTN_NONE_ID) {
      // key released
      TIMSK0 |= (0 << TOIE0);
      if (long_press_count == 0) {
        handle_button(prev_btn_id);
      }
    }
    #ifdef LONG_PRESS_ENABLE
    else if (btn_id != BTN_NONE_ID && prev_btn_id == btn_id) {
      // key still pressed
      if (long_press) {
        long_press_count++;
        if (long_press_count == 3) {
          TIMSK0 |= (0 << TOIE0);
          btn_id = get_long_key(btn_id);
          handle_button(btn_id);
          prev_btn_id = BTN_NONE_ID;
          btn_id = BTN_NONE_ID;
          long_press = false;
          long_press_count = 0;
          _delay_ms(BTN_LONG_PAUSE);
        }
      }
    }
    #endif
    else {
      TIMSK0 |= (0 << TOIE0);
      prev_btn_id = BTN_NONE_ID;
      btn_id = BTN_NONE_ID;
      long_press_count = 0;
      long_press = false;
    }

    _delay_ms(150);

    wdt_reset();

  }

  return 0;

}
