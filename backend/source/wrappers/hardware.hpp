#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <bcm2835.h>

namespace Hardware {
    enum {
        UseSPI = true,
        UseIIC = false,

        // SPI_MOSI = SPI_MOSI,
        // SPI_SCK = SPI_SCK
        // I2C_SCL = I2C_SCL,
        // I2C_SDA = I2C_SDA,

        CS  = 8,
        RST = 25,
        DC  = 24,
    };

    inline void CS0()
    {
        bcm2835_gpio_write(CS, LOW);
    }

    inline void CS1()
    {
        bcm2835_gpio_write(CS, HIGH);
    }

    inline void RST0()
    {
        bcm2835_gpio_write(RST, LOW);
    }

    inline void RST1()
    {
        bcm2835_gpio_write(RST, HIGH);
    }

    inline uint8_t GetRST()
    {
        return bcm2835_gpio_lev(RST);
    }

    inline void DC0()
    {
        bcm2835_gpio_write(DC, LOW);
    }

    inline void DC1()
    {
        bcm2835_gpio_write(DC, HIGH);
    }

    inline void EnablePin(uint8_t pin)
    {
        bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    }

    inline uint8_t ReadPin(uint8_t pin)
    {
        return bcm2835_gpio_lev(pin);
    }

    bool Init();

    inline void Exit()
    {
        if (Hardware::UseSPI) {
            bcm2835_spi_end();
        }
        else {
            bcm2835_i2c_end();
        }
        bcm2835_close();
    }

    inline void SPIWriteByte(uint8_t value)
    {
        bcm2835_spi_transfer(value);
    }

    inline void SPIWriteBytes(char *buf, uint32_t len)
    {
        bcm2835_spi_transfern(buf, len);
    }

    inline bool I2CWriteByte(uint8_t value, uint8_t command)
    {
        char buf[2] = { command, value };

        return bcm2835_i2c_write(buf, 2) == 0;
    }

    inline void DelayMS(uint32_t ms)
    {
        bcm2835_delay(ms);
    }
    inline void DelayUS(uint32_t us)
    {
        bcm2835_delayMicroseconds(us);
    }
} // namespace Hardware

#endif