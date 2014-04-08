/* Tester_MAX17043.h
 * Tested with mbed board: FRDM-KL46Z
 * Author: Mark Gottscho
 * mgottscho@ucla.edu
 */
 
#ifndef TESTER_MAX17043_H
#define TESTER_MAX17043_H

#include "mbed.h"
#include "MAX17043.h"
#include "Tester.h"

class Tester_MAX17043 : public Tester {
    public:
        Tester_MAX17043(PinName correct_sda, PinName correct_scl, int correct_i2c_addr);
        ~Tester_MAX17043();
        
        /**
         * This factory static method constructs an object of this class, and runs unit tests on the MAX17043 class.
         */
        static void test_MAX17043();
        
        virtual int testAll();
    private:
        int __testVersion();
        int __testVCell();
        int __testSOC();
        
        PinName __correct_sda;
        PinName __correct_scl;
        int __correct_i2c_addr;
        bool __good_dut;
        
        MAX17043 *__dut;
};

#endif