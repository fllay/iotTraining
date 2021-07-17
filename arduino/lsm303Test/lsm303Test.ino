
#include <LSM303AGR_ACC_Sensor.h>
#include <LSM303AGR_MAG_Sensor.h>

#define DEV_I2C Wire1   //Define which I2C bus is used. Wire1 for the Arduino Due
#define SerialPort Serial


// Components.
LSM303AGR_ACC_Sensor Acc(&DEV_I2C);
LSM303AGR_MAG_Sensor Mag(&DEV_I2C);

void setup() {
  // Led.
  pinMode(12, OUTPUT);
  DEV_I2C.begin(4, 5);

  // Initialize serial for output.
  SerialPort.begin(115200);
  
  // Initialize I2C bus.
  DEV_I2C.begin();

  // Initlialize components.
  Acc.begin();
  Acc.Enable();
  Acc.EnableTemperatureSensor();
  Mag.begin();
  Mag.Enable();
}

void loop() {
  // Led blinking.
  digitalWrite(12, HIGH);
  delay(250);
  digitalWrite(12, LOW);
  delay(250);

  // Read accelerometer LSM303AGR.
  int32_t accelerometer[3];
  Acc.GetAxes(accelerometer);

  // Read temperature LSM303AGR.
  float temperature;
  Acc.GetTemperature(&temperature);
  
  // Read magnetometer LSM303AGR.
  int32_t magnetometer[3];
  Mag.GetAxes(magnetometer);

  // Output data.
  SerialPort.print("| Acc[mg]: ");
  SerialPort.print(accelerometer[0]);
  SerialPort.print(" ");
  SerialPort.print(accelerometer[1]);
  SerialPort.print(" ");
  SerialPort.print(accelerometer[2]);
  SerialPort.print(" | Mag[mGauss]: ");
  SerialPort.print(magnetometer[0]);
  SerialPort.print(" ");
  SerialPort.print(magnetometer[1]);
  SerialPort.print(" ");
  SerialPort.print(magnetometer[2]);
  SerialPort.print(" | Temp[C]: ");
  SerialPort.print(temperature, 2);
  SerialPort.println(" |");
}
