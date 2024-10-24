#include "bluetooth.h"
#include "usart.h"

void Bluetooth_Init(void)
{
	USART1_Init(9600);
}

