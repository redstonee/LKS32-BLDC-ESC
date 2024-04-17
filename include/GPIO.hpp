#pragma once

#include "lks32mc07x_lib.h"
#include <functional>

enum class AltFunc {
    GPIO = 0, // GPIO, Analog
    SysAF,    // Comparator, Clock_Out, SWD
    Hall,
    MCPWM, // Motor_PWM
    UART,
    SPI,
    I2C,
    Timer01, // Timer0, Timer1
    Timer23, // Timer2, Timer3, QEP0, QEP1
    ADC_Trigger,
    CAN,
    SIF,
    CL
};

enum class TriggerEdge {
    None = 0, // No trigger
    Rising,   // Rising edge trigger
    Falling,  // Falling edge trigger
    Both      // Both edges trigger
};

typedef std::function<void(void)> callback_t;

class GPIO {
  private:
    GPIO_InitTypeDef GPIO_InitStruct;
    uint8_t _portNumber;
    uint8_t _pinNumber;
    AltFunc _afnum = AltFunc::GPIO;

    static inline GPIO_TypeDef *portNumberToPort(uint8_t portNumber) {
        GPIO_TypeDef *ports[] = {GPIO0, GPIO1, GPIO2, GPIO3};
        return ports[portNumber];
    }

    static inline uint16_t pinNumberToPin(uint8_t pinNumber) { return 1 << pinNumber; }


  public:
    enum Mode { OUTPUT, OUTPUT_OD, INPUT, INPUT_PULLUP };

    GPIO(uint8_t portNumber, uint8_t pinNumber, Mode mode);

    void setMode(Mode mode);

    inline void write(bool value) {
        GPIO_WriteBit(portNumberToPort(_portNumber), pinNumberToPin(_pinNumber),
                      static_cast<BitAction>(value));
    }

    inline bool read() {
        return GPIO_InitStruct.GPIO_Mode == GPIO_Mode_OUT
                   ? GPIO_ReadOutputDataBit(portNumberToPort(_portNumber),
                                            pinNumberToPin(_pinNumber))
                   : GPIO_ReadInputDataBit(portNumberToPort(_portNumber),
                                           pinNumberToPin(_pinNumber));
    }

    inline void toggle() {
        GPIO_WriteBit(
            portNumberToPort(_portNumber), pinNumberToPin(_pinNumber),
            static_cast<BitAction>(!GPIO_ReadOutputDataBit(
                portNumberToPort(_portNumber), pinNumberToPin(_pinNumber))));
    }

    inline void setAF(AltFunc afnum) {
        GPIO_PinAFConfig(portNumberToPort(_portNumber), _pinNumber,
                         static_cast<uint32_t>(afnum));
    }

    inline AltFunc getAF() { return _afnum; }

    void attachInterrupt(callback_t callback, TriggerEdge edge);
};