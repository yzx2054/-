#ifndef __SYS_H__
#define __SYS_H__

//位带操作,实现51类似的GPIO控制功能
//IO口操作宏定义
//      寄存器的位带别名 = 0x42000000 + （寄存器的地址-0x40000000）*32      + 引脚编号*4
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO口地址映射

//GPIO端口  输出数据寄存器  --  设置输出高低电平的作用
#define GPIOA_ODR_Addr    (GPIOA_BASE+0x14) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+0x14) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+0x14) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+0x14) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+0x14) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+0x14) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+0x14) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+0x14) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+0x14) //0x40022014     


//GPIO端口  输入数据寄存器 -- 获取输入高电平状态的作用
#define GPIOA_IDR_Addr    (GPIOA_BASE+0x10) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+0x10) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+0x10) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+0x10) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+0x10) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+0x10) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+0x10) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+0x10) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+0x10) //0x40022010 
 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入

#endif
