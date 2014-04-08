/* Tester_INA219.cpp
 * Tested with mbed board: FRDM-KL46Z
 * Author: Mark Gottscho
 * mgottscho@ucla.edu
 */
 
#include "mbed.h"
#include <assert.h>
#include <stdlib.h>
#include "Tester_INA219.h"
#include "Tester.h"
#include "util.h"
#include "SerialManager.h"

using namespace std;

extern SerialManager console;
extern DigitalOut green;
extern DigitalOut red;

Tester_INA219::Tester_INA219(PinName sda, PinName scl, int i2c_addr, float minBusVoltage, float maxBusVoltage, float minShuntCurrent, float maxShuntCurrent) :
                                            Tester(),
                                            __correct_sda(sda),
                                            __correct_scl(scl),
                                            __correct_i2c_addr(i2c_addr),
                                            __minBusVoltage(minBusVoltage),
                                            __maxBusVoltage(maxBusVoltage),
                                            __minShuntCurrent(minShuntCurrent),
                                            __maxShuntCurrent(maxShuntCurrent),
                                            __dut(NULL) {
}

Tester_INA219::~Tester_INA219() {
    if (__dut != NULL)
        delete __dut;
}

void Tester_INA219::test_INA219() {
    const uint8_t SUPPLY_I2C_ADDRESS = (0x44) << 1;
    const float SUPPLY_VMIN = -0.01;
    const float SUPPLY_VMAX = 5.2;
    const float SUPPLY_IMIN = 0;
    const float SUPPLY_IMAX = 0.6;
    const uint8_t BATTERY_I2C_ADDRESS = (0x40) << 1;
    const float BATTERY_VMIN = 2.5;
    const float BATTERY_VMAX = 4.2;
    const float BATTERY_IMIN = -0.5;
    const float BATTERY_IMAX = 0.5;
    const uint8_t LOAD_I2C_ADDRESS = (0x41) << 1;
    const float LOAD_VMIN = 4.9;
    const float LOAD_VMAX = 5.1;
    const float LOAD_IMIN = 0;
    const float LOAD_IMAX = 0.15;
    
    console.serial.printf("Testing INA219 on supply rail...\r\n");
    Tester_INA219 supply_tester(PTE0, PTE1, SUPPLY_I2C_ADDRESS, SUPPLY_VMIN, SUPPLY_VMAX, SUPPLY_IMIN, SUPPLY_IMAX);
    supply_tester.testAll();
    console.serial.printf("...success!\r\n");

    console.serial.printf("Testing INA219 on battery rail (note that negative power means it is charging)...\r\n");
    Tester_INA219 battery_tester(PTE0, PTE1, BATTERY_I2C_ADDRESS, BATTERY_VMIN, BATTERY_VMAX, BATTERY_IMIN, BATTERY_IMAX);
    battery_tester.testAll();
    console.serial.printf("...success!\r\n");
    
    console.serial.printf("Testing INA219 on load rail...\r\n");
    Tester_INA219 load_tester(PTE0, PTE1, LOAD_I2C_ADDRESS, LOAD_VMIN, LOAD_VMAX, LOAD_IMIN, LOAD_IMAX);
    load_tester.testAll();
    console.serial.printf("...success!\r\n");
}

int Tester_INA219::testAll() {
    //Test with known-correct SDA, SCL, I2C info
    __dut = new INA219(__correct_sda, __correct_scl, __correct_i2c_addr);
    console.serial.printf("...Vmin = %0.2f V\r\n", __minBusVoltage);
    console.serial.printf("...Vmax = %0.2f V\r\n", __maxBusVoltage);
    console.serial.printf("...Imin = %0.2f mA\r\n", __minShuntCurrent*1000);
    console.serial.printf("...Imax = %0.2f mA\r\n", __maxShuntCurrent*1000);
    __dut->selfInit();
    __testInitialConfig();
    __testBusVoltageRange();
    __testShuntAmpGain();
    __dut->selfInit();
    __testShuntVoltage();
    __testBusVoltage();
    __testCurrent();
    __testPower();
    
    delete __dut;
    
    return 0;
}

int Tester_INA219::__testInitialConfig() {
    myAssert(&console.serial, __dut != NULL, __FILE__, __LINE__, &green, &red);
    console.serial.printf("...Initial Config Register...");
    uint16_t value = __dut->getRegister16b(0x00);
    myAssert(&console.serial, value == 0x399F, __FILE__, __LINE__, &green, &red);
    console.serial.printf("OK\r\n");
    return 0;
}

int Tester_INA219::__testBusVoltageRange() {
    myAssert(&console.serial, __dut != NULL, __FILE__, __LINE__, &green, &red);
    console.serial.printf("...Bus Voltage Range...");
    __dut->setBusVoltageRange32V(true);
    myAssert(&console.serial, __dut->isBusVoltageRange32V(), __FILE__, __LINE__, &green, &red);
    __dut->setBusVoltageRange32V(false);
    myAssert(&console.serial, !__dut->isBusVoltageRange32V(), __FILE__, __LINE__, &green, &red);
    console.serial.printf("OK\r\n");
    return 0;   
}

int Tester_INA219::__testShuntAmpGain() {
    myAssert(&console.serial, __dut != NULL, __FILE__, __LINE__, &green, &red);
    console.serial.printf("...Shunt Amp Gain...");
    __dut->setShuntAmpGain(1);
    myAssert(&console.serial, __dut->getShuntAmpGain() == 1, __FILE__, __LINE__, &green, &red);
    __dut->setShuntAmpGain(2);
    myAssert(&console.serial, __dut->getShuntAmpGain() == 2, __FILE__, __LINE__, &green, &red);
    __dut->setShuntAmpGain(4);
    myAssert(&console.serial, __dut->getShuntAmpGain() == 4, __FILE__, __LINE__, &green, &red);
    __dut->setShuntAmpGain(8);
    myAssert(&console.serial, __dut->getShuntAmpGain() == 8, __FILE__, __LINE__, &green, &red);
    __dut->setShuntAmpGain(3);
    myAssert(&console.serial, __dut->getShuntAmpGain() == 8, __FILE__, __LINE__, &green, &red);
    __dut->setShuntAmpGain(4);
    myAssert(&console.serial, __dut->getShuntAmpGain() == 4, __FILE__, __LINE__, &green, &red);
    __dut->setShuntAmpGain(17);
    myAssert(&console.serial, __dut->getShuntAmpGain() == 4, __FILE__, __LINE__, &green, &red);
    console.serial.printf("OK\r\n");
    return 0;   
}

int Tester_INA219::__testShuntVoltage() {
    myAssert(&console.serial, __dut != NULL, __FILE__, __LINE__, &green, &red);
    int16_t value = __dut->getShuntVoltage(true);
    float valuef = __dut->getShuntVoltageFloat(false);
    console.serial.printf("...Shunt voltage: %0.2f mV...", valuef*1000);
    myAssert(&console.serial, abs(valuef) <= 0.06, __FILE__, __LINE__, &green, &red); //60 mV shunt voltage corresponds to 600mA current limit. Just check absolute bounds
    console.serial.printf("OK\r\n");
    return 0;
}

int Tester_INA219::__testBusVoltage() {
    myAssert(&console.serial, __dut != NULL, __FILE__, __LINE__, &green, &red);
    int16_t value = __dut->getBusVoltage(true);
    float valuef = __dut->getBusVoltageFloat(false);
    console.serial.printf("...Bus voltage: %0.2f V...", valuef);
    myAssert(&console.serial, valuef <= __maxBusVoltage && valuef >= __minBusVoltage, __FILE__, __LINE__, &green, &red); //check if bus voltage is out of expected range for this particular INA219 under test
    console.serial.printf("OK\r\n");
    return 0;
}

int Tester_INA219::__testCurrent() {
    myAssert(&console.serial, __dut != NULL, __FILE__, __LINE__, &green, &red);
    int16_t value = __dut->getCurrent(true);
    float valuef = __dut->getCurrentFloat(false);
    console.serial.printf("...Shunt current: %0.2f mA...", valuef*1000);
    myAssert(&console.serial, valuef <= __maxShuntCurrent || valuef >= __minShuntCurrent, __FILE__, __LINE__, &green, &red); //check if shunt current is out of expected range for this particular INA219 under test
    console.serial.printf("OK\r\n");
    return 0;
}

int Tester_INA219::__testPower() {
    myAssert(&console.serial, __dut != NULL, __FILE__, __LINE__, &green, &red);
    int16_t value = __dut->getPower(true);
    float valuef = __dut->getPowerFloat(false);
    float currentf = __dut->getCurrentFloat(false);
    float voltagef = __dut->getBusVoltageFloat(false);
    console.serial.printf("...Power: %0.2f mW\r\n", valuef*1000);
    //myAssert(&console.serial, abs(valuef - (currentf*voltagef)) <= 0.001, __FILE__, __LINE__, &green, &red); //check that power is within 1mW of shunt current * bus voltage (tolerance for float errors and/or sampling delays)
    return 0;
}