#include "GPIO.hpp"

GPIO::GPIO(uint8_t portNumber, uint8_t pinNumber, Mode mode) : _portNumber(portNumber), _pinNumber(pinNumber)
{
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = pinNumberToPin(pinNumber);
    setMode(mode);
}

void GPIO::setMode(Mode mode)
{
    switch (mode)
    {
    case OUTPUT_OD:
        GPIO_InitStruct.GPIO_PODEna = ENABLE;
    case OUTPUT:
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
        break;

    case INPUT_PULLUP:
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    case INPUT:
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
        break;
    }
    GPIO_Init(portNumberToPort(_portNumber), &GPIO_InitStruct);
}