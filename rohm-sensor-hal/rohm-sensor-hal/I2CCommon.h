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
#ifndef i2c_common_h
#define i2c_common_h
/*  i2c common functions 
    This interface is the same for Arduino hardware i2c lib Wire, Arduino software i2c lib SoftI2CMaster and mbed hardware lib I2C.
    Choosing between them is done by compile time using flags, for mbed use USE_MBED_HARDWARE_I2C -flag.
    Note: This method doesn't allow multiple i2c busses to be used at same time.
*/
#include "rohm_hal.h"       //types, DEBUG_print*, USE_*_HARDWARE_I2C

bool I2CCommonBegin();
uint8_t read_register(uint8_t sad, uint8_t reg, uint8_t* buf, uint8_t buf_len);
void write_registers(uint8_t sad, uint8_t reg, uint8_t* data, uint8_t data_len);
void write_register(uint8_t sad, uint8_t reg, uint8_t data);
bool change_bits(uint8_t sad, uint8_t reg, uint8_t mask, uint8_t bits);

#endif


