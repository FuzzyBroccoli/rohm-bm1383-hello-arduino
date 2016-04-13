/*   Copyright 2016 Rohm Semiconductor

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
// BM1383 pressure sensor demo for Arduino Uno using Rohm PCB0058 rev.B i2c breakout shield (Japan-shield).

#include "rohm-sensor-hal/rohm-sensor-hal/rohm_hal.h"       //types, rohmhal_print*
#include "rohm-sensor-hal/rohm-sensor-hal/I2CCommon.h"      //read_register, write_register, change_bits, USE_ARDUINO_SOFTWARE_I2C/USE_ARDUINO_HARDWARE_I2C

#include "rohm-bm1383-glv/rohm-bm1383-glv/bm1383glv.h"      //BM1383_* register definitions
#include "rohm-bm1383-glv/rohm-bm1383-glv/bm1383_driver.h"  //bm1383_* commands

//#define INTPIN PD2                  //Rohm JAPAN-shield int0
#define INTPIN PD3                  //Rohm JAPAN-shield int1
//#define INTPIN PD7                  //Rohm US-shield BM1383-int


void printIntPinStatus(){
    if (digitalRead(INTPIN) == HIGH){
        Serial.println("Arduino NO  INT (high)");
        }
    else{
        Serial.println("Arduino YES INT (low)");
        }
}

void printTreshodRegStatus(){
    if(bm1383_is_treshold_high_crossed()){
        Serial.println("TresHighReg INT");
        }
    else{
        Serial.println("TresHighReg NO INT");
        }
    if(bm1383_is_treshold_low_crossed()){
        Serial.println("TresLowReg  INT");
        }
    else{
        Serial.println("TresLowReg  NO INT");
        }
  }

void waitActiveLowInterrupt(int pin){
    int i;
    Serial.print("  Waiting INT on Arduino");
    for( i = 0 ; (i < 100) ; i++){
        if (digitalRead(pin) == LOW){
            Serial.println(" - got INT.");
            break;
            }
        Serial.print("*");
        delay(1); //wait
        }
    if (i == 100){
        Serial.println(" - NO INT in 100. Pause 10sek");
        delay(10000);
    }
}

void waitActiveLowInterruptClear(int pin){
    int i;
    Serial.print("  Waiting clear");
    for( i = 0 ; (i < 100) ; i++){
        if (digitalRead(pin) == HIGH){
            Serial.println(" - got clear.");
            break;
            }
        Serial.print("*");
        delay(1); //wait
        }
    if (i == 100){
        Serial.println(" - NO Clear in 100. Pause 10sek");
        delay(10000);
    }
}
   









void setup() {
    Serial.begin(115200);
    Serial.println("\n\n\n-------------------------------------------------------\nReboot\nStarted the serial communication.");

    I2CCommonBegin();
    bm1383_wait_until_found();
    Serial.println("\nSensor found.");
    bm1383_mode_poweroff2reset();
    delay(1);
    bm1383_mode_reset2standby();

    //setup bm1383 if needed
    bm1383_set_low_treshold(100);
    bm1383_enable_interrupt_latching();
    //bm1383_disable_interrupt_pullup();  //Pullup doesn't work, int line stays low(triggered).
    bm1383_enable_treshold_interrupts();
    bm1383_clear_interrupt();

    //setup Arduino interrupt pins
    pinMode(INTPIN, INPUT);
printTreshodRegStatus();
printIntPinStatus();
    Serial.println("Setup done. Now finding treshold value of current Pressure.");
  }


void bm1383_read_previous_measurement_and_start_new_one(){
    float pressure;
    static uint16_t treshold = 0;
  
    waitActiveLowInterrupt(INTPIN);

    pressure = bm1383_read_pressure();
    Serial.print("Pressure: "); Serial.println(pressure);

    Serial.println("Clear int command");
    bm1383_clear_interrupt();
    waitActiveLowInterruptClear(INTPIN);

    treshold = treshold + 1000;    
    Serial.print("Treshold: "); Serial.println(treshold);
    Serial.print("\nStart meas. - \n");
    bm1383_set_high_treshold(treshold);
    bm1383_start_measurement_oneshot();
}

void loop() {
    bm1383_read_previous_measurement_and_start_new_one();
    delay(10);
    }

