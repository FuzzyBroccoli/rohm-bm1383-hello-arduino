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
#ifndef bm1383_driver_h
#define bm1383_driver_h

#include "../../rohm-sensor-hal/rohm-sensor-hal/rohm_hal.h"         //types

/* bm1383 driver*/
uint8_t bm1383_readId();
void bm1383_wait_until_found();
void bm1383_mode_poweroff2reset();
void bm1383_mode_reset2poweroff();
void bm1383_mode_reset2standby();
void bm1383_mode_standby2reset();
uint8_t bm1383_start_measurement_oneshot_old();
bool bm1383_start_measurement_oneshot();
bool bm1383_start_measurement_continuous(uint8_t continuous_mode);
bool bm1383_stop_measurement();
float bm1383_read_pressure();
float bm1383_pressure_conversion(uint8_t highB, uint8_t lowB, uint8_t leastB);
bool read_drdy_reg();
void bm1383_soft_reset();

void bm1383_set_low_treshold(uint16_t value);
void bm1383_set_high_treshold(uint16_t value);
bool bm1383_enable_treshold_interrupts();
bool bm1383_disable_treshold_interrupts();
bool bm1383_enable_interrupt_latching();
bool bm1383_disable_interrupt_latching();
bool bm1383_enable_interrupt_pullup();
bool bm1383_disable_interrupt_pullup();
void bm1383_clear_interrupt();
bool bm1383_is_treshold_high_crossed();
bool bm1383_is_treshold_low_crossed();

#endif
