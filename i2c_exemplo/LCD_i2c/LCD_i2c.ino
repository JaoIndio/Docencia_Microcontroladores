#include <Wire.h>

#define LCD_ADDR 0x27 // Change to 0x3F if your I2C backpack uses that address

// Bit positions in PCF8574
#define LCD_RS  (1 << 4)
#define LCD_RW  (1 << 5)  // Usually grounded
#define LCD_EN  (1 << 6)
#define LCD_BL  (1 << 7)  // Backlight

void sendNibble(uint8_t nibble, uint8_t control) {
  uint8_t data = (nibble & 0x0F) << 0; // Shift into P0-P3
  data |= control | LCD_BL;           // Add RS/EN/BL bits

  // Send with EN high then low (LCD latch)
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(data | LCD_EN); // EN = 1
  Wire.endTransmission();
  delayMicroseconds(1);

  Wire.beginTransmission(LCD_ADDR);
  Wire.write(data & ~LCD_EN); // EN = 0
  Wire.endTransmission();
  delayMicroseconds(50);
}

void sendByte(uint8_t value, bool isData) {
  uint8_t control = isData ? LCD_RS : 0;
  sendNibble(value >> 4, control);  // Send high nibble
  sendNibble(value & 0x0F, control); // Send low nibble
}

void lcdCommand(uint8_t cmd) {
  sendByte(cmd, false);
}

void lcdWriteChar(char c) {
  sendByte(c, true);
}

void lcdInit() {
  delay(50); // Wait after power on

  // Init sequence in 4-bit mode
  sendNibble(0x03, 0);
  delay(5);
  sendNibble(0x03, 0);
  delay(5);
  sendNibble(0x03, 0);
  delay(1);
  sendNibble(0x02, 0); // Set 4-bit mode

  // Function set: 4-bit, 2 lines, 5x8 dots
  lcdCommand(0x28);
  // Display ON, Cursor OFF, Blink OFF
  lcdCommand(0x0C);
  // Entry mode set: cursor moves right
  lcdCommand(0x06);
  // Clear display
  lcdCommand(0x01);
  delay(2);
}

void lcdSetCursor(uint8_t col, uint8_t row) {
  const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  lcdCommand(0x80 | (col + row_offsets[row]));
}

void lcdPrint(const char* str) {
  while (*str) {
    lcdWriteChar(*str++);
  }
}

void setup() {
  Wire.begin();
  lcdInit();
  lcdSetCursor(0, 0);
  lcdPrint("Hello");
  lcdSetCursor(0, 1);
  lcdPrint("I2C No Lib!");
}

void loop() {
  // Nothing here
}
