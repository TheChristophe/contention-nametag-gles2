#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#ifdef DEV_MODE
#include <cstdint>
#else
#include <bcm2835.h>
#endif

namespace Hardware {
    enum {
#ifdef USE_IIC
        UseSPI = false,
        UseIIC = true,
#else
        UseSPI = true,
        UseIIC = false,
#endif

        CS  = 8,
        RST = 25,
        DC  = 24,
    };

    inline void CS0()
    {
#ifndef DEV_MODE
        bcm2835_gpio_write(CS, LOW);
#endif
    }

    inline void CS1()
    {
#ifndef DEV_MODE
        bcm2835_gpio_write(CS, HIGH);
#endif
    }

    inline void RST0()
    {
#ifndef DEV_MODE
        bcm2835_gpio_write(RST, LOW);
#endif
    }

    inline void RST1()
    {
#ifndef DEV_MODE
        bcm2835_gpio_write(RST, HIGH);
#endif
    }

    inline uint8_t GetRST()
    {
#ifndef DEV_MODE
        return bcm2835_gpio_lev(RST);
#else
        return 0;
#endif
    }

    inline void DC0()
    {
#ifndef DEV_MODE
        bcm2835_gpio_write(DC, LOW);
#endif
    }

    inline void DC1()
    {
#ifndef DEV_MODE
        bcm2835_gpio_write(DC, HIGH);
#endif
    }

    inline void EnablePin(uint8_t pin)
    {
#ifndef DEV_MODE
        bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
#endif
    }

    inline uint8_t ReadPin(uint8_t pin)
    {
#ifndef DEV_MODE
        return bcm2835_gpio_lev(pin);
#else
        return 0;
#endif
    }

    bool Init();

    inline void Exit()
    {
#ifndef DEV_MODE
        if (Hardware::UseSPI) {
            bcm2835_spi_end();
        }
        else {
            bcm2835_i2c_end();
        }
        bcm2835_close();
#endif
    }

    inline void SPIWriteByte(uint8_t value)
    {
#ifndef DEV_MODE
        bcm2835_spi_transfer(value);
#endif
    }

    inline void SPIWriteBytes(char *buf, uint32_t len)
    {
#ifndef DEV_MODE
        bcm2835_spi_transfern(buf, len);
#endif
    }

    inline bool I2CWriteByte(uint8_t value, uint8_t command)
    {
#ifndef DEV_MODE
        char buf[2] = { command, value };

        return bcm2835_i2c_write(buf, 2) == BCM2835_I2C_REASON_OK;
#else
        return true;
#endif
    }

    inline void DelayMS(uint32_t ms)
    {
#ifndef DEV_MODE
        bcm2835_delay(ms);
#endif
    }
    inline void DelayUS(uint32_t us)
    {
#ifndef DEV_MODE
        bcm2835_delayMicroseconds(us);
#endif
    }
} // namespace Hardware

#endif