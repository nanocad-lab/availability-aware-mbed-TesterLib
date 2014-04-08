/* Tester_INA219.h
 * Tested with mbed board: FRDM-KL46Z
 * Author: Mark Gottscho
 * mgottscho@ucla.edu
 */
 
#ifndef TESTER_INA219_H
#define TESTER_INA219_H

#include "mbed.h"
#include "INA219.h"
#include "Tester.h"

class Tester_INA219 : public Tester {
    public:
        Tester_INA219(PinName correct_sda, PinName correct_scl, int correct_i2c_addr, float minBusVoltage, float maxBusVoltage, float minShuntCurrent, float maxShuntCurrent);
        ~Tester_INA219();
        
        /**
         * This factory static method constructs an object of this class, and runs unit tests on the INA219 class.
         */
        static void test_INA219();
        
        virtual int testAll();
    private:
        int __testInitialConfig();
        int __testBusVoltageRange();
        int __testShuntAmpGain();
        int __testShuntVoltage();
        int __testBusVoltage();
        int __testCurrent();
        int __testPower();
        
        PinName __correct_sda;
        PinName __correct_scl;
        int __correct_i2c_addr;
        
        float __minBusVoltage;
        float __maxBusVoltage;
        float __minShuntCurrent;
        float __maxShuntCurrent;
        
        INA219 *__dut;
};

#endif