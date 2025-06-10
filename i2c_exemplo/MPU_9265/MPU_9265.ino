#include <Wire.h>

#define MPU_ADDR       0x68

// MPU-9250 registers
#define ACCEL_XOUT_H   0x3B

void setup() {
  Serial.begin(115200);
  Wire.begin();

  delay(100);
  
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz, temp;

  // --- Read Accelerometer and Gyroscope ---
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14);

  ax = read16();
  ay = read16();
  az = read16();
  temp = read16();
  gx = read16();
  gy = read16();
  gz = read16();


  // --- Print Data ---
  Serial.print("Accel Gs: ");
  Serial.print(ax / 16384.0); Serial.print(" ");
  Serial.print(ay / 16384.0); Serial.print(" ");
  Serial.println(az / 16384.0);

  Serial.print("Gyro °/s: ");
  Serial.print(gx / 131.0); Serial.print(" ");
  Serial.print(gy / 131.0); Serial.print(" ");
  Serial.println(gz / 131.0);

  delay(500);
}

int16_t read16() {
  uint8_t l = Wire.read();
  uint8_t h = Wire.read();
  return (int16_t)(h << 8 | l);
}

