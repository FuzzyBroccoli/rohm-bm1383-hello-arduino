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
#include "../rohm-sensor-hal/rohm_hal.h"           //types, USE_ARDUINO_HARDWARE_I2C, rohmhal_print*
#ifdef USE_ARDUINO_HARDWARE_I2C
#include "../rohm-sensor-hal/I2CCommon.h"          //prototypes
#include <Wire.h>               //i2c Wire

//Note that I2CCommonBegin() must be called in .ino setup() -function before using these functions.
bool I2CCommonBegin(){
    Wire.begin();
    return( true );        //always succeeds
}

/* i2c common functions */
uint8_t read_register(uint8_t sad, uint8_t reg, uint8_t* buf, uint8_t buf_len) {
      int incoming;
      uint8_t received_bytes;

      Wire.beginTransmission(sad);
      Wire.write(reg);
      Wire.endTransmission();

      Wire.requestFrom(sad, (uint8_t) buf_len, (uint8_t) true);
      received_bytes = 0;
      while( Wire.available() && ( buf_len > received_bytes ) ){
            incoming = Wire.read();
            buf[received_bytes] = (uint8_t) ( incoming & 0xff );
            received_bytes++;
            }
      return( received_bytes );
}

void write_registers(uint8_t sad, uint8_t reg, uint8_t* data, uint8_t data_len) {
    Wire.beginTransmission(sad);
    Wire.write(reg);
    Wire.write(data, data_len);
    Wire.endTransmission();
}

void write_register(uint8_t sad, uint8_t reg, uint8_t data) {
    Wire.beginTransmission(sad);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

bool change_bits(uint8_t sad, uint8_t reg, uint8_t mask, uint8_t bits){
    uint8_t value, read_bytes;
    read_bytes = read_register(sad, reg, &value, 1);
    if( read_bytes != 0 ){
        value = value & ~mask;
        value = value | (bits & mask);
        write_register(sad, reg, value);
        return true;
        }
    else{
        //rohmhal_printf("Read before change_bits() failed.");
        return false;
        }
}

#endif /* USE_ARDUINO_HARDWARE_I2C */
