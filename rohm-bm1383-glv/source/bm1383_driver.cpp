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
#include "../../rohm-sensor-hal/rohm-sensor-hal/rohm_hal.h"         //types, rohmhal_print*
#include "../../rohm-sensor-hal/rohm-sensor-hal/I2CCommon.h"        //read_register, write_register, change_bits

#include "../rohm-bm1383-glv/bm1383glv.h"      //BM1383_* register definitions
#include "../rohm-bm1383-glv/bm1383_driver.h"
#define SAD 0x5d


/* bm1383 driver*/

uint8_t bm1383_readId(){
  uint8_t id;
  uint8_t read_bytes;

  read_bytes = read_register(SAD, BM1383GLV_ID_REG, &id, 1);
  if ( read_bytes > 0 ){
    rohmhal_printf("Id: %d\n\r", id);
    return(id);
    }
  else{
    rohmhal_printf("Sorry, ID read failed.\n\r");
    return 255;
    }
}

void bm1383_wait_until_found(){
  uint8_t id;

  id = bm1383_readId();
  while (id == 255){
    rohmhal_delay(100);
    id = bm1383_readId();
    }
  return;
  }

// bm1383_mode_* -functions, modes: poweroff-reset(==sleep)-standby. Configuration can only be done in standby-mode.
// For further details please refer specification diagram: "Operation mode transition"
void bm1383_mode_poweroff2reset(){
  write_register(SAD, BM1383GLV_POWER_REG, BM1383GLV_POWER_REG_POWER_UP );
}
void bm1383_mode_reset2poweroff(){
  write_register(SAD, BM1383GLV_POWER_REG, BM1383GLV_POWER_REG_POWER_DOWN );
}
void bm1383_mode_reset2standby(){
  write_register(SAD, BM1383GLV_SLEEP_REG, BM1383GLV_SLEEP_REG_SLEEP_OFF );
}
void bm1383_mode_standby2reset(){
  write_register(SAD, BM1383GLV_SLEEP_REG, BM1383GLV_SLEEP_REG_SLEEP_ON );
}

bool bm1383_start_measurement_oneshot(){
  return change_bits(SAD, BM1383GLV_MODE_CONTROL_REG, BM1383GLV_MODE_CONTROL_REG_MODE_MASK, BM1383GLV_MODE_CONTROL_REG_MODE_ONE_SHOT);
}

/* input parameter continuous_mode: BM1383GLV_MODE_CONTROL_REG_MODE_X
   return: success true/fail */
bool bm1383_start_measurement_continuous(uint8_t continuous_mode){
  if( (continuous_mode == BM1383GLV_MODE_CONTROL_REG_MODE_50MS) ||
      (continuous_mode == BM1383GLV_MODE_CONTROL_REG_MODE_100MS) ||
      (continuous_mode == BM1383GLV_MODE_CONTROL_REG_MODE_200MS) ){
    return change_bits(SAD, BM1383GLV_MODE_CONTROL_REG, BM1383GLV_MODE_CONTROL_REG_MODE_MASK, continuous_mode);
    }
  else{
    rohmhal_printf("Wrong continuous mode.\n\r");
    return false;
    }
}

bool bm1383_stop_measurement(){   // return to standby mode
  return change_bits(SAD, BM1383GLV_MODE_CONTROL_REG, BM1383GLV_MODE_CONTROL_REG_MODE_MASK, BM1383GLV_MODE_CONTROL_REG_MODE_STANDBY);
}

float bm1383_read_pressure(){
    uint8_t incoming[3]= {0,0,0};
    float si_converted;
    uint8_t read_bytes;

    read_bytes = read_register(SAD, BM1383GLV_PRESSURE_OUT_MSB, &incoming[0], 3);
    if (read_bytes > 2){
        si_converted = bm1383_pressure_conversion(incoming[0], incoming[1], incoming[2]);
        }
    else {
        rohmhal_printf("Not enough bytes from pressure registers.\n\r");
        si_converted = 0;
        }
    return si_converted;
}

float bm1383_pressure_conversion(uint8_t highB, uint8_t lowB, uint8_t leastB){
    float BM1383_Var = 0;
    float BM1383_Deci = 0;
    float BM1383_Pres_Conv_Out = 0;

    BM1383_Var  = (highB<<3) | (lowB >> 5);
    BM1383_Deci = ((lowB & 0x1f) << 6 | ((leastB >> 2)));
    BM1383_Deci = BM1383_Deci * 0.00048828125f;      //0.00048828125 = 2^-11
    BM1383_Pres_Conv_Out = (BM1383_Var + BM1383_Deci);
    return BM1383_Pres_Conv_Out;
    }

/* return: true, when new data is available. */
bool read_drdy_reg(){
    return false;
    }

void bm1383_soft_reset(){
    write_register(SAD, BM1383GLV_RESET_CONTROL_REG, (BM1383GLV_RESET_CONTROL_REG_SW_RESET_EXECUTE | BM1383GLV_RESET_CONTROL_REG_INT_RESET_INACTIVE) );
}

void bm1383_set_high_treshold(uint16_t value){
    write_register(SAD, BM1383GLV_INT_HIGH_TRESHOLD_MSB, (value >> 8)   );
    write_register(SAD, BM1383GLV_INT_HIGH_TRESHOLD_LSB, (value & 0xff) );
    return;        //always succeed
}

void bm1383_set_low_treshold(uint16_t value){
    write_register(SAD, BM1383GLV_INT_LOW_TRESHOLD_MSB, (value >> 8)   );
    write_register(SAD, BM1383GLV_INT_LOW_TRESHOLD_LSB, (value & 0xff) );
    return;        //always succeed
}

bool bm1383_enable_treshold_interrupts(){
    return change_bits( SAD,
                        BM1383GLV_INT_CONTROL_REG,
                        (BM1383GLV_INT_CONTROL_REG_INT_HIGH_MASK | BM1383GLV_INT_CONTROL_REG_INT_LOW_MASK | BM1383GLV_INT_CONTROL_REG_INTERRUPT_MASK),
                        (BM1383GLV_INT_CONTROL_REG_INT_HIGH_ENABLE | BM1383GLV_INT_CONTROL_REG_INT_LOW_ENABLE | BM1383GLV_INT_CONTROL_REG_INTERRUPT_ENABLE)
                        );
}

bool bm1383_disable_treshold_interrupts(){
    return change_bits( SAD,
                        BM1383GLV_INT_CONTROL_REG,
                        (BM1383GLV_INT_CONTROL_REG_INT_HIGH_MASK | BM1383GLV_INT_CONTROL_REG_INT_LOW_MASK | BM1383GLV_INT_CONTROL_REG_INTERRUPT_MASK),
                        (BM1383GLV_INT_CONTROL_REG_INT_HIGH_DISABLE | BM1383GLV_INT_CONTROL_REG_INT_LOW_DISABLE | BM1383GLV_INT_CONTROL_REG_INTERRUPT_DISABLE)
                        );
}

bool bm1383_enable_interrupt_latching(){
    return change_bits(SAD, BM1383GLV_INT_CONTROL_REG, BM1383GLV_INT_CONTROL_REG_INTERRUPT_STATE_MASK, BM1383GLV_INT_CONTROL_REG_INTERRUPT_STATE_KEEP_UNTIL_CLEARED );
}

bool bm1383_disable_interrupt_latching(){
    return change_bits(SAD, BM1383GLV_INT_CONTROL_REG, BM1383GLV_INT_CONTROL_REG_INTERRUPT_STATE_MASK, BM1383GLV_INT_CONTROL_REG_INTERRUPT_STATE_CONTINUOUS_UPDATE );
}

bool bm1383_enable_interrupt_pullup(){
    return change_bits(SAD, BM1383GLV_INT_CONTROL_REG, BM1383GLV_INT_CONTROL_REG_INT_PULLUP_MASK, BM1383GLV_INT_CONTROL_REG_INT_PULLUP_ENABLE );
}

bool bm1383_disable_interrupt_pullup(){
    return change_bits(SAD, BM1383GLV_INT_CONTROL_REG, BM1383GLV_INT_CONTROL_REG_INT_PULLUP_MASK, BM1383GLV_INT_CONTROL_REG_INT_PULLUP_DISABLE );
}

void bm1383_clear_interrupt(){
    /* Clears interrupt when in latched int mode. No effect when latched mode is off. */
    write_register(SAD, BM1383GLV_RESET_CONTROL_REG, (BM1383GLV_RESET_CONTROL_REG_INT_RESET_INACTIVE | BM1383GLV_RESET_CONTROL_REG_SW_RESET_NONE) );
}

bool bm1383_is_treshold_high_crossed(){
    uint8_t value;
    read_register(SAD, BM1383GLV_INT_CONTROL_REG, &value, 1);
    return ( (value & BM1383GLV_INT_CONTROL_REG_TRESHOLD_HIGH_MASK) == BM1383GLV_INT_CONTROL_REG_TRESHOLD_HIGH_CROSSED );
}

bool bm1383_is_treshold_low_crossed(){
    uint8_t value;
    read_register(SAD, BM1383GLV_INT_CONTROL_REG, &value, 1);
    return ( (value & BM1383GLV_INT_CONTROL_REG_TRESHOLD_LOW_MASK) == BM1383GLV_INT_CONTROL_REG_TRESHOLD_LOW_CROSSED );
}

