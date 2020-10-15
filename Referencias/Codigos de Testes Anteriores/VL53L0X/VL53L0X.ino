//VL53L0X - LONG_RANGE - HIGH_ACCURACY

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X VL53L0X_1;

int VL53L0X_Info;

void setup(){

  Serial.begin(9600);
  Wire.begin();

  VL53L0X_1.setTimeout(500);
  if(!VL53L0X_1.init()){
    Serial.println("Failed to detect and initialize sensor!");
    while(1){}
  }

  //Define LONG_RANGE
  //Lower the return signal rate limit (default is 0.25 MCPS)
  VL53L0X_1.setSignalRateLimit(0.1);
  //Increase laser pulse periods (defaults are 14 and 10 PCLKs)
  VL53L0X_1.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  VL53L0X_1.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  
  //Define HIGH_ACCURACY
  //Increase timing budget to 500000 µs (default is 33000 µs)
  VL53L0X_1.setMeasurementTimingBudget(500000);

}

void loop(){

VL53L0X_Info = VL53L0X_1.readRangeSingleMillimeters();

  if(VL53L0X_Info <= 2500){
    Serial.print("VL53L0X: ");
    Serial.print(VL53L0X_Info);
    Serial.print("mm");
    Serial.println();
  }
  if(VL53L0X_Info > 2500){
    Serial.print("VL53L0X: ");
    Serial.print("Fora do alcance");
    Serial.println();
  }
  if(VL53L0X_1.timeoutOccurred()){
    Serial.print("ERRO: ");
    Serial.print("TIMEOUT");
    Serial.println();
  }
  
}
