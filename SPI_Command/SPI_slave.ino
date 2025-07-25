#include <SPI.h>

volatile bool ready = false;
volatile uint8_t byteCount = 0;
String buf = "";

#define CMD_LED_CTRL  0x50
#define ACK_BYTE      0xF5
#define LED_PIN       9

void setup() {
  pinMode(MISO, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  SPI.begin();
  SPCR &= ~(1<<MSTR);       // set as slave
  SPI.attachInterrupt();    // enable SPI ISR
}

ISR (SPI_STC_vect) {
  uint8_t b = SPDR;         // read what master sent
  static uint8_t state = 0;
  static uint8_t pin, value;

  switch(state) {
    case 0: // command phase
      if (b == CMD_LED_CTRL) {
        SPDR = ACK_BYTE;    // ACK
        state = 1;
      } else {
        SPDR = 0x00;        // NACK
      }
      break;

    case 1: // receive pin#
      pin = b;
      SPDR = ACK_BYTE;
      state = 2;
      break;

    case 2: // receive LED_ON/OFF
      value = b;
      digitalWrite(pin, value ? HIGH : LOW);
      SPDR = ACK_BYTE;
      state = 0;
      break;
  }
}

void loop() {
  // nothing here—handled in ISR
}
