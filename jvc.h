#ifndef INCLUDE_JVC_H_
#define INCLUDE_JVC_H_

// JVC port definition
#define JVC_PORT PORTB
#define JVC_PIN PINB
#define JVC_DDR DDRB
#define JVC_DQ PB3

// Transistor control macros
#define JVC_HIGH JVC_PORT &= ~(1 << JVC_DQ);
#define JVC_LOW JVC_PORT |= (1 << JVC_DQ);

// JVC impulse length definition
#define JVC_LENGTH 537
#define JVC_POSTAMBLE_LENGTH 21000
#define JVC_REPS 2

void jvc_pin_high();
void jvc_pin_low();
void jvc_preamble();
void jvc_addr();
void jvc_postamble();
void jvc_volume_up();
void jvc_volume_down();
void jvc_skip_back();
void jvc_skip_fwd();
void jvc_mute();
void jvc_fwd_hold();
void jvc_back_hold();
void jvc_source();
void jvc_sound();
void jvc_power_off();
void jvc_none();
void jvc_init();

#endif
