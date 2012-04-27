#include <Hardware/Uart/Uart.h>
int main(void)
{
	DEBUG_Init(115200);

    while(1)
    {
    	DEBUG_Send("Hello World!\n");
    }
}
