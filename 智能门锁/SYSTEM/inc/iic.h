#ifndef __IIC_H__
#define __IIC_H__

#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "sys.h"

#define SDA_OUT  PBout(9)
#define SDA_IN   PBin(9)
#define SCL_OUT  PBout(8)

uint8_t software_iic_recvAck(   void);
void software_iic_sendAck( uint8_t ack);
uint8_t software_iic_recvbyte( void);
void software_iic_sendbyte( uint8_t val);
void software_iic_stop( void);
void software_iic_start( void);
void software_iic_sda_mode( GPIOMode_TypeDef GPIO_Mode);
void software_iic_init( void);


#endif

