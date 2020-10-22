#ifndef __jwsTWI_color_tcs34725_h__
#define __jwsTWI_color_tcs34725_h__
/**
* @file color-tcs34725.h
* @brief Implementation of the TCS34725 chip for ARM mbed projects
* @author Jan-Willem Smaal  <usenet@gispen.org>
* @date 7/14/2020 
*/
#include "mbed.h"
#include "platform/mbed_thread.h"

// Not sure if this can be placed inside of the class.  
// static I2C i2c(I2C_SDA0, I2C_SCL0);


/** 
 * Adresses - depends on partnumber
 * TCS34721 and TCS34723 use 0x39
 * TCS34725 and TCS34727 use 0x29
 */
#define TCS3_I2C_SLAVE_ADDRESS 0x52  // (0x29 << 1)
//
#define TCS3_READ_PATTERN       0x53
#define TCS3_WRITE_PATTERN      0x52
//
#define TCS3_COMMAND_REGISTER    0x80


/**
 * Enable Register
 */
enum tcs3_enable_register {
    PON         = 0x01,     // Power ON
    AEN         = 0x02,     // Adc ENable
    WEN         = 0x08,     // Wait ENable
    AIEN        = 0x10      // ADC Interrupt ENable
};


/**
 * RGBC timing Register (ATIME = 0x01)
 */
enum tcs3_atime_register {
    INT_700MS   = 0x00,
    INT_154MS   = 0xC0,
    INT_101MS   = 0xD5,
     INT_24MS   = 0xF6,
     INT_2MS    = 0xFF      // Actually 2.4 ms
};


/**
 * Wait Time Register
 */
enum tcs3_wtime_register {
    T_614MS     = 0x00,
    T_204MS     = 0xAB,
      T_2MS     = 0xFF      // Actually 2.4 ms
};

/** 
 * Configuration Register (0x0D)
 */
#define WLONG   0x02


/**
 * Control Register (0x0F)
 */
enum tcs3_control_register {
     GAIN_1X =  0x00,
     GAIN_4X =  0x01,
    GAIN_16X =  0x10,
    GAIN_60X =  0x11
};


/**
 * register mapping
 */
enum tcs3_register {
    ENABLE      = 0x00,
    ATIME       = 0x01,
    WTIME       = 0x03,
    AILTL       = 0x04,
    AILTH       = 0x05,
    AIHTL       = 0x06,
    AIHTH       = 0x07,
    PERS        = 0x0C,
    CONFIG      = 0x0D,
    CONTROL     = 0x0F,
    ID          = 0x12,
    STATUS      = 0x13,
    
    // Clear
    CDATAL      = 0x14,
    CDATAH      = 0x15,

    // Red
    RDATAL      = 0x16,
    RDATAH      = 0x17,
    
    // Green
    GDATAL      = 0x18,
    GDATAH      = 0x19,
    
    // Blue
    BDATAL      = 0x1A,
    BDATAH      = 0x1B
};


/** Color class 
 * used for the TCS34725 sensor
 */
class Color {
    public:
        /// Constructor 
        Color();
        
        /// Destructor  (powers down the I2C device) 
        ~Color(); 

        
        /// Methods
        uint8_t Id();
        void UpdateCRGB(); 

        /// setters 
        void setC(uint16_t val); 
        void setR(uint16_t val);
        void setG(uint16_t val);
        void setB(uint16_t val);
        void setGain(enum tcs3_control_register reg);
        void setIntegration(enum tcs3_atime_register);
 

        /// getters   (update the class variables c,r,g,b via I2C )
        uint16_t getC();    // ReadClear();
        uint16_t getR();    // ReadRed();
        uint16_t getG();    // ReadGreen();
        uint16_t getB();    // ReadBlue();
        tcs3_control_register getGain();
        tcs3_atime_register getIntegration(); 

    private: 
        // Could be set inside a  TIMER ISR  periodically  
        //uint16_t c;         // Clear
        uint16_t c;
        uint16_t r;         // Red 
        uint16_t g;         // Green     
        uint16_t b;         // Blue 
     
    private:
        uint8_t ReadRegister(enum tcs3_register reg);                       
};

#endif 