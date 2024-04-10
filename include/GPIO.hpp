#pragma once

#include "lks32mc07x_lib.h"

enum class AltFunc
{
    GPIO = 0,
    SysAF,
    Hall,
    MCPWM,
    UART,
    SPI,
    I2C,
    Timer01,
    Timer23,
    Debug,
    CAN,
    SIF,
    CL
};

class GPIO
{
private:
    GPIO_InitTypeDef GPIO_InitStruct;
    uint8_t _portNumber;
    uint8_t _pinNumber;
    AltFunc _afnum = AltFunc::GPIO;

    inline GPIO_TypeDef *portNumberToPort(uint8_t portNumber)
    {
        GPIO_TypeDef *ports[] = {GPIO0, GPIO1, GPIO2, GPIO3};
        return ports[portNumber];
    }

    inline uint16_t pinNumberToPin(uint8_t pinNumber)
    {
        return 1 << pinNumber;
    }

public:
    enum Mode
    {
        OUTPUT,
        OUTPUT_OD,
        INPUT,
        INPUT_PULLUP
    };

    GPIO(uint8_t portNumber, uint8_t pinNumber, Mode mode);

    void setMode(Mode mode);

    inline void write(bool value)
    {
        GPIO_WriteBit(portNumberToPort(_portNumber), pinNumberToPin(_pinNumber), static_cast<BitAction>(value));
    }

    inline bool read()
    {
        return GPIO_InitStruct.GPIO_Mode == GPIO_Mode_OUT ? GPIO_ReadOutputDataBit(portNumberToPort(_portNumber), pinNumberToPin(_pinNumber)) : GPIO_ReadInputDataBit(portNumberToPort(_portNumber), pinNumberToPin(_pinNumber));
    }

    inline void toggle()
    {
        GPIO_WriteBit(portNumberToPort(_portNumber), pinNumberToPin(_pinNumber), !static_cast<bool>(GPIO_ReadOutputDataBit(portNumberToPort(_portNumber), pinNumberToPin(_pinNumber))));
    }

    inline void setAF(AltFunc afnum)
    {
        GPIO_PinAFConfig(portNumberToPort(_portNumber), _pinNumber, static_cast<uint32_t>(afnum));
    }

    inline AltFunc getAF()
    {
        return _afnum;
    }
};