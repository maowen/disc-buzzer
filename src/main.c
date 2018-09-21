#include <avr/io.h>

#include <stdint.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

/*
static inline void delay (uint16_t millis) {
  for (volatile uint16_t i = 34*millis; i>0; i--);
}
*/

typedef enum {
    OUTCMP_CTC_7000_HZ = 570,
    OUTCMP_CTC_6000_HZ = 666,
    OUTCMP_CTC_5000_HZ = 799,
    OUTCMP_CTC_4000_HZ = 199,
} outcmp_ctc_t;
  
typedef enum {
  WTDCSR_TIMEOUT_16MS =   0x00,
  WTDCSR_TIMEOUT_32MS =   0x01,
  WTDCSR_TIMEOUT_64MS =   0x02,
  WTDCSR_TIMEOUT_125MS =  0x03,
  WTDCSR_TIMEOUT_250MS =  0x04,
  WTDCSR_TIMEOUT_500MS =  0x05,
  WTDCSR_TIMEOUT_1000MS = 0x06,
  WTDCSR_TIMEOUT_2000MS = 0x07,
  WTDCSR_TIMEOUT_4000MS = 0x20,
  WTDCSR_TIMEOUT_8000MS = 0x21,
} wdt_timeout_t;

int main (void) {
  // System clock selected in Makefile (4 MHz)
  clock_prescale_set(clock_div_1);

  // Disable interrupts
  cli();

  OCR0A = OUTCMP_CTC_6000_HZ;
  TCNT0 = 0;
  // Configure Timer output
  TCCR0A = 1<<COM0A0 | 0<<WGM00;  // CTC OCR0A mode
  TCCR0B = 1<<WGM02 | 1<<CS00;    // CTC OCR0A Mode; enable clk/1

  // Configure watchdog
  CCP = 0xD8; // Write signature for change enable of protected I/O register
  // Enable Watchdog interrupt and set timeout
  WDTCSR = 1 << WDIE | WTDCSR_TIMEOUT_32MS;

  // Enable PB0 and PB1 as outputs
  DDRB = _BV(DDB0) | _BV(DDB1);
  PUEB = _BV(PUEB2); // Enable pull-up on input pin PB2

  EICRA = _BV(ISC01); // Falling edge of INT0 generates an interrupt request
  EIMSK = 1; // Enable INT0 interrupt

  // Enable interrupts
  sei();

  while (1) {
  }
}

static const uint8_t INT0_TIMEOUT_START = 2;
static uint8_t int0_timeout = 0; 

ISR(INT0_vect) {
  PINB = _BV(PINB1); // Toggle PB1
  EIMSK = 0; // Disable INT0 interrupt
  int0_timeout = INT0_TIMEOUT_START;
}

static const uint8_t period = 32;
static const uint8_t on_period = 8;
static uint8_t period_idx = 0;

ISR(WDT_vect) {
  // INT0 Debouncing
  // Wait for Pin2 to go high then start the timeout
  if (PINB & _BV(PINB2)) {
    if (int0_timeout) {
      --int0_timeout;
    }
    else {
      EIFR = 1; // Clear INT0 flag
      EIMSK = 1; // Enable INT0 interrupt
    }
  }

  if (period_idx == 0) {
      // Enable output from PB0
      DDRB |= _BV(DDB0);
  }

  if (period_idx < on_period) {
    /* PINB = _BV(PINB1); */
    /* PORTB  ^= _BV(PORTB1); // Toggle Pin1 */
    /* DDRB ^= _BV(DDB0); // Toggle OCR0A enable */
    OCR0A = period_idx % 2 ? OUTCMP_CTC_5000_HZ : OUTCMP_CTC_6000_HZ;
    TCNT0 = 0;
  }
  else {
    /* PORTB &= ~_BV(PORTB1); */
    DDRB &= ~_BV(DDB0);
  }

  ++period_idx;
  period_idx %= period;
}
