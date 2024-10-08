
#include "Arduino.h"
#include <Wire.h>
#include <SimpleFOC.h>
#include <SimpleFOCDrivers.h>
#include "drivers/drv8316/drv8316.h"

SPIClass SPI_1(SPI_MOSI, SPI_MISO, SPI_SCK);
BLDCMotor motor = BLDCMotor(11);
DRV8316Driver6PWM driver = DRV8316Driver6PWM(PHA_H, PHA_L, PHB_H, PHB_L, PHC_H, PHC_L, DRV_CS, false);


void printDRV8316Status() {
  DRV8316Status status = driver.getStatus();
  Serial.println("DRV8316 Status:");
  Serial.print("Fault: ");
  Serial.println(status.isFault());
  Serial.print("Buck Error: ");
  Serial.print(status.isBuckError());
  Serial.print("  Undervoltage: ");
  Serial.print(status.isBuckUnderVoltage());
  Serial.print("  OverCurrent: ");
  Serial.println(status.isBuckOverCurrent());
  Serial.print("Charge Pump UnderVoltage: ");
  Serial.println(status.isChargePumpUnderVoltage());
  Serial.print("OTP Error: ");
  Serial.println(status.isOneTimeProgrammingError());
  Serial.print("OverCurrent: ");
  Serial.print(status.isOverCurrent());
  Serial.print("  Ah: ");
  Serial.print(status.isOverCurrent_Ah());
  Serial.print("  Al: ");
  Serial.print(status.isOverCurrent_Al());
  Serial.print("  Bh: ");
  Serial.print(status.isOverCurrent_Bh());
  Serial.print("  Bl: ");
  Serial.print(status.isOverCurrent_Bl());
  Serial.print("  Ch: ");
  Serial.print(status.isOverCurrent_Ch());
  Serial.print("  Cl: ");
  Serial.println(status.isOverCurrent_Cl());
  Serial.print("OverTemperature: ");
  Serial.print(status.isOverTemperature());
  Serial.print("  Shutdown: ");
  Serial.print(status.isOverTemperatureShutdown());
  Serial.print("  Warning: ");
  Serial.println(status.isOverTemperatureWarning());
  Serial.print("OverVoltage: ");
  Serial.println(status.isOverVoltage());
  Serial.print("PowerOnReset: ");
  Serial.println(status.isPowerOnReset());
  Serial.print("SPI Error: ");
  Serial.print(status.isSPIError());
  Serial.print("  Address: ");
  Serial.print(status.isSPIAddressError());
  Serial.print("  Clock: ");
  Serial.print(status.isSPIClockFramingError());
  Serial.print("  Parity: ");
  Serial.println(status.isSPIParityError());
  if (status.isFault())
    driver.clearFault();
  delayMicroseconds(1);  // ensure 400ns delay
  DRV8316_PWMMode val = driver.getPWMMode();
  Serial.print("PWM Mode: ");
  Serial.println(val);
  delayMicroseconds(1);  // ensure 400ns delay
  bool lock = driver.isRegistersLocked();
  Serial.print("Lock: ");
  Serial.println(lock);
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(1);
  Serial.println("Initializing...");

  driver.voltage_power_supply = 12;
  driver.init(&SPI_1);
  driver.setBuckVoltage(VB_5V);
  driver.setOCPLevel(Curr_24A);
  driver.setOCPMode(AutoRetry_Fault);
  driver.setOCPRetryTime(Retry500ms);
  driver.setSlew(Slew_50Vus);

  motor.linkDriver(&driver);
  motor.controller = MotionControlType::velocity_openloop;
  motor.voltage_limit = 4;
  motor.velocity_limit = 20;
  motor.init();
  Serial.println("Init complete...");

  delay(100);
  printDRV8316Status();
}


// velocity set point variable
float target_velocity = 7.0;


void loop() {
  motor.move(target_velocity);
}
