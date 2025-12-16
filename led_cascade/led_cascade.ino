#define PINS 5
volatile bool states[PINS] = {false};
volatile unsigned int counters[PINS] = {0};
const unsigned int period = 250;

void setup() {
  cli();
  
  for (int pin = 1; pin <= PINS; pin++) {
    DDRB |= 1 << pin;
  }
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  OCR1A = 249;
  
  TCCR1B |= (1 << WGM12);
  
  TCCR1B |= (1 << CS11) | (1 << CS10);
  
  TIMSK1 |= (1 << OCIE1A);
  
  sei();
}

ISR(TIMER1_COMPA_vect) {
  uint8_t new_state = 0;
  
  for (int i = 0; i < PINS; i++) {
    counters[i]++;
    if (counters[i] > (i + 1) * period) {
      counters[i] = 0;
      states[i] = !states[i];
    }
    new_state |= int(states[i]) << i;
  }
  
  PORTB = new_state;
}

void loop() {
}