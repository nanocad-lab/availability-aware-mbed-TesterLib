/* Tester_MAX17043.cpp
 * Tested with mbed board: FRDM-KL46Z
 * Author: Mark Gottscho
 * mgottscho@ucla.edu
 */
 
#include "mbed.h"
#include <assert.h>
#include "MAX17043.h"
#include "Tester.h"
#include "Tester_MAX17043.h"
#include "SerialManager.h"
#include "util.h"

using namespace std;

extern SerialManager console;
extern DigitalOut green;
extern DigitalOut red;

Tester_MAX17043::Tester_MAX17043(PinName sda, PinName scl, int i2c_addr) :
                                            Tester(),
                                            __correct_sda(sda),
                                            __correct_scl(scl),
                                            __correct_i2c_addr(i2c_addr),
                                            __good_dut(false),
                                            __dut(NULL) {
}

Tester_MAX17043::~Tester_MAX17043() {
    if (__dut != NULL)
        delete __dut;
}

void Tester_MAX17043::test_MAX17043() {
    console.serial.printf("Testing MAX17043...\r\n");
    const uint8_t FUEL_GAUGE_I2C_ADDRESS = 0x6C;
    Tester_MAX17043 tester(PTE0, PTE1, FUEL_GAUGE_I2C_ADDRESS);
    tester.testAll();
    console.serial.printf("...success!\r\n");
}

int Tester_MAX17043::testAll() {
    //Test with known-correct SDA, SCL, I2C info
    __dut = new MAX17043(__correct_sda, __correct_scl, __correct_i2c_addr);
    __good_dut = true;
    
    __testVersion();
    __testVCell();
    __testSOC();
    
    delete __dut;
    
    return 0;
}

int Tester_MAX17043::__testVersion() {
    myAssert(&console.serial, __dut != NULL, __FILE__, __LINE__, &green, &red);
    if (__good_dut == true)
        myAssert(&console.serial, __dut->getVersion() == 3, __FILE__, __LINE__, &green, &red);
    else
        myAssert(&console.serial, __dut->getVersion() == 0, __FILE__, __LINE__, &green, &red);
    return 0;   
}

int Tester_MAX17043::__testVCell() {
    myAssert(&console.serial, __dut != NULL, __FILE__, __LINE__, &green, &red);
    if (__good_dut == true) {
        uint16_t value = __dut->getVCell(true);
        float valuef = __dut->getFloatVCell(false);
        console.serial.printf("...Battery cell voltage: %0.2f V\r\n", valuef);
        myAssert(&console.serial, value < 4096, __FILE__, __LINE__, &green, &red);
        myAssert(&console.serial, valuef >= 0 && valuef <= 5, __FILE__, __LINE__, &green, &red);
    } else {
        uint16_t value = __dut->getVCell(true);
        float valuef = __dut->getFloatVCell(false);
        myAssert(&console.serial, value == 0, __FILE__, __LINE__, &green, &red);
        myAssert(&console.serial, valuef == 0, __FILE__, __LINE__, &green, &red);
    }
    return 0;   
}

int Tester_MAX17043::__testSOC() {
    myAssert(&console.serial, __dut != NULL, __FILE__, __LINE__, &green, &red);
    if (__good_dut == true) {
        uint16_t value = __dut->getSOC(true);
        myAssert(&console.serial, value <= 25600, __FILE__, __LINE__, &green, &red);
        float valuef = __dut->getFloatSOC(false);
        console.serial.printf("...Battery SOC: %0.2f%%\r\n", valuef);
        myAssert(&console.serial, valuef >= 0 && valuef <= 100, __FILE__, __LINE__, &green, &red);
    } else {
        uint16_t value = __dut->getSOC(true);
        myAssert(&console.serial, value == 0, __FILE__, __LINE__, &green, &red);
        float valuef = __dut->getFloatSOC(false);
        myAssert(&console.serial, valuef == 0, __FILE__, __LINE__, &green, &red);
    }
    return 0;   
}