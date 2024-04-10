#pragma once

#include "lks32mc07x_lib.h"
#include "GPIO.hpp"

enum Radix
{
    BIN = 2,
    OCT = 8,
    DEC = 10,
    HEX = 16
};

class HWSerial
{
private:
    UART_TypeDef *_uart;
    UART_InitTypeDef is;
    GPIO *_pin_rx;
    GPIO *_pin_tx;

public:
    HWSerial(UART_TypeDef *uart, GPIO *pin_rx, GPIO *pin_tx);
    inline void setRx(GPIO *pin_rx)
    {
        _pin_rx = pin_rx;
    }
    inline void setTx(GPIO *pin_tx)
    {
        _pin_tx = pin_tx;
    }

    void begin(uint32_t baudrate);
    void print(long num, Radix radix = DEC);
    void print(const char str[]);

    void printf(const char fmt[], ...);

    inline void println()
    {
        print("\r\n");
    }

    inline void println(const char str[])
    {
        print(str);
        println();
    }

    inline void println(long num, Radix radix = DEC)
    {
        print(num, radix);
        println();
    }
};