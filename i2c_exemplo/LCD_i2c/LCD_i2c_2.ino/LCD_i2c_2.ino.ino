#include <Wire.h>

#define LCD_ADDR 0x27  // Replace with your I2C address if different

// Control bits
#define RS (1 << 0)
#define RW (1 << 1)  // Always 0 for write
#define EN (1 << 2)
#define BL (1 << 3)  // Backlight ON

void lcd_pulse_enable(uint8_t data) {
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(data | EN); // EN = 1
  Wire.endTransmission();
  delayMicroseconds(1);

  Wire.beginTransmission(LCD_ADDR);
  Wire.write(data & ~EN); // EN = 0
  Wire.endTransmission();
  delayMicroseconds(50);
}

void lcd_write_nibble(uint8_t nibble, uint8_t mode) {
  uint8_t data = (nibble << 4) | BL | mode; // D4-D7 + backlight + RS
  lcd_pulse_enable(data);
}

void lcd_send_byte(uint8_t byte, uint8_t mode) {
  lcd_write_nibble(byte >> 4, mode);  // High nibble
  lcd_write_nibble(byte & 0x0F, mode); // Low nibble
}

void lcd_command(uint8_t cmd) {
  lcd_send_byte(cmd, 0); // RS = 0
}

void lcd_data(uint8_t data) {
  lcd_send_byte(data, RS); // RS = 1
}

void lcd_init() {
  delay(50); // Wait for LCD to power up

  // Force 4-bit mode with special sequence
  lcd_write_nibble(0x03, 0);
  delay(5);
  lcd_write_nibble(0x03, 0);
  delayMicroseconds(150);
  lcd_write_nibble(0x03, 0);
  lcd_write_nibble(0x02, 0); // Set to 4-bit mode

  // Standard LCD init sequence
  lcd_command(0x28); // 4-bit, 2 lines, 5x8 font
  lcd_command(0x0C); // Display on, cursor off, blink off
  lcd_command(0x06); // Entry mode: move right
  lcd_command(0x01); // Clear display
  delay(2);          // Wait for clear to complete
}

void setup() {
  Wire.begin();
  lcd_init();

  // Set cursor at position 0, line 0
  lcd_command(0x80);  // DDRAM address for 0,0

  // Send "hello"
  const char *msg = "Hey";
  while (*msg) {
    lcd_data(*msg++);
  }
}

void loop() {
  // Nothing to do
}
