// ===============================
// Задание 1: blink по прерыванию
// ===============================

volatile bool led_state_interrupt = false;
volatile uint8_t overflow_counter = 0;
const uint8_t OVERFLOW_LIMIT = 61;
const int LED_PIN_INTERRUPT = 13;

void setup_timer2_interrupt() {
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  TIMSK2 |= (1 << TOIE2);
  sei();
}

ISR(TIMER2_OVF_vect) {
  overflow_counter++;
  if (overflow_counter >= OVERFLOW_LIMIT) {
    overflow_counter = 0;
    led_state_interrupt = !led_state_interrupt;
    digitalWrite(LED_PIN_INTERRUPT, led_state_interrupt);
  }
}

// ===============================
// Задание 2: своя millis()
// ===============================

volatile unsigned long timer0_millis = 0;
static uint8_t timer0_fract = 0;

void setup_timer0_for_millis() {
  cli();
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;
  TCCR0A |= (1 << WGM01);
  TCCR0B |= (1 << CS01) | (1 << CS00);
  OCR0A = 249;
  TIMSK0 |= (1 << OCIE0A);
  sei();
}

ISR(TIMER0_COMPA_vect) {
  unsigned long m = timer0_millis;
  uint8_t f = timer0_fract;
  m += 1;
  f += 3;
  if (f >= 125) {
    f -= 125;
    m += 1;
  }
  timer0_fract = f;
  timer0_millis = m;
}

unsigned long my_millis() {
  uint8_t oldSREG = SREG;
  cli();
  unsigned long m = timer0_millis;
  SREG = oldSREG;
  return m;
}

// ===============================
// Задание 3: три вида blink
// ===============================

uint8_t blink_mode = 0;
bool led_state_millis = false;
unsigned long prev_millis_blink = 0;
const uint16_t BLINK_INTERVAL = 500;
const int LED_PIN_ADDITIONAL = 12;

void blink_with_millis() {
  unsigned long now = my_millis();
  if (now - prev_millis_blink >= BLINK_INTERVAL) {
    prev_millis_blink = now;
    led_state_millis = !led_state_millis;
    digitalWrite(LED_PIN_ADDITIONAL, led_state_millis);
  }
}

void blink_with_delay() {
  digitalWrite(LED_PIN_ADDITIONAL, HIGH);
  delay(BLINK_INTERVAL);
  digitalWrite(LED_PIN_ADDITIONAL, LOW);
  delay(BLINK_INTERVAL);
}

void process_uart_commands() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    switch (cmd) {
      case '0': blink_mode = 0; break;
      case '1': blink_mode = 1; break;
      case '2': blink_mode = 2; break;
    }
  }
}

// ===============================
// Задание 4: опрос аналогового пина
// ===============================

unsigned long prev_analog_read = 0;
const uint16_t ANALOG_INTERVAL = 250;
const int ANALOG_PIN = A0;

void read_analog_pin() {
  unsigned long now = my_millis();
  if (now - prev_analog_read >= ANALOG_INTERVAL) {
    prev_analog_read = now;
    int value = analogRead(ANALOG_PIN);
    Serial.println(value);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN_INTERRUPT, OUTPUT);
  pinMode(LED_PIN_ADDITIONAL, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);
  setup_timer0_for_millis();
  setup_timer2_interrupt();
}

void loop() {
  process_uart_commands();
  read_analog_pin();
  switch (blink_mode) {
    case 1: blink_with_millis(); break;
    case 2: blink_with_delay(); break;
  }
}
