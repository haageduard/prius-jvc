#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include "jvc.h"

//uint8_t i = 0;

void jvc_init() {
  //i = 0;
  jvc_preamble();
}

void jvc_fin() {
  jvc_postamble();
  _delay_us(JVC_POSTAMBLE_LENGTH);
  //i++;
}

// jvc set pin high
void jvc_pin_high() {
  JVC_HIGH
  _delay_us(JVC_LENGTH * 1); // 537us
  JVC_LOW
  _delay_us(JVC_LENGTH * 3); // 537 * 3 = 1611us
}

// jvc set pin low
void jvc_pin_low() {
  JVC_HIGH
  _delay_us(JVC_LENGTH * 1); // 537us
  JVC_LOW
  _delay_us(JVC_LENGTH * 1); // 537us
}

// jvc preamble
void jvc_preamble() {

  JVC_LOW
  _delay_us(JVC_LENGTH * 1);

  JVC_HIGH
  _delay_us(JVC_LENGTH * 16);

  JVC_LOW
  _delay_us(JVC_LENGTH * 8);

}

// jvc address
void jvc_addr() {

  jvc_pin_high();    // 1 Start Bit

  jvc_pin_high();    //       (7 bit device code)
  jvc_pin_high();
  jvc_pin_high();    // 7
  jvc_pin_low();

  jvc_pin_low();
  jvc_pin_low();    //4
  jvc_pin_high();

}

// jvc postamble
void jvc_postamble() {

  jvc_pin_high(); // 3
  jvc_pin_high();

}

void jvc_cmd_volume_up() {

  jvc_addr();

  jvc_pin_low();
  jvc_pin_low();
  jvc_pin_high();    // 4
  jvc_pin_low();

  jvc_pin_low();
  jvc_pin_low();   // 0
  jvc_pin_low();

  jvc_fin();

}

// jvc volume up
void jvc_volume_up() {

  jvc_cmd_volume_up();
  jvc_cmd_volume_up();

}

void jvc_cmd_volume_down() {

  jvc_addr();

  jvc_pin_high();
  jvc_pin_low();
  jvc_pin_high();    // 5
  jvc_pin_low();

  jvc_pin_low();
  jvc_pin_low();   // 0
  jvc_pin_low();

  jvc_fin();

}

// jvc volume down
void jvc_volume_down() {

  jvc_cmd_volume_down();
  jvc_cmd_volume_down();

}

void jvc_cmd_skip_back() {

  jvc_addr();

  jvc_pin_high();
  jvc_pin_high();
  jvc_pin_low();    // 3
  jvc_pin_low();

  jvc_pin_high();
  jvc_pin_low();   // 1
  jvc_pin_low();

  jvc_fin();

}

// jvc skip back
void jvc_skip_back() {

  jvc_cmd_skip_back();
  jvc_cmd_skip_back();

}

void jvc_cmd_skip_fwd() {

  jvc_addr();

  jvc_pin_low();
  jvc_pin_high();
  jvc_pin_low();    // 2
  jvc_pin_low();

  jvc_pin_high();
  jvc_pin_low();   // 1
  jvc_pin_low();

  jvc_fin();

}

// jvc skip forward
void jvc_skip_fwd() {

  jvc_cmd_skip_fwd();
  jvc_cmd_skip_fwd();

}

void jvc_cmd_mute() {

  jvc_addr();

  jvc_pin_low();
  jvc_pin_high();
  jvc_pin_high();    // E (14)
  jvc_pin_high();

  jvc_pin_low();
  jvc_pin_low();   // 0
  jvc_pin_low();

  jvc_fin();

}

// jvc mute
void jvc_mute() {

  jvc_cmd_mute();
  jvc_cmd_mute();

}

void jvc_cmd_fwd_hold() {

  jvc_addr();

  jvc_pin_low();
  jvc_pin_low();
  jvc_pin_high();    // 4
  jvc_pin_low();

  jvc_pin_high();
  jvc_pin_low();   // 1
  jvc_pin_low();

  jvc_fin();

}

// jvc fwd hold
void jvc_fwd_hold() {

  jvc_cmd_fwd_hold();
  jvc_cmd_fwd_hold();

}

void jvc_cmd_back_hold() {

  jvc_addr();

  jvc_pin_high();
  jvc_pin_low();
  jvc_pin_high();    // 5
  jvc_pin_low();

  jvc_pin_high();
  jvc_pin_low();   // 1
  jvc_pin_low();

  jvc_fin();

}

// jvc back hold
void jvc_back_hold() {

  jvc_cmd_back_hold();
  jvc_cmd_back_hold();

}

void jvc_cmd_source() {

  jvc_addr();

  jvc_pin_low();
  jvc_pin_low();
  jvc_pin_low();    // 8
  jvc_pin_high();

  jvc_pin_low();
  jvc_pin_low();   // 0
  jvc_pin_low();

  jvc_fin();

}

// jvc source
void jvc_source() {

  jvc_cmd_source();
  jvc_cmd_source();

}

void jvc_cmd_sound() {

  jvc_addr();

  jvc_pin_high();
  jvc_pin_low();
  jvc_pin_high();    // D (13)
  jvc_pin_high();

  jvc_pin_low();
  jvc_pin_low();   // 0
  jvc_pin_low();

  jvc_fin();


}

// jvc sound
void jvc_sound() {

  jvc_cmd_sound();
  jvc_cmd_sound();

}

void jvc_cmd_power_off() {

  jvc_addr();

  jvc_pin_low();
  jvc_pin_low();
  jvc_pin_low();    // 0
  jvc_pin_low();

  jvc_pin_low();
  jvc_pin_low();   // 0
  jvc_pin_low();

  jvc_fin();

}

// jvc power off
void jvc_power_off() {

  jvc_cmd_power_off();
  jvc_cmd_power_off();

}

// jvc none
void jvc_none() {
}
