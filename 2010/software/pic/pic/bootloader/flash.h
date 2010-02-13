/*-------------------------------------------------------------------------
  flash.h - Program memory flashing process
-------------------------------------------------------------------------*/

#ifndef FLASH_H_
#define FLASH_H_

#include "common_types.h"

#define ERASE_FLASH_CMD        0
#define WRITE_FLASH_CMD        1
#define READ_FLASH_CMD         2
#define VALID_APPLICATION_CMD  3
#define RESET_CMD              4
#define SECTION_DESCRIPTOR_CMD 5

/* Sections attributes */
#define FLASH_SECTION_READ     0x01
#define FLASH_SECTION_WRITE    0x02

extern uint ep2_num_bytes_to_send;
extern uchar *ep2_source_data;

void erase_block(void);
void write_block(__data uchar *src);

#endif /*FLASH_H_*/
