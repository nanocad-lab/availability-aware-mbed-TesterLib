/* Tester.h
 * Tested with mbed board: FRDM-KL46Z
 * Author: Mark Gottscho
 * mgottscho@ucla.edu
 */
 
#ifndef TESTER_H
#define TESTER_H

#include "mbed.h"

/**
* This class is intended for unit test routines. Test classes should inherit from this.
*/
class Tester {
    public:
        Tester();
        ~Tester();
        
        /**
         * Run all unit tests.
         * @returns 0 on success.
         */
        virtual int testAll() = 0;
};

#endif