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
#include "../rohm-sensor-hal/rohm_hal.h"           //types, USE_MBED_HARDWARE_I2C, rohmhal_print*, I2C.h, I2C_SDA, I2C_SCL
#ifdef USE_MBED_HARDWARE_I2C
#include "../rohm-sensor-hal/I2CCommon.h"          //prototypes
#define I2C_WRITE 0
#define I2C_READ  1
I2C i2c(I2C_SDA, I2C_SCL);


//Note that I2CCommonBegin() must be called before using read/write functions.
bool I2CCommonBegin(){
    return( true );        //always succeeds
}

/* i2c common functions */
uint8_t read_register(uint8_t sad, uint8_t reg, uint8_t* buf, uint8_t buf_len) {
    uint8_t received_bytes;
    int read_ok;

    i2c.write( (int)((sad << 1) | I2C_WRITE), (char*)&reg, (int)1 );
    read_ok = i2c.read( (int)((sad << 1) | I2C_READ), (char*)buf, (int)buf_len);

    if( read_ok == 0 ){     //0 == success(ack)
        received_bytes = buf_len;
        }
    else{                   //non0 == fail (nack)
        received_bytes = 0;
        }
    return( received_bytes );
}

void write_registers(uint8_t sad, uint8_t reg, uint8_t* data, uint8_t data_len) {
    i2c.write( (int)((sad << 1) | I2C_WRITE ), (char*)&reg, (int)1, true);
    i2c.write( (int)((sad << 1) | I2C_WRITE ), (char*)data, (int)data_len, false);
}

void write_register(uint8_t sad, uint8_t reg, uint8_t data) {
    char data_to_send[2];

    data_to_send[0] = reg;
    data_to_send[1] = data;
    i2c.write( (int)((sad << 1) | I2C_WRITE ), &data_to_send[0], 2);
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

#endif /* USE_MBED_HARDWARE_I2C */


