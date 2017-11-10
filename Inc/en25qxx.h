#ifndef __EN25QXX_H
#define __EN25QXX_H			   
#include "stm32f4xx_hal.h"
#include "common.h"

//EN25Q80  ID  0XEF13
//EN25Q16  ID  0XEF14
//EN25Q32  ID  0XEF15
//EN25Q64  ID  0XEF16	
//EN25Q128 ID  0XEF17	
#define EN25Q80B	0XEF17

extern uint16_t EN25QXX_TYPE;					//����EN25QXXоƬ�ͺ�		   

#define	EN25QXX_CS 		PBout(14)  		//EN25QXX��Ƭѡ�ź�

////////////////////////////////////////////////////////////////////////////////// 
//ָ���
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
uint16_t  EN25QXX_ReadID(void);  	    		//��ȡFLASH ID
uint8_t	 EN25QXX_ReadSR(void);        		//��ȡ״̬�Ĵ��� 
void EN25QXX_Write_SR(uint8_t sr);  			//д״̬�Ĵ���
void EN25QXX_Write_Enable(void);  		//дʹ�� 
void EN25QXX_Write_Disable(void);		//д����
void EN25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void EN25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //��ȡflash
void EN25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//д��flash
void EN25QXX_Erase_Chip(void);    	  	//��Ƭ����
void EN25QXX_Erase_Sector(uint32_t Dst_Addr);	//��������
void EN25QXX_Wait_Busy(void);           	//�ȴ�����
void EN25QXX_PowerDown(void);        	//�������ģʽ
void EN25QXX_WAKEUP(void);				//����
#endif
















