

/**
 * main.c
 */
#include "port.h"
#include "SysCtr.h"
#include "dio.h"
#include "i2c.h"
#include "nvic.h"

int main(void)
{
    NVIC_init();
	SysCtr_init();
	PORT_init();
	I2C_init();
	uint8 arr[10]={'S', 'P', 'R', 'I', 'N', 'T', 'S'};
	I2C_writeIB(I2C_Channel_0,arr,10);
	I2C_syncTransmit(I2C_Channel_0, 0x3, I2C_Operation_Write, 0, 0);

	while(1)
	{

	}
	return 0;
}
