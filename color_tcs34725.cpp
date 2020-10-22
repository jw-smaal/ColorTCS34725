/**
* @file color-tcs34725.cpp
* @brief Implementation of the TCS34725 chip for ARM embed projects
* @author Jan-Willem Smaal  <usenet@gispen.org>
* @date 7/14/2020 
*/
#include "color_tcs34725.h"


I2C i2c(I2C_SDA0, I2C_SCL0);



/** 
 * Color Constructor 
 */
Color::Color(){
    i2c.start ();
    i2c.write(TCS3_WRITE_PATTERN);
    i2c.write(TCS3_COMMAND_REGISTER | ENABLE);
    i2c.write(PON | AEN); // POwer On  and Adc ENable 
    //i2c.write()
    i2c.stop();

    Color::setGain(GAIN_4X);
    Color::setIntegration(INT_700MS);

    return;
}

/** 
 * Color Destructor 
 */
Color::~Color() {
    i2c.start ();
    i2c.write(TCS3_WRITE_PATTERN);
    i2c.write(TCS3_COMMAND_REGISTER | ENABLE);
    i2c.write(~PON); // POwer Off 
    //i2c.write()
    i2c.stop();

    c = 0;
    r = 0;
    g = 0; 
    b = 0; 

    return; 
}

/** Update the class based on the chip registers 
 */
void Color::UpdateCRGB(){
    Color::getC();
    Color::getR();
    Color::getG();
    Color::getB();
}


/** 
 * Class Methods
 */ 
uint8_t Color::Id() {
    uint8_t tmp;
    
    i2c.start();
    i2c.write(TCS3_WRITE_PATTERN);
    i2c.write(TCS3_COMMAND_REGISTER | ID);
    i2c.start();
    i2c.write(TCS3_READ_PATTERN);
    tmp = i2c.read(0);  // Read with no ACK 
    i2c.stop();
    return(tmp);
}

/** 
 * setters 
 */
void Color::setC(uint16_t val){    
    c = val; 
    return; 
}

void Color::setR(uint16_t val){    
    r = val; 
    return; 
}


void Color::setG(uint16_t val){
    g = val; 
    return;
}


void Color::setB(uint16_t val){
    b = val; 
    return;
}

void Color::setGain(enum tcs3_control_register reg){
    i2c.start();
    i2c.write(TCS3_WRITE_PATTERN);
    i2c.write(TCS3_COMMAND_REGISTER | CONTROL);
    i2c.write(reg);
    i2c.stop();
    return; 
}


void Color::setIntegration(enum tcs3_atime_register reg){
    i2c.start();
    i2c.write(TCS3_WRITE_PATTERN);
    i2c.write(TCS3_COMMAND_REGISTER | ATIME);
    i2c.write(reg);
    i2c.stop();
    return; 
}

/* 
 * getters 
 */
uint16_t Color::getC(){
    uint16_t tmp;

    // We read first into tmp to avoid writing nibble's to the public member 
    // which might accidently be read in the middle of this assignment. 
    tmp = Color::ReadRegister(CDATAL);
    tmp = tmp + (Color::ReadRegister(CDATAH)<<8);
    c = tmp;
    return tmp;
}

uint16_t Color::getR(){
    uint16_t tmp;

    // We read first into tmp to avoid writing nibble's to the public member 
    // which might accidently be read in the middle of this assignment. 
    tmp = Color::ReadRegister(RDATAL);
    tmp = tmp + (Color::ReadRegister(RDATAH)<<8);
    r = tmp;
    return tmp;
}

uint16_t Color::getG(){
    uint16_t tmp;

     // We read first into tmp to avoid writing nibble's to the public member 
    // which might accidently be read in the middle of this assignment. 
    tmp = Color::ReadRegister(GDATAL);
    tmp = tmp + (Color::ReadRegister(GDATAH)<<8);
    g = tmp;
    return tmp;
}

uint16_t Color::getB(){
    uint16_t tmp;

    // We read first into tmp to avoid writing nibble's to the public member 
    // which might accidently be read in the middle of this assignment. 
    tmp = Color::ReadRegister(BDATAL);
    tmp = tmp + (Color::ReadRegister(BDATAH)<<8);
    b = tmp;
    return tmp;
}

// TODO: implement 
tcs3_control_register Color::getGain(void){

    return GAIN_1X; 
}

// TODO: implement 
tcs3_atime_register Color::getIntegration(void){
    return INT_700MS;
}


/* 
 * Private methods
 */ 
uint8_t Color::ReadRegister(enum tcs3_register reg){
    uint8_t tmp;
    
    i2c.start();
    i2c.write(TCS3_WRITE_PATTERN);
    i2c.write(TCS3_COMMAND_REGISTER | reg);

    // This delay did the trick.  Need to read the datasheet to see what the timing 
    // Requirements are
    //thread_sleep_for(1);
    ::rtos::ThisThread::sleep_for(1);
    
    i2c.start();
    i2c.write(TCS3_READ_PATTERN);
    tmp = i2c.read(0);
      i2c.stop();
    return tmp;
}


/* EOF  */