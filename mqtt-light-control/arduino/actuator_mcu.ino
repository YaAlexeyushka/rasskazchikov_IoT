const int ledPin = 13;

enum LedState {
  LED_OFF,
  LED_ON
};

LedState currentState = LED_OFF;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
}

void loop() {
  if (!Serial.available()) return;

  char cmd = Serial.read();

  if (cmd == 'u' && currentState != LED_ON) {
    currentState = LED_ON;
    digitalWrite(ledPin, HIGH);
    Serial.println("LED_GOES_ON");
  }

  else if (cmd == 'd' && currentState != LED_OFF) {
    currentState = LED_OFF;
    digitalWrite(ledPin, LOW);
    Serial.println("LED_GOES_OFF");
  }
}
