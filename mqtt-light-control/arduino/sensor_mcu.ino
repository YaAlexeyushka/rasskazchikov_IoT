const int sensorPin = A0;
bool streaming = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'p') {
      sendValue();
    } else if (cmd == 's') {
      streaming = true;
      Serial.println("STREAM_STARTED");
    }
  }

  if (streaming) {
    sendValue();
    delay(1000);
  }
}

void sendValue() {
  int val = analogRead(sensorPin);
  Serial.print("SENSOR_VALUE:");
  Serial.println(val);
}
