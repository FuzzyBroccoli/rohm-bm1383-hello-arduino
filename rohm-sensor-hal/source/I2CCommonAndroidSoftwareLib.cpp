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
#include "../rohm-sensor-hal/rohm_hal.h"           //types, USE_ARDUINO_SOFTWARE_I2C, rohmhal_print*
#ifdef USE_ARDUINO_SOFTWARE_I2C
#include "../rohm-sensor-hal/I2CCommon.h"          //prototypes

#define SCL_PIN 5  //A5         //Note: Download and install the "SoftI2CMaster" as alternative for "Wire".
#define SCL_PORT PORTC
#define SDA_PIN 4  //A4         //References:
#define SDA_PORT PORTC          //  http://playground.arduino.cc/Main/SoftwareI2CLibrary
#define I2C_TIMEOUT 1000        // Sets Clock Stretching up to 1sec
#define I2C_FASTMODE 1          // Sets 400kHz operating speed
#include <SoftI2CMaster.h>      //  https://github.com/felias-fogg/SoftI2CMaster

//Note that I2CCommonBegin() must be called in .ino setup() -function before using read/write functions.
bool I2CCommonBegin(){
    bool I2cOk = i2c_init();
    return( I2cOk );
}

/* i2c common functions */
uint8_t read_register(uint8_t sad, uint8_t reg, uint8_t* buf, uint8_t buf_len) {
    int incoming;
    uint8_t received_bytes;

    i2c_start( (sad << 1) | I2C_WRITE );
    i2c_write(reg);
    i2c_stop();

    received_bytes = 0;
    while( (buf_len-1) > received_bytes ){
        incoming = i2c_read( false );       //Read, but don't NAK(?)
        buf[received_bytes] = (uint8_t) ( incoming & 0xff );
        received_bytes++;
        }
    incoming = i2c_read( true );            //Read last and NAK(?)
    buf[received_bytes] = (uint8_t) ( incoming & 0xff );
    received_bytes++;
    return( received_bytes );
}

void write_registers(uint8_t sad, uint8_t reg, uint8_t* data, uint8_t data_len) {
    i2c_start( (sad << 1) | I2C_WRITE );
    i2c_write(reg);
    while( data_len ){
        i2c_write(*data);
        data++;
        data_len--;
        }
    i2c_stop();
}

void write_register(uint8_t sad, uint8_t reg, uint8_t data) {
    i2c_start( (sad << 1) | I2C_WRITE );
    i2c_write(reg);
    i2c_write(data);
    i2c_stop();
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

#endif /* USE_ARDUINO_SOFTWARE_I2C */
