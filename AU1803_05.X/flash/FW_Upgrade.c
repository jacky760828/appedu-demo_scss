#include "MX25L.h"



do{

  								alt_u8 i=0;
  								if((alt_up_rs232_get_used_space_in_read_FIFO(rs232_dev))>=16){
  									while (len--){
  										if(alt_up_rs232_read_data(rs232_dev, &rd_data[i++], &p_error)==0)
  											count++;
  									}
  								}

  								if (count==16){
  									MT25Q_Flash_Write_Enable();
  									MT25Q_Flash_Write(rd_data, ulEStrdrs,16);
  									ulEStrdrs+=0x10;
  									count=0;
  									len=16;
  								}

  								while(!(alt_up_rs232_get_used_space_in_read_FIFO(rs232_dev)));
  /*								if(ulEStrdrs==0)
  									MT25Q_Flash_Write_Enable();
  								MT25Q_Flash_Write(data_Mt25q, ulEStrdrs,1);
  								if(alt_up_rs232_read_data(rs232_dev, data_Mt25q, &p_error)==0){
  								 ulEStrdrs+=1;
  								}
  							//	usleep(5);

  								printf("ulEStrdrs=       %x\n",ulEStrdrs);
  							//	while(!(alt_up_rs232_get_used_space_in_read_FIFO(rs232_dev)));*/
  							}while(ulEStrdrs<0x590000);
