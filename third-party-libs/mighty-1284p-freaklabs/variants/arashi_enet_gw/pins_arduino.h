#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define ARASHI_ENET_GATEWAY 1

#define NUM_DIGITAL_PINS            31
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? (p) + 24 : -1)

#define digitalPinHasPWM(p)         ((p) == 7 || (p) == 8 || (p) == 9 || (p) == 10)

static const uint8_t SS   = 10;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 12;
static const uint8_t SCK  = 13;

static const uint8_t SDA = 20;
static const uint8_t SCL = 21;
static const uint8_t LED = 10;

static const uint8_t A0 = 24;
static const uint8_t A1 = 25;
static const uint8_t A2 = 26;
static const uint8_t A3 = 27;
static const uint8_t A4 = 28;
static const uint8_t A5 = 29;
static const uint8_t A6 = 30;
static const uint8_t A7 = 31;

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) < NUM_DIGITAL_PINS) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 1 : (((p) <= 15) ? 3 : (((p) <= 23) ? 2 : 0)))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) ((p) % 8)

#ifdef ARDUINO_MAIN

#define PA 1
#define PB 2
#define PC 3
#define PD 4

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
    NOT_A_PORT,
    (uint16_t) &DDRA,
    (uint16_t) &DDRB,
    (uint16_t) &DDRC,
    (uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
    NOT_A_PORT,
    (uint16_t) &PORTA,
    (uint16_t) &PORTB,
    (uint16_t) &PORTC,
    (uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
    NOT_A_PORT,
    (uint16_t) &PINA,
    (uint16_t) &PINB,
    (uint16_t) &PINC,
    (uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
    PD, /* 0 */
    PD,
    PD,
    PD,
    PB,
    PB,
    PB,
    PB,
    PD, /* 8 */
    PD,
    PB,
    PB,
    PB,
    PB,
    PC,
    PC,
    PC, /* 16 */
    PC,
    PD,
    PD,
    PC,
    PC,
    PC,
    PC,
    PA, /* 24 */
    PA,
    PA,
    PA,
    PA,
    PA,
    PA,
    PA  /* 31 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
    _BV(0), /* 0 */
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(0),
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(6), /* 8 */
    _BV(5),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    _BV(6),
    _BV(7),
    _BV(5), /* 16, port C */
    _BV(4),
    _BV(4),
    _BV(7),
    _BV(1),
    _BV(0),
    _BV(6),
    _BV(7),
    _BV(0), /* 24, port A */
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7)
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
    NOT_ON_TIMER,   /* 0  - PD0 */
    NOT_ON_TIMER,   /* 1  - PD1 */
    NOT_ON_TIMER,   /* 2  - PD2 */
    NOT_ON_TIMER,   /* 3  - PD3 */
    NOT_ON_TIMER,   /* 4  - PB0 */
    NOT_ON_TIMER,   /* 5  - PB1 */
    NOT_ON_TIMER,   /* 6  - PB2 */
    TIMER0A,        /* 7  - PB3 */
    TIMER2B,        /* 8  - PD6 */
    TIMER1A,        /* 9  - PD5 */
    TIMER0B,        /* 10 - PB4 */
    NOT_ON_TIMER,   /* 11 - PB5 */
    NOT_ON_TIMER,   /* 12 - PB6 */
    NOT_ON_TIMER,   /* 13 - PB7 */
    NOT_ON_TIMER,   /* 14 - NA */
    NOT_ON_TIMER,   /* 15 - NA */
    NOT_ON_TIMER,   /* 16 - PC5 */
    NOT_ON_TIMER,   /* 17 - NA */
    TIMER1B,        /* 18 - PD4 */
    TIMER2A,        /* 19 - PD7 */
    NOT_ON_TIMER,   /* 20 - PC1 */
    NOT_ON_TIMER,   /* 21 - PC0 */
    NOT_ON_TIMER,   /* 22 - PC6 */
    NOT_ON_TIMER,   /* 23 - PC7 */
    NOT_ON_TIMER,   /* 24 - PA0 */
    NOT_ON_TIMER,   /* 25 - PA1 */
    NOT_ON_TIMER,   /* 26 - PA2 */
    NOT_ON_TIMER,   /* 27 - PA3 */
    NOT_ON_TIMER,   /* 28 - PA4 */
    NOT_ON_TIMER,   /* 29 - PA5 */
    NOT_ON_TIMER,   /* 30 - NA */
    NOT_ON_TIMER    /* 31 - PA7 */
};

#endif // ARDUINO_MAIN

#endif // Pins_Arduino_h
// vim:ai:cin:sts=2 sw=2 ft=cpp
