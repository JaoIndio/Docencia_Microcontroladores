void setup() {
  // Initialize serial ports
  Serial.begin(9600);    // USB to PC (COM1)
  Serial1.begin(9600);   // Hardware Serial2 to Mega2
  
  Serial.println("Mega1 Ready - Type messages to echo to Mega2");
}

void loop() {
  // Forward data from PC to Mega1
  while (Serial.available() > 0) {
    byte data = Serial.read();
    Serial1.write(data);
    //Serial.print((char)data);
  }
}
