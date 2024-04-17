#include "GPIO.hpp"
#include <utility>
#include <vector>

static const std::vector<std::pair<GPIO_TypeDef *, uint8_t>> extiPins{
    std::make_pair(GPIO0, 0),  std::make_pair(GPIO0, 1),
    std::make_pair(GPIO0, 2),  std::make_pair(GPIO0, 3),
    std::make_pair(GPIO0, 6),  std::make_pair(GPIO1, 1),
    std::make_pair(GPIO2, 12), std::make_pair(GPIO0, 11),
    std::make_pair(GPIO0, 14), std::make_pair(GPIO0, 15),
    std::make_pair(GPIO1, 0),  std::make_pair(GPIO2, 7),
    std::make_pair(GPIO1, 10), std::make_pair(GPIO2, 3),
    std::make_pair(GPIO2, 4),  std::make_pair(GPIO2, 15)};

static callback_t gpio_irq_cb[16];

GPIO::GPIO(uint8_t portNumber, uint8_t pinNumber, Mode mode)
    : _portNumber(portNumber), _pinNumber(pinNumber) {

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = pinNumberToPin(pinNumber);
    setMode(mode);
}

static uint8_t pinToInterruptNum(GPIO_TypeDef *port, uint8_t pinNumber) {
    uint8_t num = 0;
    for (auto p : extiPins) {
        if (p.first == port && p.second == pinNumber)
            return num;

        num++;
    }
    return 0xff;
}

void GPIO::setMode(Mode mode) {
    switch (mode) {
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

void GPIO::attachInterrupt(callback_t callback, TriggerEdge edge) {

    uint8_t interruptNum =
        pinToInterruptNum(portNumberToPort(_portNumber), _pinNumber);
    if (interruptNum == 0xff) {
        return;
    }

    EXTI_Trigger_Config(portNumberToPort(_portNumber), _pinNumber,
                        static_cast<EXTIMode_TypeDef>(edge));

    gpio_irq_cb[interruptNum] = callback;

    NVIC_SetPriority(GPIO_IRQn, 6);
    NVIC_EnableIRQ(GPIO_IRQn);
}

void GPIO_IRQHandler(void) {
    for (auto p : extiPins) {
        if (EXTI_GetIRQFlag(p.first, p.second)) {
            EXTI_ClearIRQFlag(p.first, p.second);
            gpio_irq_cb[pinToInterruptNum(p.first, p.second)]();
        }
    }
}