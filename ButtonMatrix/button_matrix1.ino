#define NROWS 3
#define NCOLS 3
const uint8_t row_pins[NROWS] = {2, 3, 4};
const uint8_t col_pins[NCOLS] = {5, 6, 7};
volatile bool btn_pressed[NCOLS] = {false, false, false};
volatile uint8_t row = 0;
const unsigned int period = 100;
volatile unsigned int counter = 0;

void setup() {
  cli();
  for(int i = 0;i < NROWS; i++) {
    pinMode(row_pins[i], OUTPUT);
  }
  for(int i = 0;i < NCOLS; i++) {
    DDRD &= ~(1 << col_pins[i]);
    PORTD |= 1 << col_pins[i];
  }
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 250;
  TCCR1B |= 1 << WGM12;
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TIMSK1 |= 1 << OCIE1A;
  Serial.begin(9600);
  sei();
}

ISR(TIMER1_COMPA_vect) {
  counter++;
  if (counter >= period) {
    counter = 0; 
    update_button_state();
    row = ++row % NROWS;
  }
}

void update_button_state() {
    for(int i = 0; i < NROWS; i++) {
      PORTD |= 1 << row_pins[i];
    }
    PORTD &= ~(1 << row_pins[row]);
    for(int icol = 0; icol < NCOLS; icol++) {
      btn_pressed[icol] = !(PIND & (1 << col_pins[icol]));
    }
    report_row_states(row);
}

void report_row_states(int row_number) {
  for(int icol = 0; icol < NCOLS; icol++) {
    if(btn_pressed[icol] == true) {
      Serial.print("Button ");
      Serial.print(1 + icol + NCOLS * row_number);
      Serial.println(" pressed");
    }
  }
}

void loop()
{
}