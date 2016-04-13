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
#ifndef ROHM_HAL_H
#define ROHM_HAL_H

/* This file is meant for making same code work on many platforms easier. */
#ifdef __MBED__
    /* Mbed Classic */
    #define USE_MBED_HARDWARE_I2C
    #include "mbed.h"               //types
    #include "I2C.h"                //I2C

    extern Serial pc;
    //Serial pc(USBTX, USBRX); done in main.cpp

    #define rohmhal_delay()         wait()
    #define rohmhal_print(...)      pc.printf(__VA_ARGS__)
    #define rohmhal_printf(...)     pc.printf(__VA_ARGS__)
    #define rohmhal_println(...)    pc.printf(__VA_ARGS__);pc.printf("\r\n")

#elif defined(YOTTA_MODULE_NAME)
    /* Mbed OS */
    #define USE_MBED_HARDWARE_I2C
    #include "mbed-drivers/mbed.h"  //types, Serial, I2C -type, ...
    #include "mbed-drivers/I2C.h"   //I2C

    //get_stdio_serial().baud(115200); done in main.cpp

    #define rohmhal_delay()         wait()
    #define rohmhal_print(...)      printf(__VA_ARGS__)
    #define rohmhal_printf(...)     printf(__VA_ARGS__)
    #define rohmhal_println(...)    printf(__VA_ARGS__);printf("\r\n")

#elif ARDUINO
    /* Arduino */
    //#define USE_ARDUINO_SOFTWARE_I2C
    #define USE_ARDUINO_HARDWARE_I2C
    #include <arduino.h>            //types, Serial
	/* i2c library is included in .cpp because sw library can */

    #define rohmhal_delay(a)        delay(a)
    #define rohmhal_printf(a, ...)  Serial.print(a) //no printf->just display formatting sentence.
    #define rohmhal_print()         Serial.print()
    #define rohmhal_println()       Serial.println()
#else
    #error "rohm_hal.h: Not valid target."
#endif

#if 0       //Enable/disable debug prints
    #undef rohmhal_print
    #undef rohmhal_printf
    #undef rohmhal_println
    #define rohmhal_print(...)
    #define rohmhal_printf(...)
    #define rohmhal_println(...)
#endif

#endif /* ROHM_HAL_H */
