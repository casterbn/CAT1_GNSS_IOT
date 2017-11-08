#ifndef __EN25QXX_H
#define __EN25QXX_H			    
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//EN25QXX 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//W25X系列/Q系列芯片列表	   
//EN25Q80  ID  0XEF13
//EN25Q16  ID  0XEF14
//EN25Q32  ID  0XEF15
//EN25Q64  ID  0XEF16	
//EN25Q128 ID  0XEF17	
#define EN25Q80 	0XEF13 	
#define EN25Q16 	0XEF14
#define EN25Q32 	0XEF15
#define EN25Q64 	0XEF16
#define EN25Q128	0XEF17

extern uint16_t EN25QXX_TYPE;					//定义EN25QXX芯片型号		   

#define	EN25QXX_CS 		PBout(14)  		//EN25QXX的片选信号

////////////////////////////////////////////////////////////////////////////////// 
//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

void EN25QXX_Init(void);
uint16_t  EN25QXX_ReadID(void);  	    		//读取FLASH ID
uint8_t	 EN25QXX_ReadSR(void);        		//读取状态寄存器 
void EN25QXX_Write_SR(uint8_t sr);  			//写状态寄存器
void EN25QXX_Write_Enable(void);  		//写使能 
void EN25QXX_Write_Disable(void);		//写保护
void EN25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void EN25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //读取flash
void EN25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//写入flash
void EN25QXX_Erase_Chip(void);    	  	//整片擦除
void EN25QXX_Erase_Sector(uint32_t Dst_Addr);	//扇区擦除
void EN25QXX_Wait_Busy(void);           	//等待空闲
void EN25QXX_PowerDown(void);        	//进入掉电模式
void EN25QXX_WAKEUP(void);				//唤醒
#endif
















