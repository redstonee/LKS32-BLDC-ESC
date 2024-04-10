#include "HWSerial.hpp"
#include <stdarg.h>

HWSerial::HWSerial(UART_TypeDef *uart, GPIO *pin_rx, GPIO *pin_tx)
{
    UART_StructInit(&is);
    _pin_rx = pin_rx;
    _pin_tx = pin_tx;
    is.IRQEna = UART_IRQEna_SendOver | UART_IRQEna_RcvOver;
}

void HWSerial::begin(uint32_t baud)
{
    is.BaudRate = baud;
    _pin_rx->setAF(AltFunc::UART);
    _pin_tx->setAF(AltFunc::UART);
    UART_Init(_uart, &is);
}

void HWSerial::print(long num, Radix radix)
{
    char str[20];
    itoa(num, str, radix);
    print(str);
}

void HWSerial::print(const char str[])
{
    auto len = strlen(str);
    while (len--)
    {
        UART_SendData(_uart, *str++);
        while (!UART_GetIRQFlag(_uart, UART_IF_SendOver))
            ;
        UART_ClearIRQFlag(_uart, UART_IF_SendOver);
    }
}

void HWSerial::printf(const char fmt[], ...)
{
    char buf[128];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);
    print(buf);
}
