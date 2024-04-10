#include "system_lks32mc07x.h"

#include "GPIO.hpp"
#include "HWSerial.hpp"

#include "config.h"
int main(void)
{
    GPIO u0rx(U0RX_Port, U0RX_Pin, GPIO::INPUT);
    GPIO u0tx(U0TX_Port, U0TX_Pin, GPIO::OUTPUT);

    HWSerial Serial0(UART0, &u0rx, &u0tx);
    Serial0.begin(115200);
    char fuckStr[] = "Fuck The World!";

    while (1)
    {
        static uint8_t a = 0;
        Serial0.printf("Fuck me %d times \n", a++);
        delay1ms(1000);
    }
}
