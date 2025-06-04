void setup() {
  // Initialize serial ports
  Serial.begin(9600);    // USB to PC (COM1)
  Serial1.begin(9600);   // Hardware Serial2 to Mega2
  
  Serial.println("Mega2 Ready - Type messages to echo to PC");
}

void loop() {
  // Forward data from PC to Mega1
  if (Serial1.available()) {
    byte c = Serial1.read();
    Serial.write(c);  // Send to Mega2
  }
}
