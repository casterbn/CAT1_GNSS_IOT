#include "en25qxx.h" 
#include "spi.h"
#include "usart.h"	

#define  EN25Q_DEBUG    1

#if EN25Q_DEBUG
      #define EN25Q_DBG(format,...)   printf("[EN25Q80B flash DEBUG] %s:%d %s:: "format"\r\n",  __FILE__, __LINE__, __FUNCTION__,##__VA_ARGS__)
#else
     #define EN25Q_DBG(format,...)   
#endif
 
uint16_t EN25QXX_TYPE=EN25Q80B;	//Ĭ����EN25Q128

													 
void EN25QXX_Init(void)
{ 
    /*
      need to read init SPI driver, and CS chipset pin.  SPI clock set to 21M.
      Then read chipset ID to test.
    */
    //read flash chipset ID
    EN25Q_DBG("before EN25QXX_TYPE=%x", EN25QXX_TYPE);
    EN25QXX_TYPE=EN25QXX_ReadID();
    EN25Q_DBG("EN25QXX_TYPE=%x", EN25QXX_TYPE);
}  

uint8_t EN25QXX_ReadSR(void)   
{  
	uint8_t byte=0;   
	EN25QXX_CS(0);                          
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    
	byte=SPI1_ReadWriteByte(0Xff);            
	EN25QXX_CS(1);
	return byte;   
} 


void EN25QXX_Write_SR(uint8_t sr)   
{   
	EN25QXX_CS(0);                           
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   
	SPI1_ReadWriteByte(sr);               
	EN25QXX_CS(1);
}   


void EN25QXX_Write_Enable(void)   
{
	EN25QXX_CS(0);  
    SPI1_ReadWriteByte(W25X_WriteEnable);   
	EN25QXX_CS(1); 
} 


void EN25QXX_Write_Disable(void)   
{  
	EN25QXX_CS(0);                         
    SPI1_ReadWriteByte(W25X_WriteDisable);     
	EN25QXX_CS(1);                          
} 		


uint16_t EN25QXX_ReadID(void)
{
	uint16_t Temp = 0;	  
	EN25QXX_CS(GPIO_PIN_RESET);				    
	SPI1_ReadWriteByte(0x90);    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	EN25QXX_CS(GPIO_PIN_SET);				    
	return Temp;
}   		    





void EN25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   										    
	EN25QXX_CS(0);                           
    SPI1_ReadWriteByte(W25X_ReadData);        
    SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>16)); 
    SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((uint8_t)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ������  
    }
	EN25QXX_CS(1);  				    	      
}  



void EN25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
    EN25QXX_Write_Enable();             
	EN25QXX_CS(0);                           
    SPI1_ReadWriteByte(W25X_PageProgram);     
    SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>16)); 
    SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((uint8_t)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);
	EN25QXX_CS(1);                         
	EN25QXX_Wait_Busy();					
} 

//write as page, page size is 256 bytes
void EN25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; 	    
	if(NumByteToWrite<=pageremain)
           pageremain=NumByteToWrite;
	while(1)
	{	   
		EN25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)
                  break;  //write done
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			 
			if(NumByteToWrite>256)pageremain=256; 
			else pageremain=NumByteToWrite; 	 
		}
	};	    
}


uint8_t EN25QXX_BUFFER[4096];		 
void EN25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * EN25QXX_BUF;	  
   	EN25QXX_BUF=EN25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		EN25QXX_Read(EN25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(EN25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			EN25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				EN25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			EN25QXX_Write_NoCheck(EN25QXX_BUF,secpos*4096,4096);//д����������  

		}else EN25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				
			if(NumByteToWrite>4096)
                        secremain=4096; 
			else 
                       secremain=NumByteToWrite;           
		}	 
	};	 
}


void EN25QXX_Erase_Chip(void)   
{                                   
    EN25QXX_Write_Enable();                
    EN25QXX_Wait_Busy();   
  	EN25QXX_CS(0);                           
    SPI1_ReadWriteByte(W25X_ChipErase);       
	EN25QXX_CS(1);                           
	EN25QXX_Wait_Busy();   				 
}   


void EN25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
 	Dst_Addr*=4096;
    EN25QXX_Write_Enable();          
    EN25QXX_Wait_Busy();   
  	EN25QXX_CS(0);                         
    SPI1_ReadWriteByte(W25X_SectorErase);     
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>16)); 
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((uint8_t)Dst_Addr);  
	EN25QXX_CS(1);                        
    EN25QXX_Wait_Busy();   				
}  


void EN25QXX_Wait_Busy(void)   
{   
	while((EN25QXX_ReadSR()&0x01)==0x01);   
}  


void EN25QXX_PowerDown(void)   
{ 
  	EN25QXX_CS(0);                           
    SPI1_ReadWriteByte(W25X_PowerDown);       
	EN25QXX_CS(1);                                 
    //delay_us(3);                            
}   

void EN25QXX_WAKEUP(void)   
{  
  	EN25QXX_CS(0);                          
    SPI1_ReadWriteByte(W25X_ReleasePowerDown);  
	EN25QXX_CS(1);                         
    //delay_us(3);                            
}   


