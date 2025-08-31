#include <SPI.h>

volatile bool ready = false;
volatile uint8_t byteCount = 0;
String buf = "";

void setup() {
  Serial.begin(115200);
  pinMode(10, INPUT);    // SS
  SPI.begin();
  SPCR &= ~(1<<MSTR);    // slave
  SPI.attachInterrupt(); // ISR fires on byte received
}

ISR (SPI_STC_vect) {
  uint8_t b = SPDR;
  if (!ready) {
    byteCount = b;        // first byte = length
    ready = true;
    buf.reserve(byteCount);
  } else {
    buf += char(b);
    if (--byteCount == 0) {
      Serial.println(buf);
      buf = "";
      ready = false;
    }
  }
}

void loop() {
  // nothing
}
